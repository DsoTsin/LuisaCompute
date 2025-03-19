#pragma once

#include <luisa/core/concepts.h>
#include <luisa/xir/user.h>

namespace luisa::compute::xir {

class BasicBlock;
class Function;

/**
 * @enum DerivedInstructionTag
 * @brief Identifies the specific type of instruction in the XIR system.
 * 
 * This enumeration categorizes all possible instruction types in the XIR intermediate
 * representation. Instructions are grouped by their functionality (control flow,
 * memory operations, arithmetic, etc.).
 */
enum struct DerivedInstructionTag {

    /* control flow instructions */
    IF,                /**< Conditional branch instruction (if statement) */
    SWITCH,            /**< Multi-way branch instruction (switch statement) */
    LOOP,              /**< General loop construct */
    SIMPLE_LOOP,       /**< Simplified loop construct (do-while) */
    BRANCH,            /**< Unconditional branch to a target block */
    CONDITIONAL_BRANCH,/**< Branch based on a boolean condition */
    UNREACHABLE,       /**< Marks code that should never be executed */
    BREAK,             /**< Exit from a loop (removed after control flow normalization) */
    CONTINUE,          /**< Skip to next iteration (removed after control flow normalization) */
    RETURN,            /**< Return from function (early returns removed after normalization) */
    RASTER_DISCARD,    /**< Discard the current fragment in raster pipeline */

    /* PHI nodes */
    PHI,/**< Phi node for SSA form, selects value based on predecessor block */

    /* variable instructions */
    ALLOCA, /**< Allocate memory on stack or in shared memory */
    LOAD,   /**< Load value from memory */
    STORE,  /**< Store value to memory */
    GEP,    /**< Get element pointer - compute address of struct field or array element */

    /* atomic instructions */
    ATOMIC,/**< Atomic memory operations on buffers or shared memory */

    /* ALU (arithmetic logic unit) instructions */
    ARITHMETIC,/**< Arithmetic operations (add, sub, mul, etc.) */

    /* thread-group instructions */
    THREAD_GROUP,/**< Thread group operations, may involve synchronization */

    /* resource instructions */
    RESOURCE_QUERY, /**< Query resource properties, can be freely moved/eliminated */
    RESOURCE_READ,  /**< Read from resources, may be eliminated if unused */
    RESOURCE_WRITE, /**< Write to resources, may be volatile to elimination/motion */

    /* ray query instructions */
    RAY_QUERY_LOOP,        /**< Loop for ray query traversal */
    RAY_QUERY_DISPATCH,    /**< Dispatch based on ray query results */
    RAY_QUERY_OBJECT_READ, /**< Read from ray query objects */
    RAY_QUERY_OBJECT_WRITE,/**< Write to ray query objects */
    RAY_QUERY_PIPELINE,    /**< Ray query pipeline with callbacks */

    /* automatic differentiation */
    AUTODIFF_SCOPE,     /**< Scope for automatic differentiation */
    AUTODIFF_INTRINSIC, /**< Intrinsic functions for automatic differentiation */

    /* other instructions */
    CALL,   /**< Function call (user or external) */
    CAST,   /**< Type conversion */
    PRINT,  /**< Debug print */
    CLOCK,  /**< Performance timing */

    ASSERT, /**< Runtime assertion */
    ASSUME, /**< Compiler assumption hint */

    OUTLINE,/**< Mark code region for potential outlining (separate compilation) */
};

/**
 * @brief Converts a DerivedInstructionTag to its string representation.
 * 
 * @param tag The instruction tag to convert
 * @return A string view containing the tag's name
 */
[[nodiscard]] constexpr luisa::string_view to_string(DerivedInstructionTag tag) noexcept {
    using namespace std::string_view_literals;
    switch (tag) {
        case DerivedInstructionTag::IF: return "if"sv;
        case DerivedInstructionTag::SWITCH: return "switch"sv;
        case DerivedInstructionTag::LOOP: return "loop"sv;
        case DerivedInstructionTag::SIMPLE_LOOP: return "simple_loop"sv;
        case DerivedInstructionTag::BRANCH: return "branch"sv;
        case DerivedInstructionTag::CONDITIONAL_BRANCH: return "conditional_branch"sv;
        case DerivedInstructionTag::UNREACHABLE: return "unreachable"sv;
        case DerivedInstructionTag::BREAK: return "break"sv;
        case DerivedInstructionTag::CONTINUE: return "continue"sv;
        case DerivedInstructionTag::RETURN: return "return"sv;
        case DerivedInstructionTag::RASTER_DISCARD: return "raster_discard"sv;
        case DerivedInstructionTag::PHI: return "phi"sv;
        case DerivedInstructionTag::ALLOCA: return "alloca"sv;
        case DerivedInstructionTag::LOAD: return "load"sv;
        case DerivedInstructionTag::STORE: return "store"sv;
        case DerivedInstructionTag::GEP: return "gep"sv;
        case DerivedInstructionTag::ATOMIC: return "atomic"sv;
        case DerivedInstructionTag::ARITHMETIC: return "arithmetic"sv;
        case DerivedInstructionTag::THREAD_GROUP: return "thread_group"sv;
        case DerivedInstructionTag::RESOURCE_QUERY: return "resource_query"sv;
        case DerivedInstructionTag::RESOURCE_READ: return "resource_read"sv;
        case DerivedInstructionTag::RESOURCE_WRITE: return "resource_write"sv;
        case DerivedInstructionTag::RAY_QUERY_LOOP: return "ray_query_loop"sv;
        case DerivedInstructionTag::RAY_QUERY_DISPATCH: return "ray_query_dispatch"sv;
        case DerivedInstructionTag::RAY_QUERY_OBJECT_READ: return "ray_query_object_read"sv;
        case DerivedInstructionTag::RAY_QUERY_OBJECT_WRITE: return "ray_query_object_write"sv;
        case DerivedInstructionTag::RAY_QUERY_PIPELINE: return "ray_query_pipeline"sv;
        case DerivedInstructionTag::CALL: return "call"sv;
        case DerivedInstructionTag::CAST: return "cast"sv;
        case DerivedInstructionTag::PRINT: return "print"sv;
        case DerivedInstructionTag::CLOCK: return "clock"sv;
        case DerivedInstructionTag::ASSERT: return "assert"sv;
        case DerivedInstructionTag::ASSUME: return "assume"sv;
        case DerivedInstructionTag::OUTLINE: return "outline"sv;
        case DerivedInstructionTag::AUTODIFF_SCOPE: return "autodiff_scope"sv;
        case DerivedInstructionTag::AUTODIFF_INTRINSIC: return "autodiff_intrinsic"sv;
    }
    return "unknown"sv;
}

class ControlFlowMerge;

/**
 * @class InstructionCloneValueResolver
 * @brief Interface for resolving values during instruction cloning.
 * 
 * This abstract class defines the interface for resolving values when cloning
 * instructions. Implementations of this interface map original values to their
 * corresponding cloned values.
 */
struct InstructionCloneValueResolver {
    virtual ~InstructionCloneValueResolver() noexcept = default;
    
    /**
     * @brief Resolves a value during the cloning process.
     * 
     * @param value The original value to resolve
     * @return The corresponding cloned value
     */
    [[nodiscard]] virtual Value *resolve(const Value *value) noexcept = 0;
};

class XIRBuilder;

/**
 * @class Instruction
 * @brief Base class for all XIR instructions.
 * 
 * Instruction is the fundamental unit of computation in the XIR system.
 * All specific instruction types derive from this base class. Instructions
 * are organized in basic blocks and can have operands (inputs) and produce
 * results (outputs).
 */
class LC_XIR_API Instruction : public IntrusiveNode<Instruction, DerivedBlockScopeValue<Instruction, DerivedValueTag::INSTRUCTION, User>> {

protected:
    /**
     * @brief Removes this instruction from its operands' use lists.
     */
    void _remove_self_from_operand_use_lists() noexcept;
    
    /**
     * @brief Adds this instruction to its operands' use lists.
     */
    void _add_self_to_operand_use_lists() noexcept;
    
    /**
     * @brief Determines if this instruction should be added to operand use lists.
     * 
     * @return True if the instruction should be added to operand use lists
     */
    [[nodiscard]] bool _should_add_self_to_operand_use_lists() const noexcept override;

public:
    /**
     * @brief Constructs an instruction with a parent block and result type.
     * 
     * @param parent_block The basic block containing this instruction
     * @param type The result type of this instruction
     */
    explicit Instruction(BasicBlock *parent_block, const Type *type) noexcept;
    
    /**
     * @brief Gets the specific instruction tag for this instruction.
     * 
     * @return The derived instruction tag
     */
    [[nodiscard]] virtual DerivedInstructionTag derived_instruction_tag() const noexcept = 0;
    
    /**
     * @brief Creates a clone of this instruction.
     * 
     * @param b The XIR builder to use for creating the clone
     * @param resolver The resolver for mapping original values to cloned values
     * @return A clone of this instruction
     */
    [[nodiscard]] virtual Instruction *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept = 0;

    /**
     * @brief Removes this instruction from its parent block.
     */
    void remove_self() noexcept override;
    
    /**
     * @brief Inserts another instruction before this one.
     * 
     * @param node The instruction to insert before this one
     */
    void insert_before_self(Instruction *node) noexcept override;
    
    /**
     * @brief Inserts another instruction after this one.
     * 
     * @param node The instruction to insert after this one
     */
    void insert_after_self(Instruction *node) noexcept override;
    
    /**
     * @brief Replaces this instruction with another one.
     * 
     * @param node The instruction to replace this one with
     */
    void replace_self_with(Instruction *node) noexcept;

    /**
     * @brief Checks if this instruction is a basic block terminator.
     * 
     * @return True if this instruction is a terminator
     */
    [[nodiscard]] virtual bool is_terminator() const noexcept { return false; }

    /**
     * @brief Gets the control flow merge associated with this instruction.
     * 
     * @return The control flow merge, or nullptr if none exists
     */
    [[nodiscard]] virtual ControlFlowMerge *control_flow_merge() noexcept { return nullptr; }
    
    /**
     * @brief Gets the control flow merge associated with this instruction (const version).
     * 
     * @return The control flow merge, or nullptr if none exists
     */
    [[nodiscard]] const ControlFlowMerge *control_flow_merge() const noexcept;

    /**
     * @brief Type-checking helper for Instruction objects.
     */
    LUISA_XIR_DEFINED_ISA_METHOD(Instruction, instruction)
};

/**
 * @class SentinelInst
 * @brief Special instruction used as a sentinel in instruction lists.
 * 
 * This class represents a sentinel node in intrusive instruction lists.
 * It doesn't perform any computation and is used only for list management.
 */
class LC_XIR_API SentinelInst final : public Instruction {
public:
    /**
     * @brief Constructs a sentinel instruction for a basic block.
     * 
     * @param parent_block The basic block containing this sentinel
     */
    explicit SentinelInst(BasicBlock *parent_block) noexcept;
    
    /**
     * @brief Gets the derived instruction tag for this sentinel.
     * 
     * @return The derived instruction tag
     */
    [[nodiscard]] DerivedInstructionTag derived_instruction_tag() const noexcept override;
    
    /**
     * @brief Creates a clone of this sentinel instruction.
     * 
     * @param b The XIR builder to use for creating the clone
     * @param resolver The resolver for mapping original values to cloned values
     * @return A clone of this sentinel instruction
     */
    [[nodiscard]] Instruction *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

/**
 * @typedef InstructionList
 * @brief A doubly-linked list of instructions.
 * 
 * This type represents a list of instructions within a basic block,
 * using SentinelInst objects as head and tail markers.
 */
using InstructionList = InlineIntrusiveList<Instruction, SentinelInst>;

/**
 * @class TerminatorInstruction
 * @brief Base class for instructions that terminate basic blocks.
 * 
 * Terminator instructions are special instructions that end basic blocks
 * and control the flow of execution to other blocks.
 */
class LC_XIR_API TerminatorInstruction : public Instruction {
public:
    /**
     * @brief Constructs a terminator instruction for a basic block.
     * 
     * @param block The basic block containing this terminator
     */
    explicit TerminatorInstruction(BasicBlock *block) noexcept;
    
    /**
     * @brief Checks if this instruction is a terminator.
     * 
     * @return Always returns true for terminator instructions
     */
    [[nodiscard]] bool is_terminator() const noexcept final { return true; }
};

/**
 * @class BranchTerminatorInstruction
 * @brief Represents an unconditional branch to another basic block.
 * 
 * This instruction transfers control flow to a target basic block
 * without any conditions.
 */
class LC_XIR_API BranchTerminatorInstruction : public TerminatorInstruction {

public:
    /** @brief Operand index for the target block */
    static constexpr size_t operand_index_target = 0u;
    /** @brief Starting index for derived class operands */
    static constexpr size_t derived_operand_index_offset = 1u;

public:
    /**
     * @brief Constructs an unconditional branch instruction.
     * 
     * @param parent_block The basic block containing this branch
     */
    explicit BranchTerminatorInstruction(BasicBlock *parent_block) noexcept;

    /**
     * @brief Sets the target basic block for this branch.
     * 
     * @param target The target basic block
     */
    void set_target_block(BasicBlock *target) noexcept;
    
    /**
     * @brief Creates a new target basic block for this branch.
     * 
     * @param overwrite_existing Whether to overwrite an existing target
     * @return The created or existing target block
     */
    BasicBlock *create_target_block(bool overwrite_existing = false) noexcept;

    /**
     * @brief Gets the target basic block of this branch.
     * 
     * @return The target basic block
     */
    [[nodiscard]] BasicBlock *target_block() noexcept;
    
    /**
     * @brief Gets the target basic block of this branch (const version).
     * 
     * @return The target basic block
     */
    [[nodiscard]] const BasicBlock *target_block() const noexcept;
};

/**
 * @class ConditionalBranchTerminatorInstruction
 * @brief Represents a conditional branch to one of two basic blocks.
 * 
 * This instruction transfers control flow to either a "true" target block
 * or a "false" target block based on a boolean condition.
 */
class LC_XIR_API ConditionalBranchTerminatorInstruction : public TerminatorInstruction {

public:
    /** @brief Operand index for the condition value */
    static constexpr size_t operand_index_condition = 0u;
    /** @brief Operand index for the true target block */
    static constexpr size_t operand_index_true_target = 1u;
    /** @brief Operand index for the false target block */
    static constexpr size_t operand_index_false_target = 2u;
    /** @brief Starting index for derived class operands */
    static constexpr size_t derived_operand_index_offset = 3u;

public:
    /**
     * @brief Constructs a conditional branch instruction.
     * 
     * @param parent_block The basic block containing this branch
     * @param condition The boolean condition determining the branch target
     */
    explicit ConditionalBranchTerminatorInstruction(BasicBlock *parent_block,
                                                    Value *condition = nullptr) noexcept;

    /**
     * @brief Sets the condition for this branch.
     * 
     * @param condition The boolean condition
     */
    void set_condition(Value *condition) noexcept;
    
    /**
     * @brief Sets the target block for the true condition.
     * 
     * @param target The true target block
     */
    void set_true_target(BasicBlock *target) noexcept;
    
    /**
     * @brief Sets the target block for the false condition.
     * 
     * @param target The false target block
     */
    void set_false_target(BasicBlock *target) noexcept;

    /**
     * @brief Creates a new true target basic block.
     * 
     * @param overwrite_existing Whether to overwrite an existing target
     * @return The created or existing true target block
     */
    BasicBlock *create_true_block(bool overwrite_existing = false) noexcept;
    
    /**
     * @brief Creates a new false target basic block.
     * 
     * @param overwrite_existing Whether to overwrite an existing target
     * @return The created or existing false target block
     */
    BasicBlock *create_false_block(bool overwrite_existing = false) noexcept;

    /**
     * @brief Gets the condition value of this branch.
     * 
     * @return The condition value
     */
    [[nodiscard]] Value *condition() noexcept;
    
    /**
     * @brief Gets the condition value of this branch (const version).
     * 
     * @return The condition value
     */
    [[nodiscard]] const Value *condition() const noexcept;

    /**
     * @brief Gets the true target block of this branch.
     * 
     * @return The true target block
     */
    [[nodiscard]] BasicBlock *true_block() noexcept;
    
    /**
     * @brief Gets the true target block of this branch (const version).
     * 
     * @return The true target block
     */
    [[nodiscard]] const BasicBlock *true_block() const noexcept;

    /**
     * @brief Gets the false target block of this branch.
     * 
     * @return The false target block
     */
    [[nodiscard]] BasicBlock *false_block() noexcept;
    
    /**
     * @brief Gets the false target block of this branch (const version).
     * 
     * @return The false target block
     */
    [[nodiscard]] const BasicBlock *false_block() const noexcept;
};

/**
 * @class DerivedInstruction
 * @brief Template for creating derived instruction classes.
 * 
 * This template simplifies the creation of new instruction types by
 * automatically implementing common functionality.
 * 
 * @tparam Derived The derived instruction class
 * @tparam tag The instruction tag for this class
 * @tparam Base The base class to derive from
 */
template<typename Derived, DerivedInstructionTag tag, typename Base = Instruction>
    requires std::derived_from<Base, Instruction>
class DerivedInstruction : public Base {
public:
    /** @brief The derived instruction type */
    using derived_instruction_type = Derived;
    /** @brief Alias for the parent class */
    using Super = DerivedInstruction;
    /** @brief Inherit constructors from the base class */
    using Base::Base;

    /**
     * @brief Gets the static instruction tag for this class.
     * 
     * @return The derived instruction tag
     */
    [[nodiscard]] static constexpr DerivedInstructionTag
    static_derived_instruction_tag() noexcept { return tag; }

    /**
     * @brief Gets the instruction tag for this instance.
     * 
     * @return The derived instruction tag
     */
    [[nodiscard]] DerivedInstructionTag
    derived_instruction_tag() const noexcept final {
        return static_derived_instruction_tag();
    }
};

/**
 * @class DerivedTerminatorInstruction
 * @brief Template for creating derived terminator instruction classes.
 * 
 * This template simplifies the creation of new terminator instruction types.
 * 
 * @tparam Derived The derived instruction class
 * @tparam tag The instruction tag for this class
 */
template<typename Derived, DerivedInstructionTag tag>
class DerivedTerminatorInstruction : public DerivedInstruction<Derived, tag, TerminatorInstruction> {
public:
    /** @brief Alias for the parent class */
    using Super = DerivedTerminatorInstruction;
    /** @brief Inherit constructors from the parent class */
    using DerivedInstruction<Derived, tag, TerminatorInstruction>::DerivedInstruction;
};

/**
 * @class DerivedBranchInstruction
 * @brief Template for creating derived branch instruction classes.
 * 
 * This template simplifies the creation of new unconditional branch instruction types.
 * 
 * @tparam Derived The derived instruction class
 * @tparam tag The instruction tag for this class
 */
template<typename Derived, DerivedInstructionTag tag>
class DerivedBranchInstruction : public DerivedInstruction<Derived, tag, BranchTerminatorInstruction> {
public:
    /** @brief Alias for the parent class */
    using Super = DerivedBranchInstruction;
    /** @brief Inherit constructors from the parent class */
    using DerivedInstruction<Derived, tag, BranchTerminatorInstruction>::DerivedInstruction;
};

/**
 * @class DerivedConditionalBranchInstruction
 * @brief Template for creating derived conditional branch instruction classes.
 * 
 * This template simplifies the creation of new conditional branch instruction types.
 * 
 * @tparam Derived The derived instruction class
 * @tparam tag The instruction tag for this class
 */
template<typename Derived, DerivedInstructionTag tag>
class DerivedConditionalBranchInstruction : public DerivedInstruction<Derived, tag, ConditionalBranchTerminatorInstruction> {
public:
    /** @brief Alias for the parent class */
    using Super = DerivedConditionalBranchInstruction;
    /** @brief Inherit constructors from the parent class */
    using DerivedInstruction<Derived, tag, ConditionalBranchTerminatorInstruction>::DerivedInstruction;
};

/**
 * @class ControlFlowMerge
 * @brief Represents a control flow merge point.
 * 
 * This class is used to represent merge points in control flow graphs,
 * such as the end of if-then-else constructs or loop exits.
 */
class LC_XIR_API ControlFlowMerge : luisa::concepts::Noncopyable {

private:
    /** @brief The basic block where control flow merges */
    BasicBlock *_merge_block{nullptr};

protected:
    /** @brief Default constructor */
    ControlFlowMerge() noexcept = default;
    /** @brief Default destructor */
    ~ControlFlowMerge() noexcept = default;

private:
    /**
     * @brief Gets the base instruction associated with this merge.
     * 
     * @return The base instruction
     */
    [[nodiscard]] virtual Instruction *_base_instruction() noexcept = 0;

public:
    /**
     * @brief Sets the merge block for this control flow merge.
     * 
     * @param block The merge basic block
     */
    void set_merge_block(BasicBlock *block) noexcept;
    
    /**
     * @brief Gets the merge block for this control flow merge.
     * 
     * @return The merge basic block
     */
    [[nodiscard]] BasicBlock *merge_block() noexcept { return _merge_block; }
    
    /**
     * @brief Gets the merge block for this control flow merge (const version).
     * 
     * @return The merge basic block
     */
    [[nodiscard]] const BasicBlock *merge_block() const noexcept { return _merge_block; }
    
    /**
     * @brief Creates a new merge basic block.
     * 
     * @param overwrite_existing Whether to overwrite an existing merge block
     * @return The created or existing merge block
     */
    BasicBlock *create_merge_block(bool overwrite_existing = false) noexcept;
};

/**
 * @class ControlFlowMergeMixin
 * @brief Mixin that adds control flow merge functionality to instructions.
 * 
 * This template adds control flow merge functionality to instruction classes
 * that need to represent merge points in the control flow graph.
 * 
 * @tparam Base The base instruction class
 */
template<typename Base>
    requires std::derived_from<Base, Instruction>
class ControlFlowMergeMixin : public Base,
                              public ControlFlowMerge {
private:
    /**
     * @brief Gets the base instruction for this control flow merge.
     * 
     * @return The base instruction
     */
    [[nodiscard]] Instruction *_base_instruction() noexcept final {
        return static_cast<Instruction *>(this);
    }

public:
    /** @brief Alias for the parent class */
    using Super = ControlFlowMergeMixin;
    /** @brief Inherit constructors from the base class */
    using Base::Base;
    
    /**
     * @brief Gets the control flow merge for this instruction.
     * 
     * @return This object as a ControlFlowMerge
     */
    [[nodiscard]] ControlFlowMerge *control_flow_merge() noexcept final { return this; }
};

/**
 * @class InstructionOpMixin
 * @brief Mixin that adds an operation type to instructions.
 * 
 * This template adds an operation field to instruction classes
 * that need to represent different operations of the same type.
 * 
 * @tparam OpType The type of operation
 */
template<typename OpType>
class InstructionOpMixin {

private:
    /** @brief The operation for this instruction */
    OpType _op;

public:
    /**
     * @brief Constructs an instruction with the specified operation.
     * 
     * @param op The operation
     */
    explicit InstructionOpMixin(OpType op) noexcept : _op{op} {}
    
    /**
     * @brief Gets the operation for this instruction.
     * 
     * @return The operation
     */
    [[nodiscard]] OpType op() const noexcept { return _op; }
    
    /**
     * @brief Sets the operation for this instruction.
     * 
     * @param op The new operation
     */
    void set_op(OpType op) noexcept { _op = op; }
};

}// namespace luisa::compute::xir
