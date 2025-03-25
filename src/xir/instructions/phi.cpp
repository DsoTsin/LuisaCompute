#include <luisa/core/logging.h>
#include <luisa/xir/basic_block.h>
#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/phi.h>

namespace luisa::compute::xir {

PhiInst::PhiInst(BasicBlock *parent_block, const Type *type) noexcept
    : Super{parent_block, type} {}

void PhiInst::set_incoming_count(size_t count) noexcept {
    set_operand_count(count);
    _incoming_blocks.resize(count);
}

void PhiInst::set_incoming(size_t index, Value *value, BasicBlock *block) noexcept {
    LUISA_DEBUG_ASSERT(index < incoming_count(), "Phi incoming index out of range.");
    set_operand(index, value);
    _incoming_blocks[index] = block;
}

void PhiInst::add_incoming(Value *value, BasicBlock *block) noexcept {
    add_operand(value);
    _incoming_blocks.emplace_back(block);
}

void PhiInst::insert_incoming(size_t index, Value *value, BasicBlock *block) noexcept {
    insert_operand(index, value);
    _incoming_blocks.insert(_incoming_blocks.begin() + index, block);
}

void PhiInst::remove_incoming(size_t index) noexcept {
    if (index < incoming_count()) {
        remove_operand(index);
        _incoming_blocks.erase(_incoming_blocks.begin() + index);
    }
}

size_t PhiInst::incoming_count() const noexcept {
    return operand_count();
}

PhiIncoming PhiInst::incoming(size_t index) noexcept {
    LUISA_DEBUG_ASSERT(index < incoming_count(), "Phi incoming index out of range.");
    auto value = operand(index);
    auto block = _incoming_blocks[index];
    return {value, block};
}

ConstPhiIncoming PhiInst::incoming(size_t index) const noexcept {
    auto incoming = const_cast<PhiInst *>(this)->incoming(index);
    return {incoming.value, incoming.block};
}

PhiIncomingUse PhiInst::incoming_use(size_t index) noexcept {
    LUISA_DEBUG_ASSERT(index < incoming_count(), "Phi incoming index out of range.");
    auto value = operand_use(index);
    auto block = _incoming_blocks[index];
    return {value, block};
}

ConstPhiIncomingUse PhiInst::incoming_use(size_t index) const noexcept {
    auto incoming = const_cast<PhiInst *>(this)->incoming_use(index);
    return {incoming.value, incoming.block};
}

PhiInst *PhiInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto cloned = b.phi(type());
    for (auto i = 0u; i < incoming_count(); i++) {
        auto incoming = this->incoming(i);
        auto resolved_value = resolver.resolve(incoming.value);
        auto resolved_block = resolver.resolve(incoming.block);
        LUISA_DEBUG_ASSERT(resolved_block == nullptr || resolved_block->isa<BasicBlock>(), "Invalid incoming block.");
        cloned->add_incoming(resolved_value, static_cast<BasicBlock *>(resolved_block));
    }
    return cloned;
}

}// namespace luisa::compute::xir
