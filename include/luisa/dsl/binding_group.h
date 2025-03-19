#pragma once

#include <luisa/dsl/struct.h>

/**
 * @brief Macro to declare a member variable in a binding group.
 *
 * This macro is used to declare a member variable of type `member_type_##m`
 * with the name `m` within a binding group.
 */
#define LUISA_BINDING_GROUP_MAKE_MEMBER_VAR_DECL(m) \
    Var<member_type_##m> m;

/**
 * @brief Macro to declare a member expression in a binding group.
 *
 * This macro is used to declare a member expression of type `member_type_##m`
 * with the name `m` within a binding group.
 */
#define LUISA_BINDING_GROUP_MAKE_MEMBER_EXPR_DECL(m) \
    Expr<member_type_##m> m;

/**
 * @brief Macro to initialize a member variable in a binding group.
 *
 * This macro is used to initialize a member variable `m` within a binding group
 * using the `ArgumentCreation` tag.
 */
#define LUISA_BINDING_GROUP_MAKE_MEMBER_VAR_INIT(m) \
    m(detail::ArgumentCreation{})

/**
 * @brief Macro to initialize a member expression in a binding group.
 *
 * This macro is used to initialize a member expression `m` within a binding
 * group using the corresponding member `s.m` from a variable `s`.
 */
#define LUISA_BINDING_GROUP_MAKE_MEMBER_EXPR_INIT(m) \
    m(s.m)

/**
 * @brief Macro to invoke a member in a binding group.
 *
 * This macro is used to invoke a member `s.m` within a binding group and
 * append it to a `CallableInvoke` object.
 */
#define LUISA_BINDING_GROUP_MAKE_INVOKE(m) \
    invoke << s.m;

/**
 * @brief Macro to calculate the encode count for a member in a binding group.
 *
 * This macro is used to calculate the encode count for a member of type
 * `member_type_##m` within a binding group using the
 * `shader_argument_encode_count` trait.
 */
#define LUISA_BINDING_GROUP_MAKE_MEMBER_ENCODE_COUNT(m) \
    (shader_argument_encode_count<member_type_##m>::value) +

/**
 * @brief Template implementation for defining a binding group.
 *
 * This macro defines a binding group `S` with the specified members `__VA_ARGS__`.
 * It uses the provided templates `TEMPLATE` and `TEMPLATE2` to handle different
 * aspects of the binding group definition.
 *
 * @param TEMPLATE Template to be evaluated at the beginning and end of the struct definition.
 * @param TEMPLATE2 Template to be evaluated within the detail namespace.
 * @param S The name of the binding group struct.
 * @param ... The members of the binding group, specified as (member_name, member_type) pairs.
 */
#define LUISA_BINDING_GROUP_TEMPLATE_IMPL(TEMPLATE, TEMPLATE2, S, ...)                  \
    LUISA_MACRO_EVAL(TEMPLATE())                                                        \
    struct luisa_compute_extension<S>;                                                  \
    namespace luisa::compute {                                                          \
    LUISA_MACRO_EVAL(TEMPLATE())                                                        \
    struct Var<S> {                                                                     \
        using is_binding_group = void;                                                  \
        using this_type = S;                                                            \
        LUISA_MAP(LUISA_STRUCT_MAKE_MEMBER_TYPE, __VA_ARGS__)                           \
        LUISA_MAP(LUISA_BINDING_GROUP_MAKE_MEMBER_VAR_DECL, __VA_ARGS__)                \
        explicit Var(detail::ArgumentCreation) noexcept                                 \
            : LUISA_MAP_LIST(LUISA_BINDING_GROUP_MAKE_MEMBER_VAR_INIT, __VA_ARGS__) {}  \
        Var(Var &&) noexcept = default;                                                 \
        Var(const Var &) noexcept = delete;                                             \
        Var &operator=(Var &&) noexcept = delete;                                       \
        Var &operator=(const Var &) noexcept = delete;                                  \
        [[nodiscard]] auto operator->() noexcept {                                      \
            return reinterpret_cast<luisa_compute_extension<this_type> *>(this);        \
        }                                                                               \
        [[nodiscard]] auto operator->() const noexcept {                                      \
            return reinterpret_cast<const luisa_compute_extension<this_type> *>(this);  \
        }                                                                               \
    };                                                                                  \
    LUISA_MACRO_EVAL(TEMPLATE())                                                        \
    struct Expr<S> {                                                                    \
        using is_binding_group = void;                                                  \
        using this_type = S;                                                            \
        LUISA_MAP(LUISA_STRUCT_MAKE_MEMBER_TYPE, __VA_ARGS__)                           \
        LUISA_MAP(LUISA_BINDING_GROUP_MAKE_MEMBER_EXPR_DECL, __VA_ARGS__)               \
        Expr(const Var<S> &s) noexcept                                                  \
            : LUISA_MAP_LIST(LUISA_BINDING_GROUP_MAKE_MEMBER_EXPR_INIT, __VA_ARGS__) {} \
        Expr(Expr &&another) noexcept = default;                                        \
        Expr(const Expr &another) noexcept = default;                                   \
        Expr &operator=(Expr) noexcept = delete;                                        \
    };                                                                                  \
    namespace detail {                                                                  \
    LUISA_MACRO_EVAL(TEMPLATE())                                                        \
    struct shader_argument_encode_count<S> {                                            \
        using this_type = S;                                                            \
        LUISA_MAP(LUISA_STRUCT_MAKE_MEMBER_TYPE, __VA_ARGS__)                           \
        static constexpr uint value =                                                   \
            LUISA_MAP(LUISA_BINDING_GROUP_MAKE_MEMBER_ENCODE_COUNT, __VA_ARGS__)        \
        0u;                                                                             \
    };                                                                                  \
    LUISA_MACRO_EVAL(TEMPLATE2())                                                       \
    void callable_encode_binding_group(CallableInvoke &invoke, Expr<S> s) noexcept {    \
        LUISA_MAP(LUISA_BINDING_GROUP_MAKE_INVOKE, __VA_ARGS__)                         \
    }                                                                                   \
    LUISA_MACRO_EVAL(TEMPLATE2())                                                       \
    ShaderInvokeBase &operator<<(ShaderInvokeBase &invoke, const S &s) noexcept {       \
        LUISA_MAP(LUISA_BINDING_GROUP_MAKE_INVOKE, __VA_ARGS__)                         \
        return invoke;                                                                  \
    }                                                                                   \
    }                                                                                   \
    }                                                                                   \
    LUISA_MACRO_EVAL(TEMPLATE())                                                        \
    struct luisa_compute_extension<S> final : luisa::compute::Var<S>

/**
 * @brief Macro to define an empty template.
 *
 * This macro is used to define an empty template for binding groups.
 */
#define LUISA_BIND_GROUP_IMPL_EMPTY_TEMPLATE() template<>

/**
 * @brief Macro to define an empty implementation.
 *
 * This macro is used to define an empty implementation for binding groups.
 */
#define LUISA_BIND_GROUP_IMPL_EMPTY()

/**
 * @brief Macro to define a binding group.
 *
 * This macro defines a binding group `S` with the specified members `__VA_ARGS__`.
 *
 * @param S The name of the binding group struct.
 * @param ... The members of the binding group, specified as (member_name, member_type) pairs.
 */
#define LUISA_BINDING_GROUP(S, ...)           \
    LUISA_BINDING_GROUP_TEMPLATE_IMPL(        \
        LUISA_BIND_GROUP_IMPL_EMPTY_TEMPLATE, \
        LUISA_BIND_GROUP_IMPL_EMPTY,          \
        S, __VA_ARGS__)

/**
 * @brief Macro to define a templated binding group.
 *
 * This macro defines a templated binding group `S` with the specified members `__VA_ARGS__`.
 *
 * @param TEMPLATE The template declaration for the binding group.
 * @param S The name of the binding group struct.
 * @param ... The members of the binding group, specified as (member_name, member_type) pairs.
 */
#define LUISA_BINDING_GROUP_TEMPLATE(TEMPLATE, S, ...) \
    LUISA_BINDING_GROUP_TEMPLATE_IMPL(                 \
        TEMPLATE, TEMPLATE, S, __VA_ARGS__)

