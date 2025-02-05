#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API AutodiffScopeInst final : public ControlFlowMergeMixin<DerivedTerminatorInstruction<AutodiffScopeInst, DerivedInstructionTag::AUTODIFF_SCOPE>> {

public:
    static constexpr size_t operand_index_entry_block = 0u;

public:
    explicit AutodiffScopeInst(BasicBlock *parent_block) noexcept;
    void set_entry_block(BasicBlock *block) noexcept;
    BasicBlock *create_entry_block(bool overwrite_existing = false) noexcept;
    [[nodiscard]] BasicBlock *entry_block() noexcept;
    [[nodiscard]] const BasicBlock *entry_block() const noexcept;
    [[nodiscard]] AutodiffScopeInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

enum struct AutodiffIntrinsicOp {
    AUTODIFF_REQUIRES_GRADIENT,  // (expr) -> void
    AUTODIFF_GRADIENT,           // (expr) -> expr
    AUTODIFF_GRADIENT_MARKER,    // (ref, expr) -> void
    AUTODIFF_ACCUMULATE_GRADIENT,// (ref, expr) -> void
    AUTODIFF_BACKWARD,           // (expr) -> void
    AUTODIFF_DETACH,             // (expr) -> expr
};

[[nodiscard]] LC_XIR_API luisa::string_view to_string(AutodiffIntrinsicOp op) noexcept;
[[nodiscard]] LC_XIR_API AutodiffIntrinsicOp intrinsic_op_from_string(luisa::string_view name) noexcept;

class LC_XIR_API AutodiffIntrinsicInst final : public DerivedInstruction<AutodiffIntrinsicInst, DerivedInstructionTag::AUTODIFF_INTRINSIC>,
                                               public InstructionOpMixin<AutodiffIntrinsicOp> {
public:
    AutodiffIntrinsicInst(BasicBlock *parent_block, const Type *type, AutodiffIntrinsicOp op,
                          luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] AutodiffIntrinsicInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
