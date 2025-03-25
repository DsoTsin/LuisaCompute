#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/alloca.h>

namespace luisa::compute::xir {

AllocaInst::AllocaInst(BasicBlock *parent_block, const Type *type, AllocaOp op) noexcept
    : Super{op, parent_block, type} {}

AllocaInst *AllocaInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    return b.alloca_(type(), op());
}

}// namespace luisa::compute::xir
