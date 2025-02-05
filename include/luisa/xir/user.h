#pragma once

#include <luisa/xir/value.h>

namespace luisa::compute::xir {

class BasicBlock;

class LC_XIR_API User : public Value {

private:
    luisa::vector<Use *> _operands;

protected:
    [[nodiscard]] virtual bool _should_add_self_to_operand_use_lists() const noexcept = 0;

public:
    using Value::Value;
    [[nodiscard]] bool is_user() const noexcept final { return true; }
    static void set_operand_use_value(Use *use, Value *value) noexcept;

    void set_operand_count(size_t n) noexcept;
    void set_operand(size_t index, Value *value) noexcept;
    void set_operands(luisa::span<Value *const> operands) noexcept;

    void add_operand(Value *value) noexcept;
    void insert_operand(size_t index, Value *value) noexcept;
    void remove_operand(size_t index) noexcept;

    [[nodiscard]] Use *operand_use(size_t index) noexcept;
    [[nodiscard]] const Use *operand_use(size_t index) const noexcept;

    [[nodiscard]] Value *operand(size_t index) noexcept;
    [[nodiscard]] const Value *operand(size_t index) const noexcept;

    [[nodiscard]] luisa::span<Use *> operand_uses() noexcept;
    [[nodiscard]] luisa::span<const Use *const> operand_uses() const noexcept;

    [[nodiscard]] size_t operand_count() const noexcept;
};

}// namespace luisa::compute::xir
