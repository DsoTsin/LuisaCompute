#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/arithmetic.h>

namespace luisa::compute::xir {

ArithmeticInst::ArithmeticInst(BasicBlock *parent_block,
                               const Type *type, ArithmeticOp op,
                               luisa::span<Value *const> operands) noexcept
    : Super{parent_block, type}, InstructionOpMixin{op} {
    set_operands(operands);
}

ArithmeticInst *ArithmeticInst::clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept {
    luisa::fixed_vector<Value *, 16u> resolved_operands;
    resolved_operands.reserve(operand_count());
    for (auto op_use : operand_uses()) {
        resolved_operands.emplace_back(resolver.resolve(op_use->value()));
    }
    return b.call(type(), op(), resolved_operands);
}

}// namespace luisa::compute::xir
