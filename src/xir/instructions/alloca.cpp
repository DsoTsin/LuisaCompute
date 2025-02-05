#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/alloca.h>

namespace luisa::compute::xir {

AllocaInst::AllocaInst(BasicBlock *parent_block, const Type *type, AllocSpace space) noexcept
    : Super{parent_block, type}, _space{space} {}

void AllocaInst::set_space(AllocSpace space) noexcept {
    _space = space;
}

AllocaInst *AllocaInst::clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept {
    return b.alloca_(type(), space());
}

}// namespace luisa::compute::xir
