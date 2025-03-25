#include <luisa/core/logging.h>
#include <luisa/xir/basic_block.h>
#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/continue.h>

namespace luisa::compute::xir {

ContinueInst *ContinueInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto cloned = b.continue_();
    auto resolved_target = resolver.resolve(target_block());
    LUISA_DEBUG_ASSERT(resolved_target == nullptr || resolved_target->isa<BasicBlock>(), "Invalid target block.");
    cloned->set_target_block(static_cast<BasicBlock *>(resolved_target));
    return cloned;
}

}// namespace luisa::compute::xir
