#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class Function;
class Instruction;
class User;

class LC_XIR_API BasicBlock final : public DerivedFunctionScopeValue<BasicBlock, DerivedValueTag::BASIC_BLOCK> {

private:
    InstructionList _instructions;

private:
    void _do_traverse_predecessors(bool exclude_self, void *ctx, void (*visit)(void *, BasicBlock *)) noexcept;
    void _do_traverse_successors(bool exclude_self, void *ctx, void (*visit)(void *, BasicBlock *)) noexcept;

public:
    explicit BasicBlock(Function *parent_function) noexcept;

    [[nodiscard]] auto &instructions() noexcept { return _instructions; }
    [[nodiscard]] auto &instructions() const noexcept { return _instructions; }

    [[nodiscard]] bool is_terminated() const noexcept;
    [[nodiscard]] TerminatorInstruction *terminator() noexcept;
    [[nodiscard]] const TerminatorInstruction *terminator() const noexcept;

    template<typename Visit>
    void traverse_predecessors(bool exclude_self, Visit &&visit) noexcept {
        _do_traverse_predecessors(
            exclude_self, &visit, [](void *ctx, BasicBlock *bb) noexcept {
                (*static_cast<Visit *>(ctx))(bb);
            });
    }

    template<typename Visit>
    void traverse_predecessors(bool exclude_self, Visit &&visit) const noexcept {
        const_cast<BasicBlock *>(this)->traverse_predecessors(
            exclude_self, [&visit](const BasicBlock *bb) noexcept {
                visit(bb);
            });
    }

    template<typename Visit>
    void traverse_successors(bool exclude_self, Visit &&visit) noexcept {
        _do_traverse_successors(
            exclude_self, &visit, [](void *ctx, BasicBlock *bb) noexcept {
                (*static_cast<Visit *>(ctx))(bb);
            });
    }

    template<typename Visit>
    void traverse_successors(bool exclude_self, Visit &&visit) const noexcept {
        const_cast<BasicBlock *>(this)->traverse_successors(
            exclude_self, [&visit](const BasicBlock *bb) noexcept {
                visit(bb);
            });
    }

    template<typename Visit>
    void traverse_instructions(Visit &&visit) noexcept {
        for (auto &inst : _instructions) {
            visit(&inst);
        }
    }

    template<typename Visit>
    void traverse_instructions(Visit &&visit) const noexcept {
        const_cast<BasicBlock *>(this)->traverse_instructions(
            [&visit](const Instruction *inst) noexcept {
                visit(inst);
            });
    }
};

}// namespace luisa::compute::xir
