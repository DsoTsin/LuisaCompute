#include <luisa/core/logging.h>
#include <luisa/xir/function.h>
#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/call.h>

namespace luisa::compute::xir {

CallInst::CallInst(BasicBlock *parent_block, const Type *type, Function *callee,
                   luisa::span<Value *const> arguments) noexcept
    : Super{parent_block, type} {
    set_operand_count(1u + arguments.size());
    set_operand(operand_index_callee, callee);
    for (auto i = 0u; i < arguments.size(); i++) {
        set_operand(operand_index_argument_offset + i, arguments[i]);
    }
}

Function *CallInst::callee() noexcept {
    auto callee = operand(operand_index_callee);
    LUISA_DEBUG_ASSERT(callee->isa<Function>(), "Invalid callee.");
    return static_cast<Function *>(callee);
}

const Function *CallInst::callee() const noexcept {
    return const_cast<CallInst *>(this)->callee();
}

void CallInst::set_callee(Function *callee) noexcept {
    set_operand(operand_index_callee, callee);
}

void CallInst::set_arguments(luisa::span<Value *const> arguments) noexcept {
    set_operand_count(1u + arguments.size());
    for (auto i = 0u; i < arguments.size(); i++) {
        set_operand(operand_index_argument_offset + i, arguments[i]);
    }
}

void CallInst::set_argument(size_t index, Value *argument) noexcept {
    set_operand(operand_index_argument_offset + index, argument);
}

void CallInst::add_argument(Value *argument) noexcept {
    add_operand(argument);
}

void CallInst::insert_argument(size_t index, Value *argument) noexcept {
    insert_operand(operand_index_argument_offset + index, argument);
}

void CallInst::remove_argument(size_t index) noexcept {
    remove_operand(operand_index_argument_offset + index);
}

CallInst *CallInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_callee = resolver.resolve(callee());
    LUISA_DEBUG_ASSERT(resolved_callee == nullptr || resolved_callee->isa<Function>(), "Invalid callee.");
    luisa::fixed_vector<Value *, 16u> resolved_args;
    resolved_args.reserve(argument_count());
    for (auto arg_use : argument_uses()) {
        resolved_args.emplace_back(resolver.resolve(arg_use->value()));
    }
    return b.call(type(), static_cast<Function *>(resolved_callee), resolved_args);
}

}// namespace luisa::compute::xir
