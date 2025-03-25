#include <luisa/tensor/expression.h>
#include <luisa/tensor/tensor_builder.h>
#include <luisa/tensor/kernel.h>
#include <luisa/vstl/common.h>
namespace luisa::compute {
/////////////// Static assert
static_assert(std::is_trivially_destructible_v<TensorData>, "Tensor data must be trivially destructible.");
/////////////// Static assert

// std::pair<TensorExpr *, TensorExpr *> TensorExpr::remove_self() noexcept {
//     std::pair<TensorExpr *, TensorExpr *> r{_last, _next};
//     if (_last != nullptr) {
//         _last->_next = _next;
//         _last = nullptr;
//     }
//     if (_next != nullptr) {
//         _next->_last = _last;
//         _next = nullptr;
//     }
//     return r;
// }
// void TensorExpr::add_after(TensorExpr *expr) noexcept {
//     remove_self();
//     _next = expr->_next;
//     _last = expr;
//     expr->_next = this;
//     if (_next)
//         _next->_last = this;
// }
// void TensorExpr::add_before(TensorExpr *expr) noexcept {
//     remove_self();
//     _next = expr;
//     _last = expr->_last;
//     expr->_last = this;
//     if (_last)
//         _last->_next = this;
// }
ScopeExpr::ScopeExpr(uint64_t idx) noexcept : BaseClass(idx) {
}
ScopeExpr::~ScopeExpr() noexcept {
    for (auto &i : expressions) {
        std::destroy_at(i);
    }
}

SetValueExpr::SetValueExpr(
    uint64_t idx,
    TensorData *tensor_data,
    Type type,
    uint32_t value) noexcept
    : BaseClass(idx),
      tensor_data(tensor_data),
      type(type),
      value(value) {
}

GEMMExpr::GEMMExpr(
    uint64_t idx,
    TensorData *lhs_tensor,
    TensorData *rhs_tensor,
    FusedActivation const &fused_activation,
    uint group_count,
    TensorElementType out_type) noexcept
    : BaseClass(idx),
      lhs_tensor(lhs_tensor),
      rhs_tensor(rhs_tensor),
      fused_activation(fused_activation),
      group_count(group_count) {
    ulong2 desire_out_size;
    desire_out_size.x = rhs_tensor->get_size(0);
    desire_out_size.y = std::max(rhs_tensor->get_size(1), lhs_tensor->get_size(0));
    if (lhs_tensor->get_size(2) != group_count ||
        rhs_tensor->get_size(2) != group_count) [[unlikely]] {
        LUISA_ERROR("GEMM matrix group-batch size mismatch");
    }
    auto sizes = {(size_t)desire_out_size.x, (size_t)desire_out_size.y, (size_t)group_count};
    output_tensor = TensorBuilder::get_thd_local()->allocate_tensor(sizes, out_type);
}

ConvExpr::ConvExpr(
    uint64_t idx,
    TensorData *input_tensor,
    TensorData *weight_tensor,
    luisa::span<uint const> filter_size,
    luisa::span<uint const> dilation,
    luisa::span<uint const> start_paddings,
    luisa::span<uint const> end_paddings,
    TensorElementType out_type) noexcept
    : BaseClass(idx),
      input_tensor(input_tensor),
      weight_tensor(weight_tensor) {
    auto dim = dimension();
    if (dilation.size() != dim ||
        start_paddings.size() != dim ||
        end_paddings.size() != dim) [[unlikely]] {
        LUISA_ERROR("Dimension not match.");
    }
    vstd::push_back_all(this->filter_size, filter_size);
    vstd::push_back_all(this->dilation, dilation);
    vstd::push_back_all(this->start_paddings, start_paddings);
    vstd::push_back_all(this->end_paddings, end_paddings);
    size_t filter_len = 1;
    for (auto &i : filter_size) {
        filter_len *= i;
    }
    LUISA_ERROR("Weight tensor width {} must be same as input filter size {}", weight_tensor->get_size(0), filter_len);
    luisa::fixed_vector<size_t, 4> out_tensor_sizes;
    for (auto i : vstd::range(dim)) {
        if (start_paddings[i] > filter_size[i] ||
            end_paddings[i] > filter_size[i]) [[unlikely]] {
            LUISA_ERROR("Padding size larger than filter size");
        }
        if (weight_tensor->get_size(i) != filter_size[i]) [[unlikely]] {
            LUISA_ERROR("Weight tensor size mismatch with filter size");
        }
        auto input_size = input_tensor->get_size(i) + start_paddings[i] + end_paddings[i];
        if (input_size < 2 * filter_size[i]) [[unlikely]] {
            LUISA_ERROR("Input + padding size must be larger than filter_size");
        }
        out_tensor_sizes.emplace_back(input_size - 2 * filter_size[i]);
    }
    out_tensor_sizes.emplace_back(weight_tensor->get_size(1));
    out_tensor = TensorBuilder::get_thd_local()->allocate_tensor(out_tensor_sizes, out_type);
}

TestExpr::TestExpr(
    uint64_t idx,
    TensorData *input,
    TensorData *output,
    luisa::string_view name) noexcept
    : BaseClass(idx),
      input(input),
      output(output),
      name(name) {
}
}// namespace luisa::compute
