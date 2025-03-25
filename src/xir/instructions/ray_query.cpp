#include <luisa/core/logging.h>
#include <luisa/xir/basic_block.h>
#include <luisa/xir/function.h>
#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/ray_query.h>

namespace luisa::compute::xir {

RayQueryObjectReadInst::RayQueryObjectReadInst(BasicBlock *parent_block, const Type *type,
                                               RayQueryObjectReadOp op, luisa::span<Value *const> operands) noexcept
    : Super{op, parent_block, type} { set_operands(operands); }

RayQueryObjectReadInst *RayQueryObjectReadInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    luisa::fixed_vector<Value *, 16u> resolved_ops;
    resolved_ops.reserve(operand_count());
    for (auto op_use : operand_uses()) {
        resolved_ops.emplace_back(resolver.resolve(op_use->value()));
    }
    return b.call(type(), op(), resolved_ops);
}

RayQueryObjectWriteInst::RayQueryObjectWriteInst(BasicBlock *parent_block, RayQueryObjectWriteOp op,
                                                 luisa::span<Value *const> operands) noexcept
    : Super{op, parent_block, nullptr} { set_operands(operands); }

RayQueryObjectWriteInst *RayQueryObjectWriteInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    luisa::fixed_vector<Value *, 16u> resolved_ops;
    resolved_ops.reserve(operand_count());
    for (auto op_use : operand_uses()) {
        resolved_ops.emplace_back(resolver.resolve(op_use->value()));
    }
    return b.call(op(), resolved_ops);
}

RayQueryLoopInst::RayQueryLoopInst(BasicBlock *parent_block) noexcept : Super{parent_block} {
    auto dispatch_block = static_cast<Value *>(nullptr);
    auto operands = std::array{dispatch_block};
    set_operands(operands);
}

void RayQueryLoopInst::set_dispatch_block(BasicBlock *block) noexcept {
    set_operand(operand_index_dispatch_block, block);
}

BasicBlock *RayQueryLoopInst::create_dispatch_block() noexcept {
    auto block = parent_function()->create_basic_block();
    set_dispatch_block(block);
    return block;
}

BasicBlock *RayQueryLoopInst::dispatch_block() noexcept {
    return static_cast<BasicBlock *>(operand(operand_index_dispatch_block));
}

const BasicBlock *RayQueryLoopInst::dispatch_block() const noexcept {
    return const_cast<RayQueryLoopInst *>(this)->dispatch_block();
}

RayQueryLoopInst *RayQueryLoopInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto cloned = b.ray_query_loop();
    auto resolved_dispatch = resolver.resolve(dispatch_block());
    LUISA_DEBUG_ASSERT(resolved_dispatch == nullptr || resolved_dispatch->isa<BasicBlock>(), "Invalid dispatch block.");
    auto resolved_merge = resolver.resolve(merge_block());
    LUISA_DEBUG_ASSERT(resolved_merge == nullptr || resolved_merge->isa<BasicBlock>(), "Invalid merge block.");
    cloned->set_dispatch_block(static_cast<BasicBlock *>(resolved_dispatch));
    cloned->set_merge_block(static_cast<BasicBlock *>(resolved_merge));
    return cloned;
}

RayQueryDispatchInst::RayQueryDispatchInst(BasicBlock *parent_block, Value *query_object) noexcept : Super{parent_block} {
    auto exit_block = static_cast<Value *>(nullptr);
    auto on_surface_candidate_block = static_cast<Value *>(nullptr);
    auto on_procedural_candidate_block = static_cast<Value *>(nullptr);
    auto operands = std::array{query_object, exit_block, on_surface_candidate_block, on_procedural_candidate_block};
    LUISA_DEBUG_ASSERT(operands[operand_index_query_object] == query_object, "Invalid query object operand.");
    set_operands(operands);
}

void RayQueryDispatchInst::set_query_object(Value *query_object) noexcept {
    set_operand(operand_index_query_object, query_object);
}

void RayQueryDispatchInst::set_exit_block(BasicBlock *block) noexcept {
    set_operand(operand_index_exit_block, block);
}

void RayQueryDispatchInst::set_on_surface_candidate_block(BasicBlock *block) noexcept {
    set_operand(operand_index_on_surface_candidate_block, block);
}

void RayQueryDispatchInst::set_on_procedural_candidate_block(BasicBlock *block) noexcept {
    set_operand(operand_index_on_procedural_candidate_block, block);
}

BasicBlock *RayQueryDispatchInst::create_on_surface_candidate_block() noexcept {
    auto block = parent_function()->create_basic_block();
    set_on_surface_candidate_block(block);
    return block;
}

BasicBlock *RayQueryDispatchInst::create_on_procedural_candidate_block() noexcept {
    auto block = parent_function()->create_basic_block();
    set_on_procedural_candidate_block(block);
    return block;
}

Value *RayQueryDispatchInst::query_object() noexcept {
    return operand(operand_index_query_object);
}

const Value *RayQueryDispatchInst::query_object() const noexcept {
    return operand(operand_index_query_object);
}

BasicBlock *RayQueryDispatchInst::exit_block() noexcept {
    return static_cast<BasicBlock *>(operand(operand_index_exit_block));
}

const BasicBlock *RayQueryDispatchInst::exit_block() const noexcept {
    return const_cast<RayQueryDispatchInst *>(this)->exit_block();
}

BasicBlock *RayQueryDispatchInst::on_surface_candidate_block() noexcept {
    return static_cast<BasicBlock *>(operand(operand_index_on_surface_candidate_block));
}

const BasicBlock *RayQueryDispatchInst::on_surface_candidate_block() const noexcept {
    return const_cast<RayQueryDispatchInst *>(this)->on_surface_candidate_block();
}

BasicBlock *RayQueryDispatchInst::on_procedural_candidate_block() noexcept {
    return static_cast<BasicBlock *>(operand(operand_index_on_procedural_candidate_block));
}

const BasicBlock *RayQueryDispatchInst::on_procedural_candidate_block() const noexcept {
    return const_cast<RayQueryDispatchInst *>(this)->on_procedural_candidate_block();
}

RayQueryDispatchInst *RayQueryDispatchInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_query_object = resolver.resolve(query_object());
    auto cloned = b.ray_query_dispatch(resolved_query_object);
    auto resolved_exit = resolver.resolve(exit_block());
    LUISA_DEBUG_ASSERT(resolved_exit == nullptr || resolved_exit->isa<BasicBlock>(), "Invalid exit block.");
    auto resolved_on_surface = resolver.resolve(on_surface_candidate_block());
    LUISA_DEBUG_ASSERT(resolved_on_surface == nullptr || resolved_on_surface->isa<BasicBlock>(), "Invalid on surface candidate block.");
    auto resolved_on_procedural = resolver.resolve(on_procedural_candidate_block());
    LUISA_DEBUG_ASSERT(resolved_on_procedural == nullptr || resolved_on_procedural->isa<BasicBlock>(), "Invalid on procedural candidate block.");
    cloned->set_exit_block(static_cast<BasicBlock *>(resolved_exit));
    cloned->set_on_surface_candidate_block(static_cast<BasicBlock *>(resolved_on_surface));
    cloned->set_on_procedural_candidate_block(static_cast<BasicBlock *>(resolved_on_procedural));
    return cloned;
}

RayQueryPipelineInst::RayQueryPipelineInst(BasicBlock *parent_block, Value *query_object,
                                           Function *on_surface, Function *on_procedural,
                                           luisa::span<Value *const> captured_args) noexcept
    : Super{parent_block, nullptr} {
    std::array operands{query_object, static_cast<Value *>(on_surface), static_cast<Value *>(on_procedural)};
    LUISA_DEBUG_ASSERT(operands[operand_index_query_object] == query_object, "Invalid query object operand.");
    LUISA_DEBUG_ASSERT(operands[operand_index_on_surface_function] == on_surface, "Invalid on surface function operand.");
    LUISA_DEBUG_ASSERT(operands[operand_index_on_procedural_function] == on_procedural, "Invalid on procedural function operand.");
    set_operands(operands);
    if (!captured_args.empty()) {
        set_captured_arguments(captured_args);
    }
}

RayQueryPipelineInst *RayQueryPipelineInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_query_object = resolver.resolve(query_object());
    auto resolved_on_surface = resolver.resolve(on_surface_function());
    LUISA_DEBUG_ASSERT(resolved_on_surface == nullptr || resolved_on_surface->isa<Function>(), "Invalid on surface function.");
    auto resolved_on_procedural = resolver.resolve(on_procedural_function());
    LUISA_DEBUG_ASSERT(resolved_on_procedural == nullptr || resolved_on_procedural->isa<Function>(), "Invalid on procedural function.");
    luisa::fixed_vector<Value *, 16u> resolved_args;
    resolved_args.reserve(captured_argument_count());
    for (auto arg_use : captured_argument_uses()) {
        resolved_args.emplace_back(resolver.resolve(arg_use->value()));
    }
    return b.ray_query_pipeline(resolved_query_object,
                                static_cast<Function *>(resolved_on_surface),
                                static_cast<Function *>(resolved_on_procedural),
                                resolved_args);
}

void RayQueryPipelineInst::set_query_object(Value *query_object) noexcept {
    set_operand(operand_index_query_object, query_object);
}

void RayQueryPipelineInst::set_on_surface_function(Function *on_surface) noexcept {
    set_operand(operand_index_on_surface_function, on_surface);
}

void RayQueryPipelineInst::set_on_procedural_function(Function *on_procedural) noexcept {
    set_operand(operand_index_on_procedural_function, on_procedural);
}

void RayQueryPipelineInst::set_captured_argument(size_t index, Value *arg) noexcept {
    set_operand(operand_index_offset_captured_arguments + index, arg);
}

void RayQueryPipelineInst::add_captured_argument(Value *arg) noexcept {
    add_operand(arg);
}

void RayQueryPipelineInst::set_captured_arguments(luisa::span<Value *const> args) noexcept {
    set_captured_argument_count(args.size());
    for (auto i = 0u; i < args.size(); i++) {
        set_captured_argument(i, args[i]);
    }
}

void RayQueryPipelineInst::set_captured_argument_count(size_t count) noexcept {
    set_operand_count(operand_index_offset_captured_arguments + count);
}

luisa::span<Use *const> RayQueryPipelineInst::captured_argument_uses() noexcept {
    return operand_uses().subspan(operand_index_offset_captured_arguments);
}

luisa::span<const Use *const> RayQueryPipelineInst::captured_argument_uses() const noexcept {
    return const_cast<RayQueryPipelineInst *>(this)->captured_argument_uses();
}

Use *RayQueryPipelineInst::captured_argument_use(size_t index) noexcept {
    return operand_use(operand_index_offset_captured_arguments + index);
}

const Use *RayQueryPipelineInst::captured_argument_use(size_t index) const noexcept {
    return const_cast<RayQueryPipelineInst *>(this)->captured_argument_use(index);
}

Value *RayQueryPipelineInst::captured_argument(size_t index) noexcept {
    return operand(operand_index_offset_captured_arguments + index);
}

const Value *RayQueryPipelineInst::captured_argument(size_t index) const noexcept {
    return const_cast<RayQueryPipelineInst *>(this)->captured_argument(index);
}

size_t RayQueryPipelineInst::captured_argument_count() const noexcept {
    auto op_count = operand_count();
    LUISA_DEBUG_ASSERT(op_count >= operand_index_offset_captured_arguments, "Invalid captured argument count.");
    return op_count - operand_index_offset_captured_arguments;
}

Value *RayQueryPipelineInst::query_object() noexcept {
    return operand(operand_index_query_object);
}

const Value *RayQueryPipelineInst::query_object() const noexcept {
    return operand(operand_index_query_object);
}

Function *RayQueryPipelineInst::on_surface_function() noexcept {
    auto func = operand(operand_index_on_surface_function);
    LUISA_DEBUG_ASSERT(func->isa<Function>(), "Invalid on surface function operand.");
    return static_cast<Function *>(func);
}

const Function *RayQueryPipelineInst::on_surface_function() const noexcept {
    return const_cast<RayQueryPipelineInst *>(this)->on_surface_function();
}

Function *RayQueryPipelineInst::on_procedural_function() noexcept {
    auto func = operand(operand_index_on_procedural_function);
    LUISA_DEBUG_ASSERT(func->isa<Function>(), "Invalid on procedural function operand.");
    return static_cast<Function *>(func);
}

const Function *RayQueryPipelineInst::on_procedural_function() const noexcept {
    return const_cast<RayQueryPipelineInst *>(this)->on_procedural_function();
}

}// namespace luisa::compute::xir
