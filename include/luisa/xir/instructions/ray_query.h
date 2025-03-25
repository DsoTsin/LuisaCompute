#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API RayQueryObjectReadInst final : public InstructionOpMixin<RayQueryObjectReadOp, DerivedInstruction<RayQueryObjectReadInst, DerivedInstructionTag::RAY_QUERY_OBJECT_READ>> {

public:
    RayQueryObjectReadInst(BasicBlock *parent_block, const Type *type, RayQueryObjectReadOp op,
                           luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] RayQueryObjectReadInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

class LC_XIR_API RayQueryObjectWriteInst final : public InstructionOpMixin<RayQueryObjectWriteOp, DerivedInstruction<RayQueryObjectWriteInst, DerivedInstructionTag::RAY_QUERY_OBJECT_WRITE>> {
public:
    RayQueryObjectWriteInst(BasicBlock *parent_block, RayQueryObjectWriteOp op,
                            luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] RayQueryObjectWriteInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

// Ray query control flow instructions:
// RayQueryLoop {
//   /* dispatch_block */
//   RayQueryDispatch(object)
//     -> merge_block
//     -> on_surface_candidate_block {
//       /* on surface candidate block */
//       br dispatch_block
//     }
//     -> on_procedural_candidate_block {
//       /* on procedural candidate block */
//       br dispatch_block
//     }
// }
// /* merge_block */
// { ... }
class LC_XIR_API RayQueryLoopInst final : public ControlFlowMergeMixin<DerivedTerminatorInstruction<RayQueryLoopInst, DerivedInstructionTag::RAY_QUERY_LOOP>> {

public:
    static constexpr size_t operand_index_dispatch_block = 0u;

public:
    explicit RayQueryLoopInst(BasicBlock *parent_block) noexcept;
    void set_dispatch_block(BasicBlock *block) noexcept;
    BasicBlock *create_dispatch_block() noexcept;
    [[nodiscard]] BasicBlock *dispatch_block() noexcept;
    [[nodiscard]] const BasicBlock *dispatch_block() const noexcept;
    [[nodiscard]] RayQueryLoopInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

class LC_XIR_API RayQueryDispatchInst final : public DerivedTerminatorInstruction<RayQueryDispatchInst, DerivedInstructionTag::RAY_QUERY_DISPATCH> {

public:
    static constexpr size_t operand_index_query_object = 0u;
    static constexpr size_t operand_index_exit_block = 1u;
    static constexpr size_t operand_index_on_surface_candidate_block = 2u;
    static constexpr size_t operand_index_on_procedural_candidate_block = 3u;

public:
    RayQueryDispatchInst(BasicBlock *parent_block, Value *query_object) noexcept;

    void set_query_object(Value *query_object) noexcept;
    void set_exit_block(BasicBlock *block) noexcept;

    void set_on_surface_candidate_block(BasicBlock *block) noexcept;
    void set_on_procedural_candidate_block(BasicBlock *block) noexcept;

    BasicBlock *create_on_surface_candidate_block() noexcept;
    BasicBlock *create_on_procedural_candidate_block() noexcept;

    [[nodiscard]] Value *query_object() noexcept;
    [[nodiscard]] const Value *query_object() const noexcept;

    [[nodiscard]] BasicBlock *exit_block() noexcept;
    [[nodiscard]] const BasicBlock *exit_block() const noexcept;

    [[nodiscard]] BasicBlock *on_surface_candidate_block() noexcept;
    [[nodiscard]] const BasicBlock *on_surface_candidate_block() const noexcept;

    [[nodiscard]] BasicBlock *on_procedural_candidate_block() noexcept;
    [[nodiscard]] const BasicBlock *on_procedural_candidate_block() const noexcept;

    [[nodiscard]] RayQueryDispatchInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

// Ray query pipeline instruction:
// RayQueryPipeline(query_object, on_surface_func, on_procedural_func, captured_args...)
// The signature of on_*_func: (query_object, captured_args...) -> void
class LC_XIR_API RayQueryPipelineInst final : public DerivedInstruction<RayQueryPipelineInst, DerivedInstructionTag::RAY_QUERY_PIPELINE> {

public:
    static constexpr size_t operand_index_query_object = 0u;
    static constexpr size_t operand_index_on_surface_function = 1u;
    static constexpr size_t operand_index_on_procedural_function = 2u;
    static constexpr size_t operand_index_offset_captured_arguments = 3u;

public:
    RayQueryPipelineInst(BasicBlock *parent_block, Value *query_object,
                         Function *on_surface, Function *on_procedural,
                         luisa::span<Value *const> captured_args = {}) noexcept;

    void set_query_object(Value *query_object) noexcept;
    void set_on_surface_function(Function *on_surface) noexcept;
    void set_on_procedural_function(Function *on_procedural) noexcept;

    void set_captured_argument(size_t index, Value *arg) noexcept;
    void add_captured_argument(Value *arg) noexcept;
    void set_captured_arguments(luisa::span<Value *const> args) noexcept;
    void set_captured_argument_count(size_t count) noexcept;

    [[nodiscard]] Value *query_object() noexcept;
    [[nodiscard]] const Value *query_object() const noexcept;

    [[nodiscard]] Function *on_surface_function() noexcept;
    [[nodiscard]] const Function *on_surface_function() const noexcept;

    [[nodiscard]] Function *on_procedural_function() noexcept;
    [[nodiscard]] const Function *on_procedural_function() const noexcept;

    [[nodiscard]] luisa::span<Use *const> captured_argument_uses() noexcept;
    [[nodiscard]] luisa::span<const Use *const> captured_argument_uses() const noexcept;

    [[nodiscard]] Use *captured_argument_use(size_t index) noexcept;
    [[nodiscard]] const Use *captured_argument_use(size_t index) const noexcept;

    [[nodiscard]] Value *captured_argument(size_t index) noexcept;
    [[nodiscard]] const Value *captured_argument(size_t index) const noexcept;

    [[nodiscard]] size_t captured_argument_count() const noexcept;

    [[nodiscard]] RayQueryPipelineInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
