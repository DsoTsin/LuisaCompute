#include <luisa/core/logging.h>
#include <luisa/xir/module.h>
#include <luisa/xir/function.h>
#include <luisa/xir/instructions/call.h>
#include <luisa/xir/passes/call_graph.h>

namespace luisa::compute::xir {

inline void CallGraph::_add_function(Function *f) noexcept {
    auto any_caller = false;
    for (auto &&use : f->use_list()) {
        if (auto user = use.user(); user != nullptr && user->isa<CallInst>()) {
            auto call = static_cast<CallInst *>(user);
            auto caller = call->parent_function()->definition();
            LUISA_DEBUG_ASSERT(caller != nullptr, "Invalid caller.");
            _call_edges[caller].emplace_back(call);
            any_caller = true;
        }
    }
    if (!any_caller) { _root_functions.emplace_back(f); }
}

luisa::span<Function *const> CallGraph::root_functions() const noexcept {
    return luisa::span{_root_functions};
}

luisa::span<CallInst *const> CallGraph::call_edges(FunctionDefinition *f) const noexcept {
    auto iter = _call_edges.find(f);
    return iter == _call_edges.cend() ? luisa::span<CallInst *const>{} : luisa::span{iter->second};
}

CallGraph compute_call_graph(Module *module) noexcept {
    CallGraph graph;
    for (auto &&f : module->function_list()) { graph._add_function(&f); }
    return graph;
}

}// namespace luisa::compute::xir
