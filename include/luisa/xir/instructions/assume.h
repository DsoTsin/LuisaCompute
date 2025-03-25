#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API AssumeInst final : public PrintMessageMixin<DerivedInstruction<AssumeInst, DerivedInstructionTag::ASSUME>> {

public:
    static constexpr size_t operand_index_condition = 0u;

public:
    AssumeInst(BasicBlock *parent_block, Value *condition, luisa::string message = {}) noexcept;
    void set_condition(Value *condition) noexcept;
    [[nodiscard]] Value *condition() noexcept;
    [[nodiscard]] const Value *condition() const noexcept;
    [[nodiscard]] AssumeInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
