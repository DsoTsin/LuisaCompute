#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API AllocaInst final : public InstructionOpMixin<AllocaOp, DerivedInstruction<AllocaInst, DerivedInstructionTag::ALLOCA>> {
public:
    AllocaInst(BasicBlock *parent_block, const Type *type, AllocaOp op) noexcept;
    [[nodiscard]] bool is_lvalue() const noexcept override { return true; }
    [[nodiscard]] AllocaInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
