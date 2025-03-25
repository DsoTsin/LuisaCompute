#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/unreachable.h>

namespace luisa::compute::xir {

UnreachableInst::UnreachableInst(BasicBlock *parent_block, luisa::string message) noexcept
    : Super{std::move(message), parent_block} {}

UnreachableInst *UnreachableInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    return b.unreachable_(message());
}

}// namespace luisa::compute::xir
