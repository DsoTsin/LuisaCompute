//
// Created by Mike Smith on 2021/3/2.
//
#pragma once

#include <luisa/ast/constant_data.h>
#include <luisa/dsl/expr.h>

namespace luisa::compute {

/**
 * @brief Represents a constant value in the compute graph.
 *
 * Constants are immutable values that are known at compile time. They can be
 * used to store data that is accessed by kernels and callables.
 *
 * @tparam T The type of the constant value.
 */
template<typename T>
class Constant {

private:
    //! The underlying constant data.
    ConstantData _data;

public:
    /**
     * @brief Default constructor for serialization.
     *
     * This constructor is used internally for serialization purposes and
     * should not be used directly.
     */
    Constant() noexcept = default;

    /**
     * @brief Constructs a constant from a span of data.
     *
     * This constructor creates a constant from a contiguous range of values.
     *
     * @param data A span containing the constant data.
     */
    Constant(luisa::span<const T> data) noexcept
        : _data{ConstantData::create(Type::array(Type::of<T>(), data.size()),
                                     data.data(), data.size_bytes())} {}

    /**
     * @brief Constructs a constant from an array.
     *
     * This constructor creates a constant from an array of values.
     *
     * @param data A pointer to the array data.
     * @param size The size of the array.
     */
    Constant(const T *data, size_t size) noexcept
        : Constant{luisa::span{data, size}} {}

    /**
     * @brief Constructs a constant from array-like data.
     *
     * This constructor creates a constant from an object that can be implicitly
     * converted to a span of constant data.
     *
     * @tparam U The type of the array-like data.
     * @param data The array-like data.
     */
    template<typename U>
    Constant(U &&data) noexcept
        : Constant{luisa::span<const T>{std::forward<U>(data)}} {}

    /**
     * @brief Constructs a constant from an initializer list.
     *
     * This constructor creates a constant from an initializer list of values.
     *
     * @param init An initializer list containing the constant data.
     */
    Constant(std::initializer_list<T> init) noexcept
        : Constant{luisa::span<const T>{init.begin(), init.end()}} {}

    Constant(Constant &&) noexcept = default;
    Constant(const Constant &) noexcept = delete;
    Constant &operator=(Constant &&) noexcept = delete;
    Constant &operator=(const Constant &) noexcept = delete;

    /**
     * @brief Accesses a member of the constant by index.
     *
     * This method allows accessing individual elements of the constant data
     * using an index expression.
     *
     * @tparam U The type of the index expression.
     * @param index The index expression used to access the element.
     * @return An expression representing the accessed element.
     */
    template<typename U>
        requires is_integral_expr_v<U>
    [[nodiscard]] auto operator[](U &&index) const noexcept {
        return def<T>(detail::FunctionBuilder::current()->access(
            Type::of<T>(),
            detail::FunctionBuilder::current()->constant(_data),
            detail::extract_expression(std::forward<U>(index))));
    }

    /**
     * @brief Reads an element at the specified index.
     *
     * This method is equivalent to the `operator[]` and provides a more
     * descriptive name for accessing elements.
     *
     * @tparam I The type of the index expression.
     * @param index The index expression used to access the element.
     * @return An expression representing the accessed element.
     */
    template<typename I>
    [[nodiscard]] auto read(I &&index) const noexcept {
        return (*this)[std::forward<I>(index)];
    }

    [[nodiscard]] auto operator->() const noexcept { return this; }
};

template<typename T>
Constant(luisa::span<T> data) -> Constant<T>;

template<typename T>
Constant(luisa::span<const T> data) -> Constant<T>;

template<typename T>
Constant(std::initializer_list<T>) -> Constant<T>;

template<concepts::container T>
Constant(T &&) -> Constant<std::remove_const_t<typename std::remove_cvref_t<T>::value_type>>;

}// namespace luisa::compute
