#pragma once

#include <luisa/xir/argument.h>
#include <luisa/xir/basic_block.h>

namespace luisa::compute::xir {

/**
 * @enum DerivedFunctionTag
 * @brief Identifies the specific type of function in the XIR system.
 */
enum struct DerivedFunctionTag {
    KERNEL,   /**< Kernel function that can be executed on a device */
    CALLABLE, /**< Callable function that can be called from other functions */
    EXTERNAL, /**< External function declaration without a definition */
};

class Module;
class FunctionDefinition;

/**
 * @class Function
 * @brief Base class for all XIR functions.
 * 
 * Function is the base class for all function types in the XIR system.
 * It provides common functionality for managing arguments and basic blocks.
 */
class LC_XIR_API Function : public IntrusiveForwardNode<Function, DerivedGlobalValue<Function, DerivedValueTag::FUNCTION>> {

private:
    /** @brief The module containing this function */
    Module *_module;
    
    /** @brief The arguments of this function */
    luisa::vector<Argument *> _arguments;

public:
    /**
     * @brief Constructs a function within the specified module.
     * 
     * @param parent_module The module containing this function
     * @param type The return type of the function
     */
    explicit Function(Module *parent_module, const Type *type = nullptr) noexcept;
    
    /**
     * @brief Gets the specific function tag for this function.
     * 
     * @return The derived function tag
     */
    [[nodiscard]] virtual DerivedFunctionTag derived_function_tag() const noexcept = 0;

    /**
     * @brief Adds an argument to this function.
     * 
     * @param argument The argument to add
     */
    void add_argument(Argument *argument) noexcept;
    
    /**
     * @brief Inserts an argument at the specified index.
     * 
     * @param index The index to insert at
     * @param argument The argument to insert
     */
    void insert_argument(size_t index, Argument *argument) noexcept;
    
    /**
     * @brief Removes an argument from this function.
     * 
     * @param argument The argument to remove
     */
    void remove_argument(Argument *argument) noexcept;
    
    /**
     * @brief Removes the argument at the specified index.
     * 
     * @param index The index of the argument to remove
     */
    void remove_argument(size_t index) noexcept;
    
    /**
     * @brief Replaces an argument with another one.
     * 
     * @param old_argument The argument to replace
     * @param new_argument The replacement argument
     */
    void replace_argument(Argument *old_argument, Argument *new_argument) noexcept;
    
    /**
     * @brief Replaces the argument at the specified index.
     * 
     * @param index The index of the argument to replace
     * @param argument The replacement argument
     */
    void replace_argument(size_t index, Argument *argument) noexcept;

    /**
     * @brief Creates a new argument for this function.
     * 
     * @param type The type of the argument
     * @param by_ref Whether the argument is passed by reference
     * @param should_append Whether to append the argument to the function
     * @return A pointer to the created argument
     */
    Argument *create_argument(const Type *type, bool by_ref, bool should_append = true) noexcept;
    
    /**
     * @brief Creates a new value argument for this function.
     * 
     * @param type The type of the argument
     * @param should_append Whether to append the argument to the function
     * @return A pointer to the created value argument
     */
    ValueArgument *create_value_argument(const Type *type, bool should_append = true) noexcept;
    
    /**
     * @brief Creates a new reference argument for this function.
     * 
     * @param type The type of the argument
     * @param should_append Whether to append the argument to the function
     * @return A pointer to the created reference argument
     */
    ReferenceArgument *create_reference_argument(const Type *type, bool should_append = true) noexcept;
    
    /**
     * @brief Creates a new resource argument for this function.
     * 
     * @param type The type of the argument
     * @param should_append Whether to append the argument to the function
     * @return A pointer to the created resource argument
     */
    ResourceArgument *create_resource_argument(const Type *type, bool should_append = true) noexcept;

    /**
     * @brief Creates a new basic block in this function.
     * 
     * @return A pointer to the created basic block
     */
    [[nodiscard]] BasicBlock *create_basic_block() noexcept;

    /**
     * @brief Checks if this function has a definition.
     * 
     * @return True if this function has a definition
     */
    [[nodiscard]] auto is_definition() const noexcept {
        return derived_function_tag() != DerivedFunctionTag::EXTERNAL;
    }

    /**
     * @brief Gets the arguments of this function.
     * 
     * @return A reference to the argument list
     */
    [[nodiscard]] auto &arguments() noexcept { return _arguments; }
    
    /**
     * @brief Gets the arguments of this function (const version).
     * 
     * @return A const reference to the argument list
     */
    [[nodiscard]] auto &arguments() const noexcept { return _arguments; }

    /**
     * @brief Gets the definition of this function.
     * 
     * @return A pointer to the function definition, or nullptr if none exists
     */
    [[nodiscard]] virtual FunctionDefinition *definition() noexcept { return nullptr; }
    
    /**
     * @brief Gets the definition of this function (const version).
     * 
     * @return A pointer to the function definition, or nullptr if none exists
     */
    [[nodiscard]] const FunctionDefinition *definition() const noexcept {
        return const_cast<Function *>(this)->definition();
    }

    /**
     * @brief Type-checking helper for Function objects.
     */
    LUISA_XIR_DEFINED_ISA_METHOD(Function, function)
};

/**
 * @typedef FunctionList
 * @brief A list of functions.
 */
using FunctionList = IntrusiveForwardList<Function>;

/**
 * @class DerivedFunction
 * @brief Template for creating derived function classes.
 * 
 * This template simplifies the creation of new function types by
 * automatically implementing common functionality.
 * 
 * @tparam Derived The derived function class
 * @tparam tag The function tag for this class
 * @tparam Base The base class to derive from
 */
template<typename Derived, DerivedFunctionTag tag, typename Base = Function>
    requires std::derived_from<Base, Function>
class DerivedFunction : public Base {
public:
    /** @brief The derived function type */
    using derived_function_type = Derived;
    /** @brief Alias for the parent class */
    using Super = DerivedFunction;
    /** @brief Inherit constructors from the base class */
    using Base::Base;
    
    /**
     * @brief Gets the static function tag for this class.
     * 
     * @return The derived function tag
     */
    [[nodiscard]] static constexpr DerivedFunctionTag static_derived_function_tag() noexcept { return tag; }
    
    /**
     * @brief Gets the function tag for this instance.
     * 
     * @return The derived function tag
     */
    [[nodiscard]] DerivedFunctionTag derived_function_tag() const noexcept final { return static_derived_function_tag(); }
};

/**
 * @enum BasicBlockTraversalOrder
 * @brief Specifies the order for traversing basic blocks.
 */
enum struct BasicBlockTraversalOrder {
    PRE_ORDER,        /**< Visit a block before its successors */
    POST_ORDER,       /**< Visit a block after its successors */
    REVERSE_PRE_ORDER,/**< Visit a block before its predecessors */
    REVERSE_POST_ORDER,/**< Visit a block after its predecessors */

    // default order
    DEFAULT_ORDER = PRE_ORDER,/**< Default traversal order */
};

/**
 * @class FunctionDefinition
 * @brief Base class for functions with definitions.
 * 
 * FunctionDefinition extends Function to include a body block and
 * methods for traversing the basic blocks and instructions in the function.
 */
class LC_XIR_API FunctionDefinition : public Function {

private:
    /** @brief The body block of this function */
    BasicBlock *_body_block{nullptr};

public:
    /** @brief Inherit constructors from the base class */
    using Function::Function;

    /**
     * @brief Sets the body block of this function.
     * 
     * @param block The body block
     */
    void set_body_block(BasicBlock *block) noexcept;
    
    /**
     * @brief Creates a new body block for this function.
     * 
     * @param overwrite_existing Whether to overwrite an existing body block
     * @return A pointer to the created or existing body block
     */
    BasicBlock *create_body_block(bool overwrite_existing = false) noexcept;

    /**
     * @brief Gets the body block of this function.
     * 
     * @return A pointer to the body block
     */
    [[nodiscard]] BasicBlock *body_block() noexcept { return _body_block; }
    
    /**
     * @brief Gets the body block of this function (const version).
     * 
     * @return A pointer to the body block
     */
    [[nodiscard]] const BasicBlock *body_block() const noexcept { return _body_block; }

    /**
     * @brief Gets the definition of this function.
     * 
     * @return A pointer to this function as a FunctionDefinition
     */
    [[nodiscard]] FunctionDefinition *definition() noexcept final { return this; }

private:
    /**
     * @brief Internal method to traverse basic blocks in pre-order.
     * 
     * @param block The starting block
     * @param visit_ctx Context pointer passed to the visit function
     * @param visit Function to call for each block
     */
    static void _traverse_basic_block_pre_order(BasicBlock *block, void *visit_ctx,
                                                void (*visit)(void *, BasicBlock *)) noexcept;
    
    /**
     * @brief Internal method to traverse basic blocks in post-order.
     * 
     * @param block The starting block
     * @param visit_ctx Context pointer passed to the visit function
     * @param visit Function to call for each block
     */
    static void _traverse_basic_block_post_order(BasicBlock *block, void *visit_ctx,
                                                 void (*visit)(void *, BasicBlock *)) noexcept;
    
    /**
     * @brief Internal method to traverse basic blocks in reverse pre-order.
     * 
     * @param block The starting block
     * @param visit_ctx Context pointer passed to the visit function
     * @param visit Function to call for each block
     */
    static void _traverse_basic_block_reverse_pre_order(BasicBlock *block, void *visit_ctx,
                                                        void (*visit)(void *, BasicBlock *)) noexcept;
    
    /**
     * @brief Internal method to traverse basic blocks in reverse post-order.
     * 
     * @param block The starting block
     * @param visit_ctx Context pointer passed to the visit function
     * @param visit Function to call for each block
     */
    static void _traverse_basic_block_reverse_post_order(BasicBlock *block, void *visit_ctx,
                                                         void (*visit)(void *, BasicBlock *)) noexcept;

public:
    /**
     * @brief Traverses the basic blocks in this function.
     * 
     * @tparam Visit The type of the visitor function
     * @param order The traversal order
     * @param visit Function to call for each basic block
     */
    template<typename Visit>
    void traverse_basic_blocks(BasicBlockTraversalOrder order, Visit &&visit) noexcept {
        auto visitor = [](void *ctx, BasicBlock *block) noexcept {
            (*static_cast<Visit *>(ctx))(block);
        };
        switch (order) {
            default: /* pre-order by default */ [[fallthrough]];
            case BasicBlockTraversalOrder::PRE_ORDER:
                _traverse_basic_block_pre_order(_body_block, &visit, visitor);
                break;
            case BasicBlockTraversalOrder::POST_ORDER:
                _traverse_basic_block_post_order(_body_block, &visit, visitor);
                break;
            case BasicBlockTraversalOrder::REVERSE_PRE_ORDER:
                _traverse_basic_block_reverse_pre_order(_body_block, &visit, visitor);
                break;
            case BasicBlockTraversalOrder::REVERSE_POST_ORDER:
                _traverse_basic_block_reverse_post_order(_body_block, &visit, visitor);
                break;
        }
    }
    
    /**
     * @brief Traverses the basic blocks in this function (const version).
     * 
     * @tparam Visit The type of the visitor function
     * @param order The traversal order
     * @param visit Function to call for each basic block
     */
    template<typename Visit>
    void traverse_basic_blocks(BasicBlockTraversalOrder order, Visit &&visit) const noexcept {
        const_cast<FunctionDefinition *>(this)->traverse_basic_blocks(
            order, [&](const BasicBlock *block) noexcept {
                visit(block);
            });
    }
    
    /**
     * @brief Traverses the instructions in this function.
     * 
     * @tparam Visit The type of the visitor function
     * @param order The traversal order for basic blocks
     * @param visit Function to call for each instruction
     */
    template<typename Visit>
    void traverse_instructions(BasicBlockTraversalOrder order, Visit &&visit) noexcept {
        traverse_basic_blocks(order, [&visit](BasicBlock *block) noexcept {
            block->traverse_instructions(visit);
        });
    }
    
    /**
     * @brief Traverses the instructions in this function (const version).
     * 
     * @tparam Visit The type of the visitor function
     * @param order The traversal order for basic blocks
     * @param visit Function to call for each instruction
     */
    template<typename Visit>
    void traverse_instructions(BasicBlockTraversalOrder order, Visit &&visit) const noexcept {
        traverse_basic_blocks(order, [&visit](const BasicBlock *block) noexcept {
            block->traverse_instructions(visit);
        });
    }

    /**
     * @brief Traverses the basic blocks in this function with the default order.
     * 
     * @tparam Visit The type of the visitor function
     * @param visit Function to call for each basic block
     */
    template<typename Visit>
    void traverse_basic_blocks(Visit &&visit) noexcept {
        traverse_basic_blocks(BasicBlockTraversalOrder::DEFAULT_ORDER, std::forward<Visit>(visit));
    }
    
    /**
     * @brief Traverses the basic blocks in this function with the default order (const version).
     * 
     * @tparam Visit The type of the visitor function
     * @param visit Function to call for each basic block
     */
    template<typename Visit>
    void traverse_basic_blocks(Visit &&visit) const noexcept {
        traverse_basic_blocks(BasicBlockTraversalOrder::DEFAULT_ORDER, std::forward<Visit>(visit));
    }
    
    /**
     * @brief Traverses the instructions in this function with the default order.
     * 
     * @tparam Visit The type of the visitor function
     * @param visit Function to call for each instruction
     */
    template<typename Visit>
    void traverse_instructions(Visit &&visit) noexcept {
        traverse_instructions(BasicBlockTraversalOrder::DEFAULT_ORDER, std::forward<Visit>(visit));
    }
    
    /**
     * @brief Traverses the instructions in this function with the default order (const version).
     * 
     * @tparam Visit The type of the visitor function
     * @param visit Function to call for each instruction
     */
    template<typename Visit>
    void traverse_instructions(Visit &&visit) const noexcept {
        traverse_instructions(BasicBlockTraversalOrder::DEFAULT_ORDER, std::forward<Visit>(visit));
    }
};

/**
 * @class CallableFunction
 * @brief Represents a callable function in XIR.
 * 
 * A callable function can be called from other functions but cannot be
 * directly executed as a kernel.
 */
class LC_XIR_API CallableFunction final : public DerivedFunction<CallableFunction, DerivedFunctionTag::CALLABLE, FunctionDefinition> {
public:
    /** @brief Inherit constructors from the parent class */
    using Super::Super;
};

/**
 * @class KernelFunction
 * @brief Represents a kernel function in XIR.
 * 
 * A kernel function can be executed on a device and serves as the entry point
 * for compute operations.
 */
class LC_XIR_API KernelFunction final : public DerivedFunction<KernelFunction, DerivedFunctionTag::KERNEL, FunctionDefinition> {

public:
    /** @brief Default block size for kernel execution */
    static constexpr auto default_block_size = luisa::make_uint3(64u, 1u, 1u);

private:
    /** @brief The block size for this kernel */
    std::array<uint, 3> _block_size;

public:
    /**
     * @brief Constructs a kernel function with the specified block size.
     * 
     * @param parent_module The module containing this kernel
     * @param block_size The block size for kernel execution
     */
    explicit KernelFunction(Module *parent_module, luisa::uint3 block_size = default_block_size) noexcept;
    
    /**
     * @brief Sets the block size for this kernel.
     * 
     * @param size The new block size
     */
    void set_block_size(luisa::uint3 size) noexcept;
    
    /**
     * @brief Gets the block size for this kernel.
     * 
     * @return The block size
     */
    [[nodiscard]] luisa::uint3 block_size() const noexcept;
};

/**
 * @class ExternalFunction
 * @brief Represents an external function in XIR.
 * 
 * An external function is a declaration without a definition, typically
 * used to reference functions defined outside the current module.
 */
class LC_XIR_API ExternalFunction final : public DerivedFunction<ExternalFunction, DerivedFunctionTag::EXTERNAL> {
public:
    /** @brief Inherit constructors from the parent class */
    using Super::Super;
};

}// namespace luisa::compute::xir
