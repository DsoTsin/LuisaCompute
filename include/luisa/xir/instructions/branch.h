#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API BranchInst final : public DerivedBranchInstruction<BranchInst, DerivedInstructionTag::BRANCH> {
public:
    using Super::Super;
    [[nodiscard]] BranchInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

class ConditionalBranchInst final : public DerivedConditionalBranchInstruction<ConditionalBranchInst, DerivedInstructionTag::CONDITIONAL_BRANCH> {
public:
    using Super::Super;
    [[nodiscard]] ConditionalBranchInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
