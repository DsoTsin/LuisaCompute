#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class BasicBlock;

struct PhiIncoming {
    Value *value;
    BasicBlock *block;
};

struct PhiIncomingUse {
    Use *value;
    BasicBlock *block;
};

struct ConstPhiIncoming {
    const Value *value;
    const BasicBlock *block;
};

struct ConstPhiIncomingUse {
    const Use *value;
    const BasicBlock *block;
};

class LC_XIR_API PhiInst final : public DerivedInstruction<PhiInst, DerivedInstructionTag::PHI> {

private:
    luisa::vector<BasicBlock *> _incoming_blocks;

public:
    PhiInst(BasicBlock *parent_block, const Type *type) noexcept;
    void set_incoming_count(size_t count) noexcept;
    void set_incoming(size_t index, Value *value, BasicBlock *block) noexcept;
    void add_incoming(Value *value, BasicBlock *block) noexcept;
    void insert_incoming(size_t index, Value *value, BasicBlock *block) noexcept;
    void remove_incoming(size_t index) noexcept;
    [[nodiscard]] size_t incoming_count() const noexcept;
    [[nodiscard]] PhiIncoming incoming(size_t index) noexcept;
    [[nodiscard]] ConstPhiIncoming incoming(size_t index) const noexcept;
    [[nodiscard]] PhiIncomingUse incoming_use(size_t index) noexcept;
    [[nodiscard]] ConstPhiIncomingUse incoming_use(size_t index) const noexcept;
    [[nodiscard]] auto incoming_value_uses() noexcept { return operand_uses(); }
    [[nodiscard]] auto incoming_value_uses() const noexcept { return operand_uses(); }
    [[nodiscard]] auto incoming_blocks() noexcept { return luisa::span{_incoming_blocks}; }
    [[nodiscard]] auto incoming_blocks() const noexcept { return luisa::span{_incoming_blocks}; }
    [[nodiscard]] PhiInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
