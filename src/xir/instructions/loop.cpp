#include <luisa/core/logging.h>
#include <luisa/ast/type_registry.h>
#include <luisa/xir/basic_block.h>
#include <luisa/xir/function.h>
#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/loop.h>

namespace luisa::compute::xir {

LoopInst::LoopInst(BasicBlock *parent_block) noexcept : Super{parent_block} {
    set_operands(std::array{static_cast<Value *>(nullptr)});
}

void LoopInst::set_prepare_block(BasicBlock *block) noexcept {
    set_operand(operand_index_prepare_block, block);
}

void LoopInst::set_body_block(BasicBlock *block) noexcept {
    _body_block = block;
}

void LoopInst::set_update_block(BasicBlock *block) noexcept {
    _update_block = block;
}

BasicBlock *LoopInst::create_prepare_block(bool overwrite_existing) noexcept {
    LUISA_ASSERT(prepare_block() == nullptr || overwrite_existing, "Prepare block already exists.");
    auto new_block = parent_function()->create_basic_block();
    set_prepare_block(new_block);
    return new_block;
}

BasicBlock *LoopInst::create_body_block(bool overwrite_existing) noexcept {
    LUISA_ASSERT(body_block() == nullptr || overwrite_existing, "Body block already exists.");
    auto new_block = parent_function()->create_basic_block();
    set_body_block(new_block);
    return new_block;
}

BasicBlock *LoopInst::create_update_block(bool overwrite_existing) noexcept {
    LUISA_ASSERT(update_block() == nullptr || overwrite_existing, "Update block already exists.");
    auto new_block = parent_function()->create_basic_block();
    set_update_block(new_block);
    return new_block;
}

BasicBlock *LoopInst::prepare_block() noexcept {
    return static_cast<BasicBlock *>(operand(operand_index_prepare_block));
}

const BasicBlock *LoopInst::prepare_block() const noexcept {
    return const_cast<LoopInst *>(this)->prepare_block();
}

BasicBlock *LoopInst::body_block() noexcept {
    return _body_block;
}

const BasicBlock *LoopInst::body_block() const noexcept {
    return _body_block;
}

BasicBlock *LoopInst::update_block() noexcept {
    return _update_block;
}

const BasicBlock *LoopInst::update_block() const noexcept {
    return _update_block;
}

LoopInst *LoopInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto cloned = b.loop();
    auto resolved_prepare_block = resolver.resolve(prepare_block());
    LUISA_DEBUG_ASSERT(resolved_prepare_block == nullptr || resolved_prepare_block->isa<BasicBlock>(), "Invalid prepare block.");
    auto resolved_body_block = resolver.resolve(body_block());
    LUISA_DEBUG_ASSERT(resolved_body_block == nullptr || resolved_body_block->isa<BasicBlock>(), "Invalid body block.");
    auto resolved_update_block = resolver.resolve(update_block());
    LUISA_DEBUG_ASSERT(resolved_update_block == nullptr || resolved_update_block->isa<BasicBlock>(), "Invalid update block.");
    auto resolved_merge_block = resolver.resolve(merge_block());
    LUISA_DEBUG_ASSERT(resolved_merge_block == nullptr || resolved_merge_block->isa<BasicBlock>(), "Invalid merge block.");
    cloned->set_prepare_block(static_cast<BasicBlock *>(resolved_prepare_block));
    cloned->set_body_block(static_cast<BasicBlock *>(resolved_body_block));
    cloned->set_update_block(static_cast<BasicBlock *>(resolved_update_block));
    cloned->set_merge_block(static_cast<BasicBlock *>(resolved_merge_block));
    return cloned;
}

SimpleLoopInst::SimpleLoopInst(BasicBlock *parent_block) noexcept : Super{parent_block} {
    set_operands(std::array{static_cast<Value *>(nullptr)});
}

void SimpleLoopInst::set_body_block(BasicBlock *block) noexcept {
    set_operand(operand_index_body_block, block);
}

BasicBlock *SimpleLoopInst::create_body_block(bool overwrite_existing) noexcept {
    LUISA_ASSERT(body_block() == nullptr || overwrite_existing, "Body block already exists.");
    auto new_block = parent_function()->create_basic_block();
    set_body_block(new_block);
    return new_block;
}

BasicBlock *SimpleLoopInst::body_block() noexcept {
    return static_cast<BasicBlock *>(operand(operand_index_body_block));
}

const BasicBlock *SimpleLoopInst::body_block() const noexcept {
    return const_cast<SimpleLoopInst *>(this)->body_block();
}

SimpleLoopInst *SimpleLoopInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto cloned = b.simple_loop();
    auto resolved_body_block = resolver.resolve(body_block());
    LUISA_DEBUG_ASSERT(resolved_body_block == nullptr || resolved_body_block->isa<BasicBlock>(), "Invalid body block.");
    auto resolved_merge_block = resolver.resolve(merge_block());
    LUISA_DEBUG_ASSERT(resolved_merge_block == nullptr || resolved_merge_block->isa<BasicBlock>(), "Invalid merge block.");
    cloned->set_body_block(static_cast<BasicBlock *>(resolved_body_block));
    cloned->set_merge_block(static_cast<BasicBlock *>(resolved_merge_block));
    return cloned;
}

}// namespace luisa::compute::xir
