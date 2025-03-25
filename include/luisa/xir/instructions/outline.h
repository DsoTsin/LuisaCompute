#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class BasicBlock;

class LC_XIR_API OutlineInst final : public ControlFlowMergeMixin<DerivedBranchInstruction<OutlineInst, DerivedInstructionTag::OUTLINE>> {
public:
    using Super::Super;
    [[nodiscard]] OutlineInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
