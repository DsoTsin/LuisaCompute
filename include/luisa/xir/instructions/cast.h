#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API CastInst final : public InstructionOpMixin<CastOp, DerivedInstruction<CastInst, DerivedInstructionTag::CAST>> {
public:
    CastInst(BasicBlock *parent_block, const Type *target_type, CastOp op, Value *value) noexcept;
    [[nodiscard]] Value *value() noexcept;
    [[nodiscard]] const Value *value() const noexcept;
    void set_value(Value *value) noexcept;
    [[nodiscard]] CastInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
