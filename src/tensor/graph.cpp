#include <luisa/tensor/graph.h>
#include <luisa/tensor/ext.h>
#include <luisa/tensor/tensor_builder.h>

namespace luisa::compute {
Graph::Graph(TensorExt *ext, TensorBuilder &&builder) noexcept
    : Resource(
          ext->device(),
          Resource::Tag::TENSOR_GRAPH,
          ext->create_graph(std::move(builder))),
      _ext(ext) {}
Graph::Graph() noexcept = default;
Graph::~Graph() noexcept {
    if (*this) {
        static_cast<TensorExt *>(device()->extension(TensorExt::name))->destroy_graph(handle());
    }
}
Graph &Graph::operator=(Graph &&rhs) noexcept {
    _move_from(std::move(rhs));
    return *this;
}
luisa::unique_ptr<Command> Graph::execute(std::initializer_list<ByteBufferView> tensor) noexcept {
    luisa::vector<uint64_t> handles;
    handles.reserve(tensor.size());
    for (auto &i : tensor) {
        handles.emplace_back(i.handle());
    }
    return _ext->execute(handle(), handles);
}
Graph::Graph(Graph &&rhs) noexcept
    : Resource(std::move(rhs)) {
    rhs._ext = nullptr;
}

}// namespace luisa::compute