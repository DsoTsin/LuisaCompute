#include <luisa/core/logging.h>
#include <luisa/xir/basic_block.h>

namespace luisa::compute::xir {

void BasicBlock::_do_traverse_predecessors(bool exclude_self, void *ctx, void (*visit)(void *, BasicBlock *)) noexcept {
#ifndef NDEBUG
    luisa::fixed_vector<BasicBlock *, 16u> visited;
#endif
    // we can find all predecessors by traversing all users of the block and find their containing blocks
    for (auto &&use : use_list()) {
        auto user = use.user();
        LUISA_ASSERT(user != nullptr && user->isa<Instruction>(), "Invalid user of basic block.");
        auto user_block = static_cast<Instruction *>(user)->parent_block();
        LUISA_DEBUG_ASSERT(user_block != nullptr, "Invalid parent block.");
        if ((!exclude_self || user_block != this) && user_block->isa<BasicBlock>()) {
#ifndef NDEBUG
            LUISA_ASSERT(std::find(visited.begin(), visited.end(), user_block) == visited.end(),
                         "Duplicate block in predecessor list.");
            visited.emplace_back(user_block);
#endif
            visit(ctx, user_block);
        }
    }
}

void BasicBlock::_do_traverse_successors(bool exclude_self, void *ctx, void (*visit)(void *, BasicBlock *)) noexcept {
#ifndef NDEBUG
    luisa::fixed_vector<BasicBlock *, 16u> visited;
#endif
    // we can find all successors by finding the block operands of the terminator instruction
    auto terminator = this->terminator();
    for (auto op_use : terminator->operand_uses()) {
        LUISA_DEBUG_ASSERT(op_use != nullptr, "Invalid operand use.");
        if (auto op = op_use->value(); op != nullptr && (!exclude_self || op != this) && op->isa<BasicBlock>()) {
#ifndef NDEBUG
            // check that we don't visit the same block twice
            LUISA_ASSERT(std::find(visited.begin(), visited.end(), op) == visited.end(),
                         "Duplicate block in successor list.");
            visited.emplace_back(static_cast<BasicBlock *>(op));
#endif
            visit(ctx, static_cast<BasicBlock *>(op));
        }
    }
}

BasicBlock::BasicBlock(Function *function) noexcept
    : Super{function, nullptr}, _instructions{this} {}

bool BasicBlock::is_terminated() const noexcept {
    return _instructions.back().is_terminator();
}

TerminatorInstruction *BasicBlock::terminator() noexcept {
    LUISA_DEBUG_ASSERT(is_terminated(), "Basic block is not terminated.");
    return static_cast<TerminatorInstruction *>(&_instructions.back());
}

const TerminatorInstruction *BasicBlock::terminator() const noexcept {
    return const_cast<BasicBlock *>(this)->terminator();
}

}// namespace luisa::compute::xir
