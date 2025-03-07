#include <luisa/core/logging.h>
#include <luisa/xir/module.h>
#include <luisa/xir/function.h>
#include <luisa/xir/instructions/call.h>
#include <luisa/xir/passes/call_graph.h>
#include <luisa/xir/passes/promote_ref_arg.h>

namespace luisa::compute::xir {

namespace detail {

struct ArgumentBitmap {

    luisa::unordered_map<CallableFunction *, size_t> callable_bit_offsets;
    luisa::bitvector write_bits;// records whether an argument is written to (either by this function or by a callee)
    luisa::bitvector smem_bits; // records whether an argument might be a shared memory pointer

    void register_callable(CallableFunction *f) noexcept {
        auto offset = write_bits.size();
        if (callable_bit_offsets.try_emplace(f, offset).second) {
            write_bits.resize(offset + f->arguments().size(), false);
            smem_bits.resize(offset + f->arguments().size(), false);
        }
    }

    struct Range {

        size_t offset;
        luisa::bitvector &write_bits;
        luisa::bitvector &smem_bits;

        // returns true if changed
        [[nodiscard]] auto _mark(luisa::bitvector &bits, size_t i) const noexcept {
            if (!bits[offset + i]) {
                bits[offset + i] = true;
                return true;
            }
            return false;
        }

        [[nodiscard]] auto mark_write(size_t i) const noexcept { return _mark(write_bits, i); }
        [[nodiscard]] auto mark_smem(size_t i) const noexcept { return _mark(smem_bits, i); }
    };

    [[nodiscard]] auto operator[](CallableFunction *f) noexcept {
        auto iter = callable_bit_offsets.find(f);
        LUISA_DEBUG_ASSERT(iter != callable_bit_offsets.end(), "Callable function not found.");
        return Range{iter->second, write_bits, smem_bits};
    }
};

// checks if a function is a promotable callable, i.e., it is a callable function and all of its uses are call instructions
[[nodiscard]] static auto is_promotable_callable(Function *f) noexcept {
    if (f->isa<CallableFunction>()) {
        for (auto &&use : f->use_list()) {
            if (auto user = use.user(); user != nullptr && !user->isa<CallInst>()) {
                return false;
            }
        }
        return true;
    }
    return false;
}

static void traverse_call_graph_post_order(Function *f, const CallGraph &call_graph,
                                           const ArgumentBitmap &bitmap,
                                           luisa::unordered_set<Function *> &visited,
                                           luisa::vector<CallableFunction *> &post_order) noexcept {
    if (visited.emplace(f).second) {
        if (auto def = f->definition()) {
            auto edges = call_graph.call_edges(def);
            for (auto &&call : edges) {
                traverse_call_graph_post_order(call->callee(), call_graph, bitmap, visited, post_order);
            }
            if (def->isa<CallableFunction>() && bitmap.callable_bit_offsets.contains(static_cast<CallableFunction *>(def))) {
                post_order.emplace_back(static_cast<CallableFunction *>(def));
            }
        }
    }
}

static void promote_ref_args_in_module(Module *m, PromoteRefArgInfo &info) noexcept {
    ArgumentBitmap bitmap;
    for (auto &&f : m->function_list()) {
        if (is_promotable_callable(&f)) {
            bitmap.register_callable(static_cast<CallableFunction *>(&f));
        }
    }
    auto call_graph = compute_call_graph(m);
    luisa::vector<CallableFunction *> post_order;
    {
        post_order.reserve(bitmap.callable_bit_offsets.size());
        luisa::unordered_set<Function *> visited;
        for (auto &&f : call_graph.root_functions()) {
            traverse_call_graph_post_order(f, call_graph, bitmap, visited, post_order);
        }
    }
}

}// namespace detail

PromoteRefArgInfo promote_ref_arg_pass_run_on_module(Module *module) noexcept {
    PromoteRefArgInfo info;
    detail::promote_ref_args_in_module(module, info);
    return info;
}

}// namespace luisa::compute::xir
