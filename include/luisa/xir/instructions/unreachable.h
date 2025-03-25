#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

// Note: this instruction must be the terminator of a basic block.
class LC_XIR_API UnreachableInst final : public PrintMessageMixin<DerivedTerminatorInstruction<UnreachableInst, DerivedInstructionTag::UNREACHABLE>> {
public:
    explicit UnreachableInst(BasicBlock *parent_block, luisa::string message = {}) noexcept;
    [[nodiscard]] UnreachableInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
