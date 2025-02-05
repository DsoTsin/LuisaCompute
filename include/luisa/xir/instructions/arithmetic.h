#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

enum class ArithmeticOp {

    // unary operators
    UNARY_PLUS,   // +x
    UNARY_MINUS,  // -x
    UNARY_BIT_NOT,// ~x

    // binary operators
    BINARY_ADD,
    BINARY_SUB,
    BINARY_MUL,
    BINARY_DIV,
    BINARY_MOD,

    BINARY_BIT_AND,
    BINARY_BIT_OR,
    BINARY_BIT_XOR,

    BINARY_SHIFT_LEFT,
    BINARY_SHIFT_RIGHT,
    BINARY_ROTATE_LEFT,
    BINARY_ROTATE_RIGHT,

    BINARY_LESS,
    BINARY_GREATER,
    BINARY_LESS_EQUAL,
    BINARY_GREATER_EQUAL,
    BINARY_EQUAL,
    BINARY_NOT_EQUAL,

    // math
    ALL,// (boolN)
    ANY,// (boolN)

    SELECT,  // (vecN, vecN, boolN)
    CLAMP,   // (vecN, vecN, vecN)
    SATURATE,// (vecN)
    LERP,    // (vecN, vecN, vecN)

    SMOOTHSTEP,// (vecN, vecN, vecN)
    STEP,      // (x, y): (x >= y) ? 1 : 0

    ABS,// (vecN)
    MIN,// (vecN)
    MAX,// (vecN)

    CLZ,     // (uintN)
    CTZ,     // (uintN)
    POPCOUNT,// (uintN)
    REVERSE, // (uintN)

    ISINF,// (floatN)
    ISNAN,// (floatN)

    ACOS, // (floatN)
    ACOSH,// (floatN)
    ASIN, // (floatN)
    ASINH,// (floatN)
    ATAN, // (floatN)
    ATAN2,// (floatN, floatN)
    ATANH,// (floatN)

    COS, // (floatN)
    COSH,// (floatN)
    SIN, // (floatN)
    SINH,// (floatN)
    TAN, // (floatN)
    TANH,// (floatN)

    EXP,    // (floatN)
    EXP2,   // (floatN)
    EXP10,  // (floatN)
    LOG,    // (floatN)
    LOG2,   // (floatN)
    LOG10,  // (floatN)
    POW,    // (floatN, floatN)
    POW_INT,// (floatN, intN)

    SQRT, // (floatN)
    RSQRT,// (floatN)

    CEIL, // (floatN)
    FLOOR,// (floatN)
    FRACT,// (floatN)
    TRUNC,// (floatN)
    ROUND,// (floatN)
    RINT, // (floatN)

    FMA,     // (a: floatN, b: floatN, c: floatN): return a * b + c
    COPYSIGN,// (floatN, floatN)

    CROSS,         // (floatN, floatN)
    DOT,           // (floatN, floatN)
    LENGTH,        // (floatN)
    LENGTH_SQUARED,// (floatN)
    NORMALIZE,     // (floatN)
    FACEFORWARD,   // (floatN, floatN, floatN)
    REFLECT,       // (floatN, floatN)

    REDUCE_SUM,    // (floatN) -> float
    REDUCE_PRODUCT,// (floatN) -> float
    REDUCE_MIN,    // (floatN) -> float
    REDUCE_MAX,    // (floatN) -> float

    OUTER_PRODUCT,// (floatN, floatN) -> floatNxN | (floatNxN, floatNxN) -> floatNxN

    MATRIX_COMP_NEG,   // (floatNxN) -> floatNxN
    MATRIX_COMP_ADD,   // (floatNxN, floatNxN) -> floatNxN | (floatNxN, float) -> floatNxN | (float, floatNxN) -> floatNxN
    MATRIX_COMP_SUB,   // (floatNxN, floatNxN) -> floatNxN | (floatNxN, float) -> floatNxN | (float, floatNxN) -> floatNxN
    MATRIX_COMP_MUL,   // (floatNxN, floatNxN) -> floatNxN | (floatNxN, float) -> floatNxN | (float, floatNxN) -> floatNxN
    MATRIX_COMP_DIV,   // (floatNxN, floatNxN) -> floatNxN | (floatNxN, float) -> floatNxN | (float, floatNxN) -> floatNxN
    MATRIX_LINALG_MUL, // (floatNxN, floatNxN) -> floatNxN | (floatNxN, floatN) -> floatN
    MATRIX_DETERMINANT,// (floatNxN) -> float
    MATRIX_TRANSPOSE,  // (floatNxN) -> floatNxN
    MATRIX_INVERSE,    // (floatNxN) -> floatNxN

    // aggregate operations
    AGGREGATE,
    SHUFFLE,
    INSERT,
    EXTRACT,
};

[[nodiscard]] LC_XIR_API luisa::string_view to_string(ArithmeticOp op) noexcept;
[[nodiscard]] LC_XIR_API ArithmeticOp arithmetic_op_from_string(luisa::string_view name) noexcept;

class LC_XIR_API ArithmeticInst final : public DerivedInstruction<ArithmeticInst, DerivedInstructionTag::ARITHMETIC>,
                                        public InstructionOpMixin<ArithmeticOp> {
public:
    ArithmeticInst(BasicBlock *parent_block,
                   const Type *type, ArithmeticOp op,
                   luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] ArithmeticInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
