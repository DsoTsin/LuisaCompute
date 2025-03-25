#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API StoreInst final : public DerivedInstruction<StoreInst, DerivedInstructionTag::STORE> {

public:
    static constexpr size_t operand_index_variable = 0u;
    static constexpr size_t operand_index_value = 1u;

public:
    StoreInst(BasicBlock *parent_block, Value *variable, Value *value) noexcept;

    [[nodiscard]] auto variable() noexcept { return operand(operand_index_variable); }
    [[nodiscard]] auto variable() const noexcept { return operand(operand_index_variable); }
    [[nodiscard]] auto value() noexcept { return operand(operand_index_value); }
    [[nodiscard]] auto value() const noexcept { return operand(operand_index_value); }

    void set_variable(Value *variable) noexcept;
    void set_value(Value *value) noexcept;

    [[nodiscard]] StoreInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
