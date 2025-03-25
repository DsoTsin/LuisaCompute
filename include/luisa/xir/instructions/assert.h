#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API AssertInst final : public PrintMessageMixin<DerivedInstruction<AssertInst, DerivedInstructionTag::ASSERT>> {

public:
    static constexpr size_t operand_index_condition = 0u;

public:
    AssertInst(BasicBlock *parent_block, Value *condition, luisa::string message = {}) noexcept;
    void set_condition(Value *condition) noexcept;
    [[nodiscard]] Value *condition() noexcept;
    [[nodiscard]] const Value *condition() const noexcept;
    [[nodiscard]] AssertInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
