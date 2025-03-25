#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class BasicBlock;

// Branch instruction:
//
// if (cond) {
//   true_block
// } else {
//   false_block
// }
// { merge_block }
//
// Note: this instruction must be the terminator of a basic block.
class LC_XIR_API IfInst final : public ControlFlowMergeMixin<DerivedConditionalBranchInstruction<IfInst, DerivedInstructionTag::IF>> {
public:
    using Super::Super;
    [[nodiscard]] IfInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
