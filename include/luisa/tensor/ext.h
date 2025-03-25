#pragma once
#include <luisa/runtime/rhi/device_interface.h>

namespace luisa::compute {
class TensorBuilder;
class TensorExt : public DeviceExtension {
public:
    static constexpr luisa::string_view name = "TensorExt";
    [[nodiscard]] virtual ResourceCreationInfo create_graph(TensorBuilder &&builder) noexcept = 0;
    [[nodiscard]] virtual void destroy_graph(uint64_t handle) noexcept = 0;
    [[nodiscard]] virtual luisa::unique_ptr<Command> execute(
        uint64_t graph_handle,
        luisa::span<uint64_t const> resources) noexcept;
    [[nodiscard]] virtual DeviceInterface *device() const noexcept = 0;
    ~TensorExt() noexcept = default;
};
}// namespace luisa::compute