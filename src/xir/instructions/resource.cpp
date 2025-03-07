#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/resource.h>

namespace luisa::compute::xir {

ResourceQueryInst::ResourceQueryInst(BasicBlock *parent_block, const Type *type, ResourceQueryOp op,
                                     luisa::span<Value *const> operands) noexcept
    : Super{parent_block, type}, InstructionOpMixin{op} { set_operands(operands); }

ResourceQueryInst *ResourceQueryInst::clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept {
    luisa::fixed_vector<Value *, 8u> cloned_operands;
    cloned_operands.reserve(operand_count());
    for (auto op_use : operand_uses()) {
        cloned_operands.emplace_back(resolver.resolve(op_use->value()));
    }
    return b.call(type(), op(), cloned_operands);
}

ResourceReadInst::ResourceReadInst(BasicBlock *parent_block, const Type *type, ResourceReadOp op,
                                   luisa::span<Value *const> operands) noexcept
    : Super{parent_block, type}, InstructionOpMixin{op} { set_operands(operands); }

ResourceReadInst *ResourceReadInst::clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept {
    luisa::fixed_vector<Value *, 8u> cloned_operands;
    cloned_operands.reserve(operand_count());
    for (auto op_use : operand_uses()) {
        cloned_operands.emplace_back(resolver.resolve(op_use->value()));
    }
    return b.call(type(), op(), cloned_operands);
}

ResourceWriteInst::ResourceWriteInst(BasicBlock *parent_block, ResourceWriteOp op,
                                     luisa::span<Value *const> operands) noexcept
    : Super{parent_block, nullptr}, InstructionOpMixin{op} { set_operands(operands); }

ResourceWriteInst *ResourceWriteInst::clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept {
    luisa::fixed_vector<Value *, 8u> cloned_operands;
    cloned_operands.reserve(operand_count());
    for (auto op_use : operand_uses()) {
        cloned_operands.emplace_back(resolver.resolve(op_use->value()));
    }
    return b.call(op(), cloned_operands);
}

}// namespace luisa::compute::xir
