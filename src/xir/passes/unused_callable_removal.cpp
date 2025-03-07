#include <luisa/xir/function.h>
#include <luisa/xir/module.h>
#include <luisa/xir/passes/unused_callable_removal.h>

namespace luisa::compute::xir {

namespace detail {

static void collect_reachable_callables(Function *f, luisa::unordered_set<Function *> &reachable) noexcept {
    if (reachable.emplace(f).second) {
        if (auto def = f->definition()) {
            def->traverse_instructions([&](Instruction *inst) noexcept {
                for (auto &&op_use : inst->operand_uses()) {
                    if (auto op = op_use->value(); op != nullptr && op->isa<Function>()) {
                        collect_reachable_callables(static_cast<Function *>(op), reachable);
                    }
                }
            });
        }
    }
}

}// namespace detail

UnusedCallableRemovalInfo unused_callable_removal_pass_run_on_module(Module *module) noexcept {
    luisa::unordered_set<Function *> reachable;
    for (auto &&f : module->function_list()) {
        if (f.isa<KernelFunction>()) {
            detail::collect_reachable_callables(&f, reachable);
        }
    }
    UnusedCallableRemovalInfo info;
    for (auto &&f : module->function_list()) {
        if (f.isa<CallableFunction>() && !reachable.contains(&f)) {
            info.removed_callable_functions.emplace_back(static_cast<CallableFunction *>(&f));
        }
    }
    for (auto f : info.removed_callable_functions) {
        f->remove_self();
    }
    return info;
}

}// namespace luisa::compute::xir
