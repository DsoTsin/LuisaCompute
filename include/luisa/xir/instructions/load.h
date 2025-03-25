#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API LoadInst final : public DerivedInstruction<LoadInst, DerivedInstructionTag::LOAD> {
public:
    LoadInst(BasicBlock *parent_block, const Type *type, Value *variable) noexcept;
    [[nodiscard]] Value *variable() noexcept;
    [[nodiscard]] const Value *variable() const noexcept;
    void set_variable(Value *variable) noexcept;
    [[nodiscard]] LoadInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
