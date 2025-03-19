#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class Function;
class Instruction;
class User;

/**
 * @class BasicBlock
 * @brief Represents a basic block in the XIR control flow graph.
 * 
 * A basic block is a sequence of instructions with a single entry point and a single exit point.
 * It forms the building block of control flow in XIR. A basic block ends with a terminator
 * instruction that determines the next block to execute.
 */
class LC_XIR_API BasicBlock final : public DerivedFunctionScopeValue<BasicBlock, DerivedValueTag::BASIC_BLOCK> {

private:
    /** @brief The list of instructions in this basic block */
    InstructionList _instructions;

private:
    /**
     * @brief Internal method to traverse predecessor blocks.
     * 
     * @param exclude_self Whether to exclude this block from traversal
     * @param ctx Context pointer passed to the visit function
     * @param visit Function to call for each predecessor block
     */
    void _do_traverse_predecessors(bool exclude_self, void *ctx, void (*visit)(void *, BasicBlock *)) noexcept;
    
    /**
     * @brief Internal method to traverse successor blocks.
     * 
     * @param exclude_self Whether to exclude this block from traversal
     * @param ctx Context pointer passed to the visit function
     * @param visit Function to call for each successor block
     */
    void _do_traverse_successors(bool exclude_self, void *ctx, void (*visit)(void *, BasicBlock *)) noexcept;

public:
    /**
     * @brief Constructs a basic block within the specified function.
     * 
     * @param parent_function The function containing this basic block
     */
    explicit BasicBlock(Function *parent_function) noexcept;

    /**
     * @brief Gets the list of instructions in this basic block.
     * 
     * @return A reference to the instruction list
     */
    [[nodiscard]] auto &instructions() noexcept { return _instructions; }
    
    /**
     * @brief Gets the list of instructions in this basic block (const version).
     * 
     * @return A const reference to the instruction list
     */
    [[nodiscard]] auto &instructions() const noexcept { return _instructions; }

    /**
     * @brief Checks if this basic block is terminated.
     * 
     * @return True if the block has a terminator instruction
     */
    [[nodiscard]] bool is_terminated() const noexcept;
    
    /**
     * @brief Gets the terminator instruction of this basic block.
     * 
     * @return A pointer to the terminator instruction, or nullptr if none exists
     */
    [[nodiscard]] TerminatorInstruction *terminator() noexcept;
    
    /**
     * @brief Gets the terminator instruction of this basic block (const version).
     * 
     * @return A pointer to the terminator instruction, or nullptr if none exists
     */
    [[nodiscard]] const TerminatorInstruction *terminator() const noexcept;

    /**
     * @brief Traverses the predecessor blocks of this basic block.
     * 
     * @tparam Visit The type of the visitor function
     * @param exclude_self Whether to exclude this block from traversal
     * @param visit Function to call for each predecessor block
     */
    template<typename Visit>
    void traverse_predecessors(bool exclude_self, Visit &&visit) noexcept {
        _do_traverse_predecessors(
            exclude_self, &visit, [](void *ctx, BasicBlock *bb) noexcept {
                (*static_cast<Visit *>(ctx))(bb);
            });
    }

    /**
     * @brief Traverses the predecessor blocks of this basic block (const version).
     * 
     * @tparam Visit The type of the visitor function
     * @param exclude_self Whether to exclude this block from traversal
     * @param visit Function to call for each predecessor block
     */
    template<typename Visit>
    void traverse_predecessors(bool exclude_self, Visit &&visit) const noexcept {
        const_cast<BasicBlock *>(this)->traverse_predecessors(
            exclude_self, [&visit](const BasicBlock *bb) noexcept {
                visit(bb);
            });
    }

    /**
     * @brief Traverses the successor blocks of this basic block.
     * 
     * @tparam Visit The type of the visitor function
     * @param exclude_self Whether to exclude this block from traversal
     * @param visit Function to call for each successor block
     */
    template<typename Visit>
    void traverse_successors(bool exclude_self, Visit &&visit) noexcept {
        _do_traverse_successors(
            exclude_self, &visit, [](void *ctx, BasicBlock *bb) noexcept {
                (*static_cast<Visit *>(ctx))(bb);
            });
    }

    /**
     * @brief Traverses the successor blocks of this basic block (const version).
     * 
     * @tparam Visit The type of the visitor function
     * @param exclude_self Whether to exclude this block from traversal
     * @param visit Function to call for each successor block
     */
    template<typename Visit>
    void traverse_successors(bool exclude_self, Visit &&visit) const noexcept {
        const_cast<BasicBlock *>(this)->traverse_successors(
            exclude_self, [&visit](const BasicBlock *bb) noexcept {
                visit(bb);
            });
    }

    /**
     * @brief Traverses the instructions in this basic block.
     * 
     * @tparam Visit The type of the visitor function
     * @param visit Function to call for each instruction
     */
    template<typename Visit>
    void traverse_instructions(Visit &&visit) noexcept {
        for (auto &inst : _instructions) {
            visit(&inst);
        }
    }

    /**
     * @brief Traverses the instructions in this basic block (const version).
     * 
     * @tparam Visit The type of the visitor function
     * @param visit Function to call for each instruction
     */
    template<typename Visit>
    void traverse_instructions(Visit &&visit) const noexcept {
        const_cast<BasicBlock *>(this)->traverse_instructions(
            [&visit](const Instruction *inst) noexcept {
                visit(inst);
            });
    }
};

}// namespace luisa::compute::xir
