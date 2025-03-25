#include <luisa/tensor/pass/expr_topo.h>
#include <luisa/core/stl/pdqsort.h>
#include <luisa/vstl/vector.h>
namespace luisa::compute {
void ExprTopo::mark_depend(
    TensorExpr *depended,
    TensorExpr *depending) noexcept {
    _expr_depends[depending->idx()].depending_count++;
    _expr_depends[depended->idx()].depending_self_exprs.emplace_back(depending);
}

ExprTopo::ExprTopo(luisa::span<TensorExpr *const> exprs, size_t tensor_count) noexcept {
    _expr_depends.resize(exprs.size());
    for (size_t i = 0; i < exprs.size(); ++i) {
        _expr_depends[i].self = exprs[i];
    }
    _tensor_depends.resize(tensor_count);
    // Mark all tensor
    for (auto &i : exprs) {
        auto callback = [&](TensorData *data, Usage usage) {
            auto &tensor_dep = _tensor_depends[data->idx()];
            tensor_dep.depend_exprs.emplace_back(i, usage);
        };
        i->get_tensors(vstd::make_func_ref(callback));
    }
    // iterate tensors to mark depend
    for (auto &i : _tensor_depends) {
        TensorExpr *last_write_expr = nullptr;
        TensorExpr *last_rw_expr = nullptr;
        for (auto &dep : i.depend_exprs) {
            if (dep.second == Usage::READ) {
                if (last_write_expr) {
                    mark_depend(last_write_expr, dep.first);
                }
            } else {
                if (last_rw_expr) {
                    mark_depend(last_rw_expr, dep.first);
                }
                last_write_expr = dep.first;
            }
            last_rw_expr = dep.first;
        }
    }
}

ExprTopo::ExprTopo() noexcept = default;

luisa::vector<TensorExpr *> ExprTopo::topo_sort(
    luisa::span<TensorExpr *const> exprs, size_t tensor_count) noexcept {
    ExprTopo topo(exprs, tensor_count);
    return topo._topo_sort();
}

luisa::vector<TensorExpr *> ExprTopo::_topo_sort() noexcept {
    luisa::vector<TensorExpr *> r;
    luisa::fixed_vector<ExprDependency *, 16> stack;
    r.reserve(_expr_depends.size());
    for (auto &i : _expr_depends) {
        if (i.depending_count == 0) {
            stack.emplace_back(&i);
        }
        auto self_tag = static_cast<int32_t>(i.self->tag());
        // Let same tag stay together
        pdqsort(
            i.depending_self_exprs.begin(),
            i.depending_self_exprs.end(),
            [&](TensorExpr *a, TensorExpr *b) {
                auto a_tag = static_cast<int32_t>(a->tag()) - self_tag;
                auto b_tag = static_cast<int32_t>(b->tag()) - self_tag;
                if (a_tag < 0) {
                    a_tag += 10000;
                }
                if (b_tag < 0) {
                    b_tag += 10000;
                }
                return a_tag < b_tag ? true : (a_tag == b_tag && a->idx() < b->idx());
            });
    }
    if (stack.empty()) [[unlikely]] {
        LUISA_ERROR("Circular depend.");
    }

    while (!stack.empty()) {
        auto last = stack.back();
        stack.pop_back();
        r.emplace_back(last->self);
        for (auto expr : last->depending_self_exprs) {
            auto &expr_dep = _expr_depends[expr->idx()];
            if (--expr_dep.depending_count <= 0) {
                stack.emplace_back(&expr_dep);
            }
        }
    }
    return r;
}

ExprTopo::~ExprTopo() noexcept {}
}// namespace luisa::compute