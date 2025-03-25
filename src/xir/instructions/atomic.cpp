#include <luisa/core/logging.h>
#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/atomic.h>

namespace luisa::compute::xir {

AtomicInst::AtomicInst(BasicBlock *parent_block, const Type *type, AtomicOp op,
                       Value *base, luisa::span<Value *const> indices,
                       luisa::span<Value *const> values) noexcept
    : Super{op, parent_block, type} {
    auto expected_value_count = this->value_count();
    LUISA_DEBUG_ASSERT(values.empty() || values.size() == expected_value_count,
                       "Invalid number of values for atomic instruction.");
    auto op_count = 1 + expected_value_count + indices.size();
    set_operand_count(op_count);
    if (base != nullptr) { set_base(base); }
    if (!indices.empty()) { set_indices(indices); }
    if (!values.empty()) { set_values(values); }
}

Value *AtomicInst::base() noexcept { return operand(0u); }
const Value *AtomicInst::base() const noexcept { return operand(0u); }

void AtomicInst::set_base(Value *base) noexcept {
    set_operand(0u, base);
}

void AtomicInst::set_index_count(size_t count) noexcept {
    luisa::fixed_vector<Value *, 2u> value_backup;
    for (auto u : value_uses()) { value_backup.emplace_back(u->value()); }
    auto op_count = 1u /* base */ + count + value_backup.size();
    set_operand_count(op_count);
    auto uses = this->value_uses();
    for (auto i = 0u; i < value_backup.size(); i++) { uses[i]->set_value(value_backup[i]); }
}

void AtomicInst::set_indices(luisa::span<Value *const> indices) noexcept {
    set_index_count(indices.size());
    auto uses = this->index_uses();
    for (auto i = 0u; i < indices.size(); i++) { uses[i]->set_value(indices[i]); }
}

void AtomicInst::set_values(luisa::span<Value *const> values) noexcept {
    auto uses = this->value_uses();
    if (values.empty()) {
        for (auto u : uses) { u->set_value(nullptr); }
    } else {
        LUISA_DEBUG_ASSERT(values.size() == uses.size(),
                           "Invalid number of values for atomic instruction.");
        for (auto i = 0u; i < uses.size(); i++) { uses[i]->set_value(values[i]); }
    }
}

AtomicInst *AtomicInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_base = resolver.resolve(base());
    luisa::fixed_vector<Value *, 16u> resolved_indices;
    resolved_indices.reserve(index_count());
    for (auto index_use : index_uses()) {
        resolved_indices.emplace_back(resolver.resolve(index_use->value()));
    }
    luisa::fixed_vector<Value *, 16u> resolved_values;
    resolved_values.reserve(value_count());
    for (auto value_use : value_uses()) {
        resolved_values.emplace_back(resolver.resolve(value_use->value()));
    }
    return b.call(type(), op(), resolved_base, resolved_indices, resolved_values);
}

Use *AtomicInst::base_use() noexcept { return operand_use(0u); }
const Use *AtomicInst::base_use() const noexcept { return operand_use(0u); }

size_t AtomicInst::index_count() const noexcept {
    auto op_count = operand_count();
    LUISA_DEBUG_ASSERT(op_count >= 1u + value_count(),
                       "Invalid number of operands for atomic instruction.");
    return op_count - 1u - value_count();
}

luisa::span<Use *const> AtomicInst::index_uses() noexcept {
    return operand_uses().subspan(1u /* base */, index_count());
}

luisa::span<const Use *const> AtomicInst::index_uses() const noexcept {
    return const_cast<AtomicInst *>(this)->index_uses();
}

luisa::span<Use *const> AtomicInst::value_uses() noexcept {
    auto op_uses = operand_uses();
    return op_uses.subspan(op_uses.size() - value_count());
}

luisa::span<const Use *const> AtomicInst::value_uses() const noexcept {
    return const_cast<AtomicInst *>(this)->value_uses();
}

}// namespace luisa::compute::xir
