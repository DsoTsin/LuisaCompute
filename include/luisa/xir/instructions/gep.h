#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

// Get element pointer instruction.
class LC_XIR_API GEPInst final : public DerivedInstruction<GEPInst, DerivedInstructionTag::GEP> {

public:
    static constexpr size_t operand_index_base = 0u;
    static constexpr size_t operand_index_index_offset = 1u;

public:
    explicit GEPInst(BasicBlock *parent_block, const Type *type,
                     Value *base, luisa::span<Value *const> indices = {}) noexcept;
    [[nodiscard]] bool is_lvalue() const noexcept override { return true; }

    [[nodiscard]] auto base() noexcept { return operand(operand_index_base); }
    [[nodiscard]] auto base() const noexcept { return operand(operand_index_base); }
    [[nodiscard]] auto index(size_t i) noexcept { return operand(operand_index_index_offset + i); }
    [[nodiscard]] auto index(size_t i) const noexcept { return operand(operand_index_index_offset + i); }
    [[nodiscard]] auto index_uses() noexcept { return operand_uses().subspan(operand_index_index_offset); }
    [[nodiscard]] auto index_uses() const noexcept { return operand_uses().subspan(operand_index_index_offset); }
    [[nodiscard]] auto index_count() const noexcept { return index_uses().size(); }

    void set_base(Value *base) noexcept;
    void set_indices(luisa::span<Value *const> indices) noexcept;
    void set_index(size_t i, Value *index) noexcept;
    void add_index(Value *index) noexcept;
    void insert_index(size_t i, Value *index) noexcept;
    void remove_index(size_t i) noexcept;

    [[nodiscard]] GEPInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
