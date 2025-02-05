#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

enum class ThreadGroupOp {

    // shader execution re-ordering
    SHADER_EXECUTION_REORDER,// (uint hint, uint hint_bits): void

    RASTER_QUAD_DDX,// (arg: floatN): floatN
    RASTER_QUAD_DDY,// (arg: floatN): floatN

    // warp operations
    WARP_IS_FIRST_ACTIVE_LANE,  // (): bool
    WARP_FIRST_ACTIVE_LANE,     // (): uint
    WARP_ACTIVE_ALL_EQUAL,      // (scalar/vector): boolN
    WARP_ACTIVE_BIT_AND,        // (intN): intN
    WARP_ACTIVE_BIT_OR,         // (intN): intN
    WARP_ACTIVE_BIT_XOR,        // (intN): intN
    WARP_ACTIVE_COUNT_BITS,     // (bool): uint
    WARP_ACTIVE_MAX,            // (type: scalar/vector): type
    WARP_ACTIVE_MIN,            // (type: scalar/vector): type
    WARP_ACTIVE_PRODUCT,        // (type: scalar/vector): type
    WARP_ACTIVE_SUM,            // (type: scalar/vector): type
    WARP_ACTIVE_ALL,            // (bool): bool
    WARP_ACTIVE_ANY,            // (bool): bool
    WARP_ACTIVE_BIT_MASK,       // (bool): uint4 (uint4 contained 128-bit)
    WARP_PREFIX_COUNT_BITS,     // (bool): uint (count bits before this lane)
    WARP_PREFIX_SUM,            // (type: scalar/vector): type (sum lanes before this lane)
    WARP_PREFIX_PRODUCT,        // (type: scalar/vector): type (multiply lanes before this lane)
    WARP_READ_LANE,             // (type: scalar/vector/matrix, index: uint): type (read this variable's value at this lane)
    WARP_READ_FIRST_ACTIVE_LANE,// (type: scalar/vector/matrix): type (read this variable's value at the first lane)

    // block synchronization
    SYNCHRONIZE_BLOCK,// ()
};

[[nodiscard]] LC_XIR_API luisa::string_view to_string(ThreadGroupOp op) noexcept;
[[nodiscard]] LC_XIR_API ThreadGroupOp thread_group_op_from_string(luisa::string_view name) noexcept;

class LC_XIR_API ThreadGroupInst final : public DerivedInstruction<ThreadGroupInst, DerivedInstructionTag::THREAD_GROUP>,
                                         public InstructionOpMixin<ThreadGroupOp> {
public:
    ThreadGroupInst(BasicBlock *parent_block,
                    const Type *type, ThreadGroupOp op,
                    luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] ThreadGroupInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
