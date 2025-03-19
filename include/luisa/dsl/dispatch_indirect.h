#pragma once

#include <luisa/dsl/expr.h>
#include <luisa/dsl/var.h>
#include <luisa/dsl/struct.h>
#include <luisa/runtime/dispatch_buffer.h>

namespace luisa::compute {

/**
 * @brief Expression specialization for IndirectDispatchBuffer.
 *
 * Provides methods to manipulate an indirect dispatch buffer within the DSL.
 */
template<>
struct LC_DSL_API Expr<IndirectDispatchBuffer> {

private:
    //! The underlying RefExpr.
    const RefExpr *_expression{nullptr};

public:
    /**
     * @brief Constructs an Expr from a RefExpr.
     * @param expr The RefExpr to construct from.
     */
    explicit Expr(const RefExpr *expr) noexcept : _expression{expr} {}

    /**
     * @brief Constructs an Expr from an IndirectDispatchBuffer.
     * @param buffer The IndirectDispatchBuffer to construct from.
     */
    Expr(const IndirectDispatchBuffer &buffer) noexcept;

    /**
     * @brief Returns the underlying RefExpr.
     * @return The underlying RefExpr.
     */
    [[nodiscard]] auto expression() const noexcept { return _expression; }

    /**
     * @brief Sets the dispatch count for the indirect dispatch buffer.
     * @param count The dispatch count.
     */
    void set_dispatch_count(Expr<uint> count) const noexcept;

    /**
     * @brief Sets the kernel for the indirect dispatch buffer.
     * @param offset The offset into the dispatch buffer.
     * @param block_size The block size for the kernel.
     * @param dispatch_size The dispatch size for the kernel.
     * @param kernel_id The kernel ID.
     */
    void set_kernel(Expr<uint> offset, Expr<uint3> block_size, Expr<uint3> dispatch_size, Expr<uint> kernel_id) const noexcept;

    /**
     * @brief Sets the kernel for the indirect dispatch buffer.
     * @param offset The offset into the dispatch buffer.
     * @param block_size The block size for the kernel.
     * @param dispatch_size The dispatch size for the kernel.
     */
    void set_kernel(Expr<uint> offset, Expr<uint3> block_size, Expr<uint3> dispatch_size) const noexcept {
        set_kernel(offset, block_size, dispatch_size, 0u);
    }

    /**
     * @brief Returns a pointer to this Expr.
     * @return A pointer to this Expr.
     */
    [[nodiscard]] auto operator->() const noexcept { return this; }
};

Expr(const IndirectDispatchBuffer &) -> Expr<IndirectDispatchBuffer>;

/**
 * @brief Variable specialization for IndirectDispatchBuffer.
 *
 * Provides a variable that can hold an indirect dispatch buffer within the DSL.
 */
template<>
struct Var<IndirectDispatchBuffer> : public Expr<IndirectDispatchBuffer> {
    /**
     * @brief Constructs a Var from an ArgumentCreation tag.
     * @param tag The ArgumentCreation tag.
     */
    explicit Var(detail::ArgumentCreation) noexcept
        : Expr<IndirectDispatchBuffer>{
              detail::FunctionBuilder::current()->buffer(
                  Type::of<IndirectDispatchBuffer>())} {}
    Var(Var &&) noexcept = default;
    Var(const Var &) noexcept = delete;
    Var &operator=(Var &&) noexcept = delete;
    Var &operator=(const Var &) noexcept = delete;
};

using IndirectDispatchBufferVar = Var<IndirectDispatchBuffer>;

namespace detail {

/**
 * @brief Expression proxy for IndirectDispatchBuffer.
 *
 * Provides methods to manipulate an indirect dispatch buffer outside the DSL.
 */
class LC_DSL_API IndirectDispatchBufferExprProxy {

private:
    IndirectDispatchBuffer _buffer;

public:
    LUISA_RESOURCE_PROXY_AVOID_CONSTRUCTION(IndirectDispatchBufferExprProxy)

public:
    /**
     * @brief Sets the dispatch count for the indirect dispatch buffer.
     * @param count The dispatch count.
     */
    void set_dispatch_count(Expr<uint> count) const noexcept;

    /**
     * @brief Sets the kernel for the indirect dispatch buffer.
     * @param offset The offset into the dispatch buffer.
     * @param block_size The block size for the kernel.
     * @param dispatch_size The dispatch size for the kernel.
     * @param kernel_id The kernel ID.
     */
    void set_kernel(Expr<uint> offset, Expr<uint3> block_size, Expr<uint3> dispatch_size, Expr<uint> kernel_id) const noexcept;

    /**
     * @brief Sets the kernel for the indirect dispatch buffer.
     * @param offset The offset into the dispatch buffer.
     * @param block_size The block size for the kernel.
     * @param dispatch_size The dispatch size for the kernel.
     */
    void set_kernel(Expr<uint> offset, Expr<uint3> block_size, Expr<uint3> dispatch_size) const noexcept {
        set_kernel(offset, block_size, dispatch_size, 0u);
    }
};

}// namespace detail

}// namespace luisa::compute
