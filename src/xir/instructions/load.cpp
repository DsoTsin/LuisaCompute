#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/load.h>

namespace luisa::compute::xir {

LoadInst::LoadInst(BasicBlock *parent_block, const Type *type, Value *variable) noexcept
    : Super{parent_block, type} {
    auto operands = std::array{variable};
    set_operands(operands);
}

Value *LoadInst::variable() noexcept {
    return operand(0);
}

const Value *LoadInst::variable() const noexcept {
    return operand(0);
}

void LoadInst::set_variable(Value *variable) noexcept {
    return set_operand(0, variable);
}

LoadInst *LoadInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_variable = resolver.resolve(variable());
    return b.load(type(), resolved_variable);
}

}// namespace luisa::compute::xir
