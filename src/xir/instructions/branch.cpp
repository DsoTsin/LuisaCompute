#include <luisa/xir/builder.h>
#include <luisa/core/logging.h>
#include <luisa/xir/basic_block.h>
#include <luisa/xir/instructions/branch.h>

namespace luisa::compute::xir {

BranchInst *BranchInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto cloned = b.br();
    auto resolved_target = resolver.resolve(target_block());
    LUISA_DEBUG_ASSERT(resolved_target == nullptr || resolved_target->isa<BasicBlock>(), "Invalid target block.");
    cloned->set_target_block(static_cast<BasicBlock *>(resolved_target));
    return cloned;
}

ConditionalBranchInst *ConditionalBranchInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_cond = resolver.resolve(condition());
    auto cloned = b.cond_br(resolved_cond);
    auto resolved_true_block = resolver.resolve(true_block());
    LUISA_DEBUG_ASSERT(resolved_true_block == nullptr || resolved_true_block->isa<BasicBlock>(), "Invalid true block");
    auto resolved_false_block = resolver.resolve(false_block());
    LUISA_DEBUG_ASSERT(resolved_false_block == nullptr || resolved_false_block->isa<BasicBlock>(), "Invalid false block");
    cloned->set_true_target(static_cast<BasicBlock *>(resolved_true_block));
    cloned->set_false_target(static_cast<BasicBlock *>(resolved_false_block));
    return cloned;
}

}// namespace luisa::compute::xir
