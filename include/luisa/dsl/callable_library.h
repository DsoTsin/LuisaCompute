#pragma once
#include <luisa/ast/callable_library.h>
#include <luisa/core/logging.h>
#include <luisa/dsl/func.h>

namespace luisa::compute {

namespace detail {

/**
 * @brief Callable type checker.
 *
 * This struct provides a method to check if the return type and arguments of a callable match the expected types.
 * @tparam T Callable signature type.
 */
template<typename T>
struct CallableTypeChecker;

/**
 * @brief Callable type checker specialization for function types.
 *
 * This struct provides a method to check if the return type and arguments of a callable match the expected types.
 * @tparam Ret Return type of the callable.
 * @tparam Args Argument types of the callable.
 */
template<typename Ret, typename... Args>
struct CallableTypeChecker<Ret(Args...)> {
    /**
     * @brief Checks if the return type and arguments of a callable match the expected types.
     *
     * @param ret_type The return type of the callable.
     * @param args The arguments of the callable.
     */
    static void check(Type const *ret_type, luisa::span<const Variable> args) {
        if (ret_type != Type::of<Ret>()) [[unlikely]] {
            LUISA_ERROR("Return type not match.");
        }
        auto types = {Type::of<Args>()...};
        size_t arg_count = 0;
        for (auto &&i : args) {
            if (!i.is_shared() && !i.is_builtin()) {
                arg_count += 1;
            }
        }
        if (types.size() != arg_count) [[unlikely]] {
            LUISA_ERROR("Argument size not match, required: {}, contained: {}", types.size(), args.size());
        }
        auto arg = args.begin();
        for (size_t i = 0; i < types.size(); ++i) {
            while (arg->is_shared() || arg->is_builtin()) {
                arg++;
            }
            if (types.begin()[i] != arg->type()) [[unlikely]] {
                LUISA_ERROR("Argument {} type mismatch, required: {}, contained: {}", i, types.begin()[i]->description(), arg->type()->description());
            }
            arg++;
        }
    }
};
}// namespace detail

/**
 * @brief Gets a callable from the callable library.
 * @tparam T Callable signature type.
 * @param name The name of the callable.
 * @return The callable with the given name.
 */
template<typename T>
Callable<T> CallableLibrary::get_callable(luisa::string_view name) const noexcept {
    auto iter = _callables.find(name);
    if (iter == _callables.end()) [[unlikely]] {
        LUISA_ERROR("Callable {} not found", name);
    }
    auto &func = iter->second;
    detail::CallableTypeChecker<T>::check(func->return_type(), func->arguments());
    return Callable<T>{func};
}

/**
 * @brief Gets a kernel from the callable library.
 * @tparam dim Kernel dimension.
 * @tparam T Kernel argument types.
 * @param name The name of the kernel.
 * @return The kernel with the given name.
 */
template<size_t dim, typename... T>
Kernel<dim, T...> CallableLibrary::get_kernel(luisa::string_view name) const noexcept {
    auto iter = _callables.find(name);
    if (iter == _callables.end()) [[unlikely]] {
        LUISA_ERROR("Callable {} not found", name);
    }
    auto &func = iter->second;
    detail::CallableTypeChecker<void(T...)>::check(func->return_type(), func->arguments());
    return Kernel<dim, T...>{func};
}
}// namespace luisa::compute
