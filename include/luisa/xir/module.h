#pragma once

#include <luisa/core/stl/memory.h>
#include <luisa/core/stl/unordered_map.h>
#include <luisa/xir/constant.h>
#include <luisa/xir/undefined.h>
#include <luisa/xir/special_register.h>
#include <luisa/xir/function.h>

namespace luisa::compute {
class Type;
}// namespace luisa::compute

namespace luisa::compute::xir {

class Constant;

/**
 * @class Module
 * @brief Top-level container for XIR functions and global values.
 * 
 * The Module class represents a complete unit of XIR code, containing functions,
 * constants, undefined values, and special registers. It serves as the main
 * container for all XIR entities and provides methods for creating and managing
 * these entities.
 */
class LC_XIR_API Module final : public MetadataListMixin<PoolOwner> {

private:
    FunctionList _function_list;
    ConstantList _constant_list;
    UndefinedList _undefined_list;
    SpecialRegisterList _special_register_list;

    // maps for uniquifying
    luisa::unordered_map<uint64_t, Constant *> _hash_to_constant;
    luisa::unordered_map<const Type *, Undefined *> _type_to_undefined;
    luisa::unordered_map<DerivedSpecialRegisterTag, SpecialRegister *> _tag_to_special_register;

private:
    /**
     * @brief Gets or creates a constant with the given value.
     * 
     * @param temp The template constant to get or create
     * @return A pointer to the constant
     */
    [[nodiscard]] Constant *_get_or_create_constant(const Constant &temp) noexcept;

public:
    /**
     * @brief Constructs a module with the given initial pool capacity.
     * 
     * @param init_pool_cap Initial capacity for the memory pool
     */
    explicit Module(size_t init_pool_cap = 0u) noexcept : Super{init_pool_cap} {}

    /**
     * @brief Creates a new kernel function in this module.
     * 
     * @return A pointer to the created kernel function
     */
    [[nodiscard]] KernelFunction *create_kernel() noexcept;
    
    /**
     * @brief Creates a new callable function with the given return type.
     * 
     * @param ret_type The return type of the function
     * @return A pointer to the created callable function
     */
    [[nodiscard]] CallableFunction *create_callable(const Type *ret_type) noexcept;
    
    /**
     * @brief Creates a new external function with the given return type.
     * 
     * @param ret_type The return type of the function
     * @return A pointer to the created external function
     */
    [[nodiscard]] ExternalFunction *create_external_function(const Type *ret_type) noexcept;
    
    /**
     * @brief Gets the list of functions in this module.
     * 
     * @return A reference to the function list
     */
    [[nodiscard]] auto &function_list() noexcept { return _function_list; }
    
    /**
     * @brief Gets the list of functions in this module (const version).
     * 
     * @return A const reference to the function list
     */
    [[nodiscard]] auto &function_list() const noexcept { return _function_list; }

    /**
     * @brief Creates a new constant with the given type and data.
     * 
     * @param type The type of the constant
     * @param data Pointer to the constant data (optional)
     * @return A pointer to the created constant
     */
    [[nodiscard]] Constant *create_constant(const Type *type, const void *data = nullptr) noexcept;
    
    /**
     * @brief Creates a new zero constant with the given type.
     * 
     * @param type The type of the constant
     * @return A pointer to the created zero constant
     */
    [[nodiscard]] Constant *create_constant_zero(const Type *type) noexcept;
    
    /**
     * @brief Creates a new one constant with the given type.
     * 
     * @param type The type of the constant
     * @return A pointer to the created one constant
     */
    [[nodiscard]] Constant *create_constant_one(const Type *type) noexcept;
    
    /**
     * @brief Gets the list of constants in this module.
     * 
     * @return A reference to the constant list
     */
    [[nodiscard]] auto &constant_list() noexcept { return _constant_list; }
    
    /**
     * @brief Gets the list of constants in this module (const version).
     * 
     * @return A const reference to the constant list
     */
    [[nodiscard]] auto &constant_list() const noexcept { return _constant_list; }

    /**
     * @brief Creates a new undefined value with the given type.
     * 
     * @param type The type of the undefined value
     * @return A pointer to the created undefined value
     */
    [[nodiscard]] Undefined *create_undefined(const Type *type) noexcept;
    
    /**
     * @brief Gets the list of undefined values in this module.
     * 
     * @return A reference to the undefined list
     */
    [[nodiscard]] auto &undefined_list() noexcept { return _undefined_list; }
    
    /**
     * @brief Gets the list of undefined values in this module (const version).
     * 
     * @return A const reference to the undefined list
     */
    [[nodiscard]] auto &undefined_list() const noexcept { return _undefined_list; }

    /**
     * @brief Creates a new special register with the given tag.
     * 
     * @param tag The tag identifying the special register type
     * @return A pointer to the created special register
     */
    [[nodiscard]] SpecialRegister *create_special_register(DerivedSpecialRegisterTag tag) noexcept;
    
    /**
     * @brief Gets the list of special registers in this module.
     * 
     * @return A reference to the special register list
     */
    [[nodiscard]] auto &special_register_list() noexcept { return _special_register_list; }
    
    /**
     * @brief Gets the list of special registers in this module (const version).
     * 
     * @return A const reference to the special register list
     */
    [[nodiscard]] auto &special_register_list() const noexcept { return _special_register_list; }

    /**
     * @brief Creates a thread ID special register.
     * 
     * @return A pointer to the created thread ID register
     */
    [[nodiscard]] SPR_ThreadID *create_thread_id() noexcept;
    
    /**
     * @brief Creates a block ID special register.
     * 
     * @return A pointer to the created block ID register
     */
    [[nodiscard]] SPR_BlockID *create_block_id() noexcept;
    
    /**
     * @brief Creates a warp lane ID special register.
     * 
     * @return A pointer to the created warp lane ID register
     */
    [[nodiscard]] SPR_WarpLaneID *create_warp_lane_id() noexcept;
    
    /**
     * @brief Creates a dispatch ID special register.
     * 
     * @return A pointer to the created dispatch ID register
     */
    [[nodiscard]] SPR_DispatchID *create_dispatch_id() noexcept;
    
    /**
     * @brief Creates a kernel ID special register.
     * 
     * @return A pointer to the created kernel ID register
     */
    [[nodiscard]] SPR_KernelID *create_kernel_id() noexcept;
    
    /**
     * @brief Creates an object ID special register.
     * 
     * @return A pointer to the created object ID register
     */
    [[nodiscard]] SPR_ObjectID *create_object_id() noexcept;
    
    /**
     * @brief Creates a block size special register.
     * 
     * @return A pointer to the created block size register
     */
    [[nodiscard]] SPR_BlockSize *create_block_size() noexcept;
    
    /**
     * @brief Creates a warp size special register.
     * 
     * @return A pointer to the created warp size register
     */
    [[nodiscard]] SPR_WarpSize *create_warp_size() noexcept;
    
    /**
     * @brief Creates a dispatch size special register.
     * 
     * @return A pointer to the created dispatch size register
     */
    [[nodiscard]] SPR_DispatchSize *create_dispatch_size() noexcept;
};

}// namespace luisa::compute::xir
