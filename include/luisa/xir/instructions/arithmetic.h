#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API ArithmeticInst final : public InstructionOpMixin<ArithmeticOp, DerivedInstruction<ArithmeticInst, DerivedInstructionTag::ARITHMETIC>> {
public:
    ArithmeticInst(BasicBlock *parent_block,
                   const Type *type, ArithmeticOp op,
                   luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] ArithmeticInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
