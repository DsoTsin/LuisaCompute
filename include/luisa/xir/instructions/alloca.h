#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

enum struct AllocSpace {
    LOCAL,
    SHARED,
};

class LC_XIR_API AllocaInst final : public DerivedInstruction<AllocaInst, DerivedInstructionTag::ALLOCA> {

private:
    AllocSpace _space;

public:
    AllocaInst(BasicBlock *parent_block, const Type *type, AllocSpace space) noexcept;
    [[nodiscard]] bool is_lvalue() const noexcept override { return true; }
    void set_space(AllocSpace space) noexcept;
    [[nodiscard]] auto space() const noexcept { return _space; }
    [[nodiscard]] AllocaInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
