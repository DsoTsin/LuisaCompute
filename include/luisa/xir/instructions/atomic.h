#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

enum class AtomicOp {
    EXCHANGE,        /// [(base, indices..., desired) -> old]: stores desired, returns old.
    COMPARE_EXCHANGE,/// [(base, indices..., expected, desired) -> old]: stores (old == expected ? desired : old), returns old.
    FETCH_ADD,       /// [(base, indices..., val) -> old]: stores (old + val), returns old.
    FETCH_SUB,       /// [(base, indices..., val) -> old]: stores (old - val), returns old.
    FETCH_AND,       /// [(base, indices..., val) -> old]: stores (old & val), returns old.
    FETCH_OR,        /// [(base, indices..., val) -> old]: stores (old | val), returns old.
    FETCH_XOR,       /// [(base, indices..., val) -> old]: stores (old ^ val), returns old.
    FETCH_MIN,       /// [(base, indices..., val) -> old]: stores min(old, val), returns old.
    FETCH_MAX,       /// [(base, indices..., val) -> old]: stores max(old, val), returns old.
};

[[nodiscard]] LC_XIR_API luisa::string_view to_string(AtomicOp op) noexcept;
[[nodiscard]] LC_XIR_API AtomicOp atomic_op_from_string(luisa::string_view name) noexcept;

[[nodiscard]] constexpr auto atomic_op_value_count(AtomicOp op) noexcept {
    return op == AtomicOp::COMPARE_EXCHANGE ? 2u : 1u;
}

class LC_XIR_API AtomicInst final : public DerivedInstruction<AtomicInst, DerivedInstructionTag::ATOMIC>,
                                    public InstructionOpMixin<AtomicOp> {
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

    [[nodiscard]] AtomicInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
