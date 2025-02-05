#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

enum struct CastOp {
    STATIC_CAST,
    BITWISE_CAST,
};

[[nodiscard]] LC_XIR_API luisa::string_view to_string(CastOp op) noexcept;
[[nodiscard]] LC_XIR_API CastOp cast_op_from_string(luisa::string_view name) noexcept;

class LC_XIR_API CastInst final : public DerivedInstruction<CastInst, DerivedInstructionTag::CAST>,
                                  public InstructionOpMixin<CastOp> {
public:
    CastInst(BasicBlock *parent_block, const Type *target_type, CastOp op, Value *value) noexcept;
    [[nodiscard]] Value *value() noexcept;
    [[nodiscard]] const Value *value() const noexcept;
    void set_value(Value *value) noexcept;
    [[nodiscard]] CastInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
