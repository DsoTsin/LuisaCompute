#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API ThreadGroupInst final : public InstructionOpMixin<ThreadGroupOp, DerivedInstruction<ThreadGroupInst, DerivedInstructionTag::THREAD_GROUP>> {
public:
    ThreadGroupInst(BasicBlock *parent_block,
                    const Type *type, ThreadGroupOp op,
                    luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] ThreadGroupInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
