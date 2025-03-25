#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/gep.h>

namespace luisa::compute::xir {

GEPInst::GEPInst(BasicBlock *parent_block, const Type *type,
                 Value *base, luisa::span<Value *const> indices) noexcept
    : Super{parent_block, type} {
    set_operand_count(1u + indices.size());
    set_operand(operand_index_base, base);
    for (size_t i = 0u; i < indices.size(); ++i) {
        set_operand(operand_index_index_offset + i, indices[i]);
    }
}

void GEPInst::set_base(Value *base) noexcept {
    set_operand(operand_index_base, base);
}

void GEPInst::set_indices(luisa::span<Value *const> indices) noexcept {
    set_operand_count(1u + indices.size());
    for (size_t i = 0u; i < indices.size(); ++i) {
        set_operand(operand_index_index_offset + i, indices[i]);
    }
}

void GEPInst::set_index(size_t i, Value *index) noexcept {
    set_operand(operand_index_index_offset + i, index);
}

void GEPInst::add_index(Value *index) noexcept {
    add_operand(index);
}

void GEPInst::insert_index(size_t i, Value *index) noexcept {
    insert_operand(operand_index_index_offset + i, index);
}

void GEPInst::remove_index(size_t i) noexcept {
    remove_operand(operand_index_index_offset + i);
}

GEPInst *GEPInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_base = resolver.resolve(base());
    luisa::fixed_vector<Value *, 16u> resolved_indices;
    resolved_indices.reserve(index_count());
    for (auto index_use : index_uses()) {
        resolved_indices.emplace_back(resolver.resolve(index_use->value()));
    }
    return b.gep(type(), resolved_base, resolved_indices);
}

}// namespace luisa::compute::xir
