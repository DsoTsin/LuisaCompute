#include <luisa/ast/type_registry.h>
#include <luisa/xir/function.h>
#include <luisa/xir/module.h>
#include <luisa/xir/instructions/return.h>
#include <luisa/xir/builder.h>
#include <luisa/xir/passes/reg2mem.h>
#include <luisa/xir/passes/early_return_elimination.h>

namespace luisa::compute::xir {

namespace detail {

// we start from the body block and follow the control flow merging path until we reach the final return (or not)
[[nodiscard]] static ReturnInst *find_final_return_instruction(FunctionDefinition *def) noexcept {
    for (auto block = def->body_block();;) {
        // check if the block ends with a return instruction
        auto terminator = block->terminator();
        if (terminator->isa<ReturnInst>()) { return static_cast<ReturnInst *>(terminator); }
        // check if the block ends with a control flow merge instruction, if not, we can't find the final return
        auto control_merge = terminator->control_flow_merge();
        if (control_merge == nullptr || control_merge->merge_block() == nullptr) { return nullptr; }
        // otherwise, follow the control flow merge path
        block = control_merge->merge_block();
    }
}

static void eliminate_early_return(ReturnInst *return_inst, AllocaInst *not_returned_flag) noexcept {
}

static void eliminate_early_return_in_function(Function *function, EarlyReturnEliminationInfo &info) noexcept {
    if (auto def = function->definition()) {
        auto final_return = find_final_return_instruction(def);
        auto prev_size = info.eliminated_instructions.size();
        def->traverse_basic_blocks([&](BasicBlock *block) noexcept {
            if (auto terminator = block->terminator(); terminator != final_return && terminator->isa<ReturnInst>()) {
                info.eliminated_instructions.emplace_back(static_cast<ReturnInst *>(terminator));
            }
        });
        if (auto early_returns = luisa::span{info.eliminated_instructions}.subspan(prev_size); !early_returns.empty()) {
            XIRBuilder b;
            // create a flag to indicate whether the function has *NOT* returned
            b.set_insertion_point(def->body_block()->instructions().head_sentinel());
            auto bool_type = Type::of<bool>();
            auto not_returned_flag = b.alloca_local(bool_type);
            not_returned_flag->add_comment("early return flag");
            // initialize the flag to true
            auto const_true = function->parent_module()->create_constant_one(bool_type);
            b.set_insertion_point(def->body_block()->terminator()->prev());
            auto store_inst = b.store(not_returned_flag, const_true);
            store_inst->add_comment("initialize early return flag");
            // eliminate early returns
            for (auto inst : early_returns) { eliminate_early_return(inst, not_returned_flag); }
        }
    }
}

}// namespace detail

EarlyReturnEliminationInfo early_return_elimination_pass_run_on_function(Function *function) noexcept {
    EarlyReturnEliminationInfo info;
    detail::eliminate_early_return_in_function(function, info);
    return info;
}

EarlyReturnEliminationInfo early_return_elimination_pass_run_on_module(Module *module) noexcept {
    EarlyReturnEliminationInfo info;
    for (auto &&f : module->function_list()) {
        detail::eliminate_early_return_in_function(&f, info);
    }
    return info;
}

}// namespace luisa::compute::xir
