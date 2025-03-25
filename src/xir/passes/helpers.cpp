#include <luisa/core/logging.h>
#include <luisa/xir/undefined.h>
#include <luisa/xir/function.h>
#include <luisa/xir/builder.h>

#include "helpers.h"

namespace luisa::compute::xir {

Value *trace_pointer_base_value(Value *pointer) noexcept {
    return pointer != nullptr && pointer->isa<GEPInst>() ?
               trace_pointer_base_value(static_cast<GEPInst *>(pointer)->base()) :
               pointer;
}

AllocaInst *trace_pointer_base_local_alloca_inst(Value *pointer) noexcept {
    if (auto base = trace_pointer_base_value(pointer);
        base != nullptr && base->isa<AllocaInst>() &&
        static_cast<AllocaInst *>(base)->op() == AllocaOp::LOCAL) {
        return static_cast<AllocaInst *>(base);
    }
    return nullptr;
}

bool remove_redundant_phi_instruction(PhiInst *phi) noexcept {
    if (phi->use_list().empty()) {
        phi->remove_self();
        return true;
    }
    static constexpr auto is_invariant = [](Value *v) noexcept {
        if (v == nullptr) { return true; }
        switch (v->derived_value_tag()) {
            case DerivedValueTag::UNDEFINED: [[fallthrough]];
            case DerivedValueTag::CONSTANT: [[fallthrough]];
            case DerivedValueTag::ARGUMENT: [[fallthrough]];
            case DerivedValueTag::SPECIAL_REGISTER: return true;
            default: break;
        }
        return false;
    };
    auto all_same = true;
    auto any_undef = false;
    auto same_incoming = static_cast<Value *>(nullptr);
    for (auto value_use : phi->incoming_value_uses()) {
        auto value = value_use->value();
        LUISA_DEBUG_ASSERT(value != nullptr, "Invalid incoming value.");
        if (same_incoming == nullptr || same_incoming->isa<Undefined>()) { same_incoming = value; }
        if (value->isa<Undefined>()) {
            any_undef = true;
        } else if (same_incoming != value) {
            all_same = false;
            break;
        }
    }
    if (all_same && (!any_undef || is_invariant(same_incoming))) {
        if (same_incoming != nullptr) {
            phi->replace_all_uses_with(same_incoming);
        } else {
            LUISA_DEBUG_ASSERT(phi->use_list().empty(), "Invalid phi node.");
        }
        phi->remove_self();
        return true;
    }
    return false;
}

void lower_phi_node_to_local_variable(PhiInst *phi) noexcept {
    if (!remove_redundant_phi_instruction(phi)) {
        auto f = phi->parent_function();
        LUISA_DEBUG_ASSERT(f != nullptr && f->definition() != nullptr, "Invalid function.");
        XIRBuilder b;
        // create alloca at the beginning of the function
        b.set_insertion_point(f->definition()->body_block()->instructions().head_sentinel());
        auto phi_alloca = b.alloca_local(phi->type());
        phi_alloca->add_comment("alloca to lower phi node");
        // store incoming values at the end of their respective blocks
        for (auto i = 0u; i < phi->incoming_count(); i++) {
            if (auto incoming = phi->incoming(i); incoming.value != nullptr && !incoming.value->isa<Undefined>()) {
                LUISA_DEBUG_ASSERT(incoming.block != nullptr, "Invalid incoming block.");
                b.set_insertion_point(incoming.block->terminator()->prev());
                b.store(phi_alloca, incoming.value);
            }
        }
        // replace phi uses with local load instructions
        b.set_insertion_point(phi);
        auto phi_load = b.load(phi->type(), phi_alloca);
        phi_load->add_comment("load from phi alloca");
        phi->replace_all_uses_with(phi_load);
        phi->remove_self();
    }
}

void hoist_alloca_instructions_to_entry_block(FunctionDefinition *f) noexcept {
    luisa::vector<AllocaInst *> collected;
    f->traverse_instructions([&](Instruction *inst) noexcept {
        if (inst->isa<AllocaInst>()) {
            collected.emplace_back(static_cast<AllocaInst *>(inst));
        }
    });
    if (!collected.empty()) {
        XIRBuilder b;
        b.set_insertion_point(f->body_block()->instructions().head_sentinel());
        for (auto inst : collected) {
            inst->remove_self();
            b.append(inst);
        }
    }
}

}// namespace luisa::compute::xir
