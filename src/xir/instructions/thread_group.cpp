#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/thread_group.h>

namespace luisa::compute::xir {

ThreadGroupInst::ThreadGroupInst(BasicBlock *parent_block, const Type *type, ThreadGroupOp op,
                                 luisa::span<Value *const> operands) noexcept
    : Super{op, parent_block, type} {
    if (!operands.empty()) {
        set_operands(operands);
    }
}

ThreadGroupInst *ThreadGroupInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    luisa::fixed_vector<Value *, 16u> resolved_operands;
    resolved_operands.reserve(operand_count());
    for (auto op_use : operand_uses()) {
        resolved_operands.emplace_back(resolver.resolve(op_use->value()));
    }
    return b.call(type(), op(), resolved_operands);
}

}// namespace luisa::compute::xir
