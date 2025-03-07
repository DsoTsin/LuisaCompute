#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

// Note: this instruction must be the terminator of a basic block.
class LC_XIR_API UnreachableInst final : public DerivedTerminatorInstruction<UnreachableInst, DerivedInstructionTag::UNREACHABLE> {

private:
    luisa::string _message;

public:
    explicit UnreachableInst(BasicBlock *parent_block, luisa::string message = {}) noexcept;
    void set_message(luisa::string_view message) noexcept;
    [[nodiscard]] luisa::string_view message() const noexcept;
    [[nodiscard]] UnreachableInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
