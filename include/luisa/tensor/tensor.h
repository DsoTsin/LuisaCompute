#pragma once
#include <luisa/core/dll_export.h>
#include <luisa/core/stl/memory.h>
#include <luisa/core/pool.h>
#include <luisa/runtime/buffer.h>
#include <luisa/runtime/byte_buffer.h>
#include <luisa/tensor/fused_activation.h>
namespace luisa::compute {

enum struct TensorElementType : uint8_t {
    Float16,
    Float32,
    Float64,
};
class LC_TENSOR_API TensorData {
    luisa::span<size_t const> _sizes;
    TensorElementType _type;
    uint64_t _idx;
    size_t _size_bytes;

public:
    TensorData(luisa::span<size_t const> sizes,
               TensorElementType element_type,
               uint64_t uid) noexcept;
    TensorData(TensorData &&rhs) noexcept;

    TensorData(TensorData const &rhs) = delete;

    [[nodiscard]] uint64_t idx() const noexcept {
        return _idx;
    }
    [[nodiscard]] size_t get_size(uint dimension) const noexcept {
        if (dimension >= _sizes.size()) return 1;
        return _sizes[dimension];
    }
    [[nodiscard]] size_t dimension() const noexcept {
        return _sizes.size();
    }
    [[nodiscard]] size_t size_bytes() const noexcept {
        return _size_bytes;
    }
    [[nodiscard]] TensorElementType element_type() const noexcept {
        return _type;
    }
};

class TensorBuilder;

class LC_TENSOR_API Tensor {
    friend class TensorBuilder;

    TensorData *_data;
    bool _contained;

public:
    Tensor(TensorData *data,
           bool contained = true) noexcept;
    Tensor(Tensor &&rhs) noexcept;
    ~Tensor() noexcept;
    Tensor &operator=(Tensor &&rhs) noexcept {
        if (&rhs == this) [[unlikely]]
            return *this;
        std::destroy_at(this);
        new (this) Tensor(std::move(rhs));
    }
    [[nodiscard]] auto data() const noexcept { return _data; }
    void dispose() noexcept;

    [[nodiscard]] static Tensor one(TensorElementType element_type, luisa::span<const size_t> sizes) noexcept;
    [[nodiscard]] static Tensor zero(TensorElementType element_type, luisa::span<const size_t> sizes) noexcept;

    [[nodiscard]] static Tensor gemm(
        Tensor const &lhs,
        Tensor const &rhs,
        FusedActivation const &activation,
        TensorElementType out_type) noexcept;

    [[nodiscard]] static Tensor conv_1d(
        Tensor const &input,
        Tensor const &weight,
        FusedActivation const &activation,
        TensorElementType out_type,
        uint filter_radius,
        uint dilation,
        uint start_padding = std::numeric_limits<uint>::max(),
        uint end_padding = std::numeric_limits<uint>::max()) noexcept;

    [[nodiscard]] static Tensor conv_2d(
        Tensor const &input,
        Tensor const &weight,
        FusedActivation const &activation,
        TensorElementType out_type,
        uint2 filter_radius,
        uint2 dilation,
        uint2 start_padding = uint2(std::numeric_limits<uint>::max()),
        uint2 end_padding = uint2(std::numeric_limits<uint>::max())) noexcept;

    [[nodiscard]] static Tensor conv_3d(
        Tensor const &input,
        Tensor const &weight,
        FusedActivation const &activation,
        TensorElementType out_type,
        uint3 filter_radius,
        uint3 dilation,
        uint3 start_padding = uint3(std::numeric_limits<uint>::max()),
        uint3 end_padding = uint3(std::numeric_limits<uint>::max())) noexcept;
};

// class DTensor {
//     Device &device;
//     bool _requires_grad = false;
//     bool _reserve_memory = false;
//     bool _dirty = false;
//     std::array<size_t, 3> _shape;
//     std::array<size_t, 3> _stride;
//     luisa::optional<Buffer<T>> _buffer;
//     luisa::optional<Var<T>> _var;
// public:
//     using shape_type = std::array<size_t, 3>;
//     using value_type = T;

// private:
//     static size_t compute_size(shape_type s) {
//         size_t size = 1;
//         for (auto i : s) {
//             size *= i;
//         }
//         return size;
//     }
// public:

//     explicit DTensor(Device &device) : device(device) {}

//     DTensor(Device &device, Buffer<T> buffer, shape_type shape) : device(device), _buffer(buffer), _shape(shape) {
//     }
//     static DTensor zeros(Device &device, shape_type shape) noexcept {
//         auto size = compute_size(shape);
//         auto tensor = Tensor{device, device.create_buffer<T>(size), shape};
//     }
//     void fill(const T &value) noexcept {
//     }
//     [[nodiscard]] luisa::optional<Buffer<T> &> buffer() noexcept;
//     [[nodiscard]] Tensor &requires_grad(bool requires_grad) noexcept {
//         _requires_grad = requires_grad;
//         return *this;
//     }
//     [[nodiscard]] Tensor &reserve_memory(bool reserve_memory) noexcept {
//         _reserve_memory = reserve_memory;
//         return *this;
//     }
//     // inplace operations
//     void scatter_(const Tensor<uint, Dim> &index, const Tensor<T, Dim> &src) noexcept {
//     }
//     [[nodiscard]] DTensor<T> gather(const DTensor<uint> &index) const noexcept {
//         // TODO: implement
//         return DTensor<T>{device};
//     }

//     // template<size_t... Is>
//     // [[nodiscard]] Tensor<T, Dim + sizeof...(Is)> repeat(Is...) {
//     //     // TODO: implement
//     //     return Tensor<T, Dim>{device};
//     // }
// };

// template<class R, size_t Dim, class... Ts>
// Tensor<R, Dim> map(const Tensor<Ts, Dim> &... ts) noexcept {
//     // TODO: implement
// }

}// namespace luisa::compute