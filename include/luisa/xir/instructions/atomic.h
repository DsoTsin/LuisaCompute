#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API AtomicInst final : public InstructionOpMixin<AtomicOp, DerivedInstruction<AtomicInst, DerivedInstructionTag::ATOMIC>> {
public:
    AtomicInst(BasicBlock *parent_block, const Type *type, AtomicOp op, Value *base,
               luisa::span<Value *const> indices = {}, luisa::span<Value *const> values = {}) noexcept;

    [[nodiscard]] Value *base() noexcept;
    [[nodiscard]] const Value *base() const noexcept;
    void set_base(Value *base) noexcept;

    [[nodiscard]] Use *base_use() noexcept;
    [[nodiscard]] const Use *base_use() const noexcept;

    [[nodiscard]] size_t index_count() const noexcept;
    void set_index_count(size_t count) noexcept;

    [[nodiscard]] luisa::span<Use *const> index_uses() noexcept;
    [[nodiscard]] luisa::span<const Use *const> index_uses() const noexcept;
    void set_indices(luisa::span<Value *const> indices) noexcept;

    [[nodiscard]] size_t value_count() const noexcept {
        return atomic_op_value_count(this->op());
    }

    [[nodiscard]] luisa::span<Use *const> value_uses() noexcept;
    [[nodiscard]] luisa::span<const Use *const> value_uses() const noexcept;
    void set_values(luisa::span<Value *const> values) noexcept;

    [[nodiscard]] AtomicInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
