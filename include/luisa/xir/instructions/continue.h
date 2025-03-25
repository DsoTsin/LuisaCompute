#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

// Note: this instruction must be the terminator of a basic block.
class LC_XIR_API ContinueInst final : public DerivedBranchInstruction<ContinueInst, DerivedInstructionTag::CONTINUE> {
public:
    using Super::Super;
    [[nodiscard]] ContinueInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
