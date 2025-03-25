#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API ResourceQueryInst final : public InstructionOpMixin<ResourceQueryOp, DerivedInstruction<ResourceQueryInst, DerivedInstructionTag::RESOURCE_QUERY>> {
public:
    ResourceQueryInst(BasicBlock *parent_block,
                      const Type *type, ResourceQueryOp op,
                      luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] ResourceQueryInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

class LC_XIR_API ResourceReadInst final : public InstructionOpMixin<ResourceReadOp, DerivedInstruction<ResourceReadInst, DerivedInstructionTag::RESOURCE_READ>> {
public:
    ResourceReadInst(BasicBlock *parent_block,
                     const Type *type, ResourceReadOp op,
                     luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] ResourceReadInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

class LC_XIR_API ResourceWriteInst final : public InstructionOpMixin<ResourceWriteOp, DerivedInstruction<ResourceWriteInst, DerivedInstructionTag::RESOURCE_WRITE>> {
public:
    ResourceWriteInst(BasicBlock *parent_block, ResourceWriteOp op,
                      luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] ResourceWriteInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
