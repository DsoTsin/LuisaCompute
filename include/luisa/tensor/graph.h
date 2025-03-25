#pragma once
#include <luisa/core/dll_export.h>
#include <luisa/runtime/rhi/resource.h>
namespace luisa::compute {
class TensorBuilder;
class TensorExt;
class LC_TENSOR_API Graph : public Resource {
    TensorExt *_ext;
public:
    Graph(TensorExt *ext, TensorBuilder &&builder) noexcept;
    Graph() noexcept;
    ~Graph() noexcept;
    Graph(Graph const &) = delete;
    Graph(Graph &&rhs) noexcept;
    Graph &operator=(Graph const &) = delete;
    Graph &operator=(Graph &&) noexcept;
    using Resource::operator bool;
    luisa::unique_ptr<Command> execute(std::initializer_list<ByteBufferView> tensor) noexcept;
};
}// namespace luisa::compute