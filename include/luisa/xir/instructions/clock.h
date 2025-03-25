#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API ClockInst final : public DerivedInstruction<ClockInst, DerivedInstructionTag::CLOCK> {
public:
    explicit ClockInst(BasicBlock *parent_block) noexcept;
    [[nodiscard]] ClockInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
