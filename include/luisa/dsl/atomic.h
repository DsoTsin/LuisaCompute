#pragma once

#include <luisa/core/dll_export.h>
#include <luisa/ast/atomic_ref_node.h>
#include <luisa/dsl/expr.h>

namespace luisa::compute::detail {



/**
 * @brief Forward declaration of the AtomicRef class.
 *
 * The AtomicRef class template provides a way to perform atomic operations
 * on a reference to a variable.
 *
 * @tparam T The type of the variable being referenced.
 */
template<typename>
class AtomicRef;

/**
 * @brief Base class for AtomicRef, providing common functionality.
 *
 * This class manages the underlying AtomicRefNode and provides basic
 * access and manipulation methods for atomic references.
 */
class AtomicRefBase {

private:
    //! Pointer to the underlying atomic reference node in the AST.
    const AtomicRefNode *_access_chain{nullptr};

protected:
    /**
     * @brief Constructs an AtomicRefBase from an AtomicRefNode.
     *
     * This constructor is used internally to create AtomicRefBase instances
     * from the AST representation of atomic references.
     *
     * @param access_chain Pointer to the AtomicRefNode.
     */
    explicit AtomicRefBase(const AtomicRefNode *access_chain) noexcept
        : _access_chain{access_chain} {}

public:
    AtomicRefBase(AtomicRefBase &&) noexcept = default;
    AtomicRefBase(const AtomicRefBase &) noexcept = delete;
    AtomicRefBase &operator=(AtomicRefBase &&) noexcept = delete;
    AtomicRefBase &operator=(const AtomicRefBase &) noexcept = delete;

protected:
    /**
     * @brief Returns the underlying AtomicRefNode.
     *
     * @return A pointer to the AtomicRefNode.
     */
    [[nodiscard]] auto access_chain() const noexcept { return _access_chain; }

    /**
     * @brief Accesses an element of the atomic reference by index.
     *
     * This method is used to access elements within an atomic reference,
     * such as elements of an array or vector.
     *
     * @tparam T The type of the element being accessed.
     * @tparam I The type of the index expression.
     * @param index The index expression used to access the element.
     * @return An AtomicRef to the specified element.
     */
    template<typename T, typename I>
    [[nodiscard]] auto access(I &&index) const noexcept {
        auto i = def(std::forward<I>(index));
        return AtomicRef<T>{_access_chain->access(i.expression())};
    }

    /**
     * @brief Accesses a member of the atomic reference by index.
     *
     * This method is used to access members within an atomic reference,
     * such as members of a struct or tuple.
     *
     * @tparam T The type of the member being accessed.
     * @param i The index of the member to access.
     * @return An AtomicRef to the specified member.
     */
    template<typename T>
    [[nodiscard]] auto member(size_t i) const noexcept {
        return AtomicRef<T>{_access_chain->access(i)};
    }
};

/**
 * @brief Common macro for defining AtomicRef constructors.
 */
#define LUISA_ATOMIC_REF_COMMON()                                  \
public:                                                            \
    explicit AtomicRef(const AtomicRefNode *access_chain) noexcept \
        : AtomicRefBase{access_chain} {}

/**
 * @brief Class template for performing atomic operations on variables.
 *
 * This class provides a type-safe way to perform atomic operations on
 * variables of type T. It supports various atomic operations such as
 * exchange, compare exchange, fetch add, fetch sub, fetch and, fetch or,
 * fetch xor, fetch min, and fetch max.
 *
 * @tparam T The type of the variable being referenced.
 */
template<typename T>
class AtomicRef : private AtomicRefBase {
public:
    LUISA_ATOMIC_REF_COMMON()
};

/**
 * @brief Specialization of AtomicRef for integer types (int and uint).
 *
 * This specialization provides atomic operations specific to integer types,
 * such as fetch add, fetch sub, fetch and, fetch or, fetch xor, fetch min,
 * and fetch max.
 *
 * @tparam T The integer type being referenced (int or uint).
 */
template<typename T>
    requires std::same_as<T, int> || std::same_as<T, uint>
class AtomicRef<T> : private AtomicRefBase {

public:
    LUISA_ATOMIC_REF_COMMON()

public:
    /**
     * @brief Atomically exchanges the value of the referenced variable.
     *
     * Stores `desired` in the referenced variable and returns the original value.
     *
     * @param desired The new value to store.
     * @return The original value of the referenced variable.
     */
    auto exchange(Expr<T> desired) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_EXCHANGE,
            {desired.expression()}));
    }

    /**
     * @brief Atomically compares the value of the referenced variable with
     *        `expected` and, if they are equal, replaces the value with
     *        `desired`.
     *
     * Stores `desired` in the referenced variable if its current value equals
     * `expected`; otherwise, it leaves the referenced variable unchanged.
     * Returns the original value of the referenced variable.
     *
     * @param expected The value to compare with the current value.
     * @param desired The new value to store if the current value equals `expected`.
     * @return The original value of the referenced variable.
     */
    auto compare_exchange(Expr<T> expected, Expr<T> desired) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_COMPARE_EXCHANGE,
            {expected.expression(), desired.expression()}));
    }

    /**
     * @brief Atomically adds a value to the referenced variable.
     *
     * Stores the sum of the current value and `val` in the referenced
     * variable and returns the original value.
     *
     * @param val The value to add.
     * @return The original value of the referenced variable.
     */
    auto fetch_add(Expr<T> val) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_FETCH_ADD,
            {val.expression()}));
    };

    /**
     * @brief Atomically subtracts a value from the referenced variable.
     *
     * Stores the difference between the current value and `val` in the
     * referenced variable and returns the original value.
     *
     * @param val The value to subtract.
     * @return The original value of the referenced variable.
     */
    auto fetch_sub(Expr<T> val) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_FETCH_SUB,
            {val.expression()}));
    };

    /**
     * @brief Atomically performs a bitwise AND operation on the referenced variable.
     *
     * Stores the result of the bitwise AND between the current value and `val`
     * in the referenced variable and returns the original value.
     *
     * @param val The value to perform the bitwise AND with.
     * @return The original value of the referenced variable.
     */
    auto fetch_and(Expr<T> val) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_FETCH_AND,
            {val.expression()}));
    };

    /**
     * @brief Atomically performs a bitwise OR operation on the referenced variable.
     *
     * Stores the result of the bitwise OR between the current value and `val`
     * in the referenced variable and returns the original value.
     *
     * @param val The value to perform the bitwise OR with.
     * @return The original value of the referenced variable.
     */
    auto fetch_or(Expr<T> val) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_FETCH_OR,
            {val.expression()}));
    };

    /**
     * @brief Atomically performs a bitwise XOR operation on the referenced variable.
     *
     * Stores the result of the bitwise XOR between the current value and `val`
     * in the referenced variable and returns the original value.
     *
     * @param val The value to perform the bitwise XOR with.
     * @return The original value of the referenced variable.
     */
    auto fetch_xor(Expr<T> val) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_FETCH_XOR,
            {val.expression()}));
    };

    /**
     * @brief Atomically stores the minimum of the current value and `val`
     *        in the referenced variable.
     *
     * Stores the smaller of the current value and `val` in the referenced
     * variable and returns the original value.
     *
     * @param val The value to compare with the current value.
     * @return The original value of the referenced variable.
     */
    auto fetch_min(Expr<T> val) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_FETCH_MIN,
            {val.expression()}));
    };

    /**
     * @brief Atomically stores the maximum of the current value and `val`
     *        in the referenced variable.
     *
     * Stores the larger of the current value and `val` in the referenced
     * variable and returns the original value.
     *
     * @param val The value to compare with the current value.
     * @return The original value of the referenced variable.
     */
    auto fetch_max(Expr<T> val) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_FETCH_MAX,
            {val.expression()}));
    };
};

/**
 * @brief Specialization of AtomicRef for floating-point types (float).
 *
 * This specialization provides atomic operations specific to floating-point
 * types, such as exchange, compare exchange, fetch add, fetch sub,
 * fetch min, and fetch max.
 *
 * @tparam T The floating-point type being referenced (float).
 */
template<typename T>
    requires std::same_as<T, float>
class AtomicRef<T> : private AtomicRefBase {

public:
    LUISA_ATOMIC_REF_COMMON()

public:
    /**
     * @brief Atomically exchanges the value of the referenced variable.
     *
     * Stores `desired` in the referenced variable and returns the original value.
     *
     * @param desired The new value to store.
     * @return The original value of the referenced variable.
     */
    auto exchange(Expr<T> desired) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_EXCHANGE,
            {desired.expression()}));
    }

    /**
     * @brief Atomically compares the value of the referenced variable with
     *        `expected` and, if they are equal, replaces the value with
     *        `desired`.
     *
     * Stores `desired` in the referenced variable if its current value equals
     * `expected`; otherwise, it leaves the referenced variable unchanged.
     * Returns the original value of the referenced variable.
     *
     * @param expected The value to compare with the current value.
     * @param desired The new value to store if the current value equals `expected`.
     * @return The original value of the referenced variable.
     */
    auto compare_exchange(Expr<T> expected, Expr<T> desired) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_COMPARE_EXCHANGE,
            {expected.expression(), desired.expression()}));
    }

    /**
     * @brief Atomically adds a value to the referenced variable.
     *
     * Stores the sum of the current value and `val` in the referenced
     * variable and returns the original value.
     *
     * @param val The value to add.
     * @return The original value of the referenced variable.
     */
    auto fetch_add(Expr<T> val) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_FETCH_ADD,
            {val.expression()}));
    };

    /**
     * @brief Atomically subtracts a value from the referenced variable.
     *
     * Stores the difference between the current value and `val` in the
     * referenced variable and returns the original value.
     *
     * @param val The value to subtract.
     * @return The original value of the referenced variable.
     */
    auto fetch_sub(Expr<T> val) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_FETCH_SUB,
            {val.expression()}));
    };

    /**
     * @brief Atomically stores the minimum of the current value and `val`
     *        in the referenced variable.
     *
     * Stores the smaller of the current value and `val` in the referenced
     * variable and returns the original value.
     *
     * @param val The value to compare with the current value.
     * @return The original value of the referenced variable.
     */
    auto fetch_min(Expr<T> val) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_FETCH_MIN,
            {val.expression()}));
    };

    /**
     * @brief Atomically stores the maximum of the current value and `val`
     *        in the referenced variable.
     *
     * Stores the larger of the current value and `val` in the referenced
     * variable and returns the original value.
     *
     * @param val The value to compare with the current value.
     * @return The original value of the referenced variable.
     */
    auto fetch_max(Expr<T> val) noexcept {
        return def<T>(access_chain()->operate(
            CallOp::ATOMIC_FETCH_MAX,
            {val.expression()}));
    };
};

/**
 * @brief Specializations of AtomicRef for built-in aggregate types.
 */

// arrays
template<typename T, size_t N>
class AtomicRef<std::array<T, N>> : private AtomicRefBase {
public:
    LUISA_ATOMIC_REF_COMMON()
    /**
     * @brief Accesses an element of the atomic reference by index.
     *
     * @param index The index expression used to access the element.
     * @return An AtomicRef to the specified element.
     */
    template<typename I>
    [[nodiscard]] auto operator[](I &&index) const noexcept {
        return this->access<T>(std::forward<I>(index));
    }
};

template<typename T, size_t N>
class AtomicRef<std::array<T, N> &> : private AtomicRefBase {
public:
    LUISA_ATOMIC_REF_COMMON()
    /**
     * @brief Accesses an element of the atomic reference by index.
     *
     * @param index The index expression used to access the element.
     * @return An AtomicRef to the specified element.
     */
    template<typename I>
    [[nodiscard]] auto operator[](I &&index) const noexcept {
        return this->access<T>(std::forward<I>(index));
    }
};

// vectors
template<typename T>
class AtomicRef<Vector<T, 2>> : private AtomicRefBase {

public:
    AtomicRef<T> x{this->member<T>(0u)};
    AtomicRef<T> y{this->member<T>(1u)};

public:
    LUISA_ATOMIC_REF_COMMON()
    /**
     * @brief Accesses an element of the atomic reference by index.
     *
     * @param index The index expression used to access the element.
     * @return An AtomicRef to the specified element.
     */
    template<typename I>
    [[nodiscard]] auto operator[](I &&index) const noexcept {
        return this->access<T>(std::forward<I>(index));
    }
};

template<typename T>
class AtomicRef<Vector<T, 3>> : private AtomicRefBase {

public:
    AtomicRef<T> x{this->member<T>(0u)};
    AtomicRef<T> y{this->member<T>(1u)};
    AtomicRef<T> z{this->member<T>(2u)};

public:
    LUISA_ATOMIC_REF_COMMON()
    /**
     * @brief Accesses an element of the atomic reference by index.
     *
     * @param index The index expression used to access the element.
     * @return An AtomicRef to the specified element.
     */
    template<typename I>
    [[nodiscard]] auto operator[](I &&index) const noexcept {
        return this->access<T>(std::forward<I>(index));
    }
};

template<typename T>
class AtomicRef<Vector<T, 4>> : private AtomicRefBase {

public:
    AtomicRef<T> x{this->member<T>(0u)};
    AtomicRef<T> y{this->member<T>(1u)};
    AtomicRef<T> z{this->member<T>(2u)};
    AtomicRef<T> w{this->member<T>(3u)};

public:
    LUISA_ATOMIC_REF_COMMON()
    /**
     * @brief Accesses an element of the atomic reference by index.
     *
     * @param index The index expression used to access the element.
     * @return An AtomicRef to the specified element.
     */
    template<typename I>
    [[nodiscard]] auto operator[](I &&index) const noexcept {
        return this->access<T>(std::forward<I>(index));
    }
};

// matrices
template<size_t N>
class AtomicRef<Matrix<N>> : private AtomicRefBase {
public:
    LUISA_ATOMIC_REF_COMMON()
    /**
     * @brief Accesses an element of the atomic reference by index.
     *
     * @param index The index expression used to access the element.
     * @return An AtomicRef to the specified element.
     */
    template<typename I>
    [[nodiscard]] auto operator[](I &&index) const noexcept {
        return this->access<Vector<float, N>>(std::forward<I>(index));
    }
};

// tuples
template<typename... Ts>
class AtomicRef<std::tuple<Ts...>> : private AtomicRefBase {
public:
    LUISA_ATOMIC_REF_COMMON()
    /**
     * @brief Gets the element at the specified index in the tuple.
     *
     * @tparam i The index of the element to access.
     * @return An AtomicRef to the specified element.
     */
    template<size_t i>
    [[nodiscard]] auto get() const noexcept {
        static_assert(i < sizeof...(Ts));
        using T = std::tuple_element_t<i, std::tuple<Ts...>>;
        return this->member<T>(i);
    }
};

#undef LUISA_ATOMIC_REF_COMMON

}// namespace luisa::compute::detail
