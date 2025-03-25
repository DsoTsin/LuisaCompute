#include <luisa/core/logging.h>
#include <luisa/xir/basic_block.h>
#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/outline.h>

namespace luisa::compute::xir {

OutlineInst *OutlineInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto cloned = b.outline();
    auto resolved_target = resolver.resolve(target_block());
    LUISA_DEBUG_ASSERT(resolved_target == nullptr || resolved_target->isa<BasicBlock>(), "Invalid target block.");
    auto resolved_merge = resolver.resolve(merge_block());
    LUISA_DEBUG_ASSERT(resolved_merge == nullptr || resolved_merge->isa<BasicBlock>(), "Invalid merge block.");
    cloned->set_target_block(static_cast<BasicBlock *>(resolved_target));
    cloned->set_merge_block(static_cast<BasicBlock *>(resolved_merge));
    return cloned;
}

}// namespace luisa::compute::xir
