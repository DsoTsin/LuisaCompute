#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/return.h>

namespace luisa::compute::xir {

ReturnInst::ReturnInst(BasicBlock *parent_block, Value *value) noexcept : Super{parent_block} {
    set_operands(std::array{value});
}

void ReturnInst::set_return_value(Value *value) noexcept {
    set_operand(operand_index_return_value, value);
}

Value *ReturnInst::return_value() noexcept {
    return operand(operand_index_return_value);
}

const Value *ReturnInst::return_value() const noexcept {
    return operand(operand_index_return_value);
}

const Type *ReturnInst::return_type() const noexcept {
    auto ret_value = return_value();
    return ret_value == nullptr ? nullptr : ret_value->type();
}

ReturnInst *ReturnInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_value = resolver.resolve(return_value());
    return b.return_(resolved_value);
}

}// namespace luisa::compute::xir
