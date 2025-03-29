#include <luisa/tensor/tensor.h>
#include <luisa/tensor/tensor_builder.h>
#include <luisa/tensor/expression.h>
#include <luisa/core/logging.h>

namespace luisa::compute {
TensorData::TensorData(luisa::span<size_t const> sizes,
                       TensorElementType element_type,
                       uint64_t uid) noexcept
    : _type(element_type),
      _idx(uid) {
    _size_bytes = 0;
    switch (_type) {
        case TensorElementType::Float16:
            _size_bytes = 2;
            break;
        case TensorElementType::Float32:
            _size_bytes = 4;
            break;
        case TensorElementType::Float64:
            _size_bytes = 8;
            break;
    }
    auto new_sizes = TensorBuilder::get_thd_local()->allocate_array<size_t>(sizes.size());
    for (size_t idx = 0; idx != sizes.size(); ++idx) {
        new_sizes[idx] = sizes[idx];
        _size_bytes *= sizes[idx];
    }
    _sizes = luisa::span<size_t const>{new_sizes, sizes.size()};
}

TensorData::TensorData(TensorData &&rhs) noexcept = default;
namespace tensor_builder_detail {
static thread_local TensorBuilder *tensor_builder{nullptr};
}// namespace tensor_builder_detail
Tensor::Tensor(TensorData *data,
               bool contained) noexcept
    : _data(data), _contained(contained) {
}
Tensor::Tensor(Tensor &&rhs) noexcept
    : _data(rhs._data),
      _contained(rhs._contained) {
    rhs._contained = false;
}

void Tensor::dispose() noexcept {
    if (_contained) {
        tensor_builder_detail::tensor_builder->deallocate_tensor(_data);
        _contained = false;
    }
    _data = nullptr;
}

Tensor::~Tensor() {
    dispose();
}

void TensorBuilder::deallocate_tensor(TensorData *tensor) noexcept {
    // TODO: record deallocate
}
TensorData *TensorBuilder::allocate_tensor(
    luisa::span<size_t const> sizes,
    TensorElementType element_type) noexcept {
    // TODO: record allocate
    auto id = _allocated_tensor.size();
    auto ptr = _tensor_pool.create(
        sizes,
        element_type,
        id);
    _allocated_absolute_size += ptr->size_bytes();
    _allocated_tensor.emplace_back(ptr);
    return ptr;
}
TensorBuilder::TensorBuilder() noexcept : _root_expr(~0ull) {
    _expr_stack.emplace_back(&_root_expr);
}
void TensorBuilder::push_scope() noexcept {
    auto last = _expr_stack.back();
     _expr_stack.emplace_back(last->allocate_expr<ScopeExpr>());
}
void TensorBuilder::pop_scope() noexcept {
    LUISA_ASSERT(_expr_stack.size() > 1, "Invalid scope pop.");
    _expr_stack.pop_back();
}
TensorBuilder::~TensorBuilder() noexcept {
    LUISA_ASSERT(_expr_stack.size() == 1, "Un-poped scope lefted.");
}
void *TensorBuilder::allocate_stack(size_t size_bytes, size_t alignment) noexcept {
    for (auto &i : _stack_allocator) {
        auto aligned_offset = (i.offset + alignment - 1ull) & (~(alignment - 1ull));
        if (i.size - aligned_offset >= size_bytes) {
            auto result = i.ptr.get() + aligned_offset;
            i.offset = aligned_offset + size_bytes;
            return result;
        }
    }
    size_t init_size = 65536ull;
    if (!_stack_allocator.empty()) {
        init_size = _stack_allocator.back().size * 2;
    }
    return _stack_allocator
        .emplace_back(
            static_cast<std::byte *>(luisa::detail::allocator_allocate(init_size, 0)),
            init_size,
            size_bytes)
        .ptr.get();
}

TensorBuilder *TensorBuilder::get_thd_local() {
    return tensor_builder_detail::tensor_builder;
}
void TensorBuilder::set_thd_local(TensorBuilder *builder) {
    tensor_builder_detail::tensor_builder = builder;
}

}// namespace luisa::compute