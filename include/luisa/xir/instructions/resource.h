#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

enum class ResourceQueryOp {

    // buffer query operations
    BUFFER_SIZE,     /// [(buffer) -> size]
    BYTE_BUFFER_SIZE,/// [(buffer) -> size_bytes]

    // texture query operations
    TEXTURE2D_SIZE,/// [(texture) -> Vector<uint, dim>]
    TEXTURE3D_SIZE,/// [(texture) -> Vector<uint, dim>]

    // bindless array query operations
    BINDLESS_BUFFER_SIZE,     // (bindless_array, index: uint, stride: uint) -> size: uint64
    BINDLESS_BYTE_BUFFER_SIZE,// (bindless_array, index: uint) -> size: uint64

    BINDLESS_TEXTURE2D_SIZE,      // (bindless_array, index: uint): uint2
    BINDLESS_TEXTURE3D_SIZE,      // (bindless_array, index: uint): uint3
    BINDLESS_TEXTURE2D_SIZE_LEVEL,// (bindless_array, index: uint, level: uint): uint2
    BINDLESS_TEXTURE3D_SIZE_LEVEL,// (bindless_array, index: uint, level: uint): uint3

    // texture sampling (note: we assume texture sampling is not affected by resource writes in the same shader)
    TEXTURE2D_SAMPLE,           // (tex, uv: float2, filter: uint, level: uint): float4
    TEXTURE2D_SAMPLE_LEVEL,     // (tex, uv: float2, level: float, filter: uint, level: uint): float4
    TEXTURE2D_SAMPLE_GRAD,      // (tex, uv: float2, ddx: float2, ddy: float2, filter: uint, level: uint): float4
    TEXTURE2D_SAMPLE_GRAD_LEVEL,// (tex, uv: float2, ddx: float2, ddy: float2,  mip_clamp: float, filter: uint, level: uint): float4

    TEXTURE3D_SAMPLE,           // (tex, uv: float3, filter: uint, level: uint): float4
    TEXTURE3D_SAMPLE_LEVEL,     // (tex, uv: float3, level: float, filter: uint, level: uint): float4
    TEXTURE3D_SAMPLE_GRAD,      // (tex, uv: float3, ddx: float3, ddy: float3, filter: uint, level: uint): float4
    TEXTURE3D_SAMPLE_GRAD_LEVEL,// (tex, uv: float3, ddx: float3, ddy: float3,  mip_clamp: float, filter: uint, level: uint): float4

    BINDLESS_TEXTURE2D_SAMPLE,           // (bindless_array, index: uint, uv: float2): float4
    BINDLESS_TEXTURE2D_SAMPLE_LEVEL,     // (bindless_array, index: uint, uv: float2, level: float): float4
    BINDLESS_TEXTURE2D_SAMPLE_GRAD,      // (bindless_array, index: uint, uv: float2, ddx: float2, ddy: float2): float4
    BINDLESS_TEXTURE2D_SAMPLE_GRAD_LEVEL,// (bindless_array, index: uint, uv: float2, ddx: float2, ddy: float2, mip_clamp: float): float4
    BINDLESS_TEXTURE3D_SAMPLE,           // (bindless_array, index: uint, uv: float3): float4
    BINDLESS_TEXTURE3D_SAMPLE_LEVEL,     // (bindless_array, index: uint, uv: float3, level: float): float4
    BINDLESS_TEXTURE3D_SAMPLE_GRAD,      // (bindless_array, index: uint, uv: float3, ddx: float3, ddy: float3): float4
    BINDLESS_TEXTURE3D_SAMPLE_GRAD_LEVEL,// (bindless_array, index: uint, uv: float3, ddx: float3, ddy: float3, mip_clamp: float): float4

    BINDLESS_TEXTURE2D_SAMPLE_SAMPLER,           // (bindless_array, index: uint, uv: float2, filter: uint, level: uint): float4
    BINDLESS_TEXTURE2D_SAMPLE_LEVEL_SAMPLER,     // (bindless_array, index: uint, uv: float2, level: float, filter: uint, level: uint): float4
    BINDLESS_TEXTURE2D_SAMPLE_GRAD_SAMPLER,      // (bindless_array, index: uint, uv: float2, ddx: float2, ddy: float2, filter: uint, level: uint): float4
    BINDLESS_TEXTURE2D_SAMPLE_GRAD_LEVEL_SAMPLER,// (bindless_array, index: uint, uv: float2, ddx: float2, ddy: float2,  mip_clamp: float, filter: uint, level: uint): float4
    BINDLESS_TEXTURE3D_SAMPLE_SAMPLER,           // (bindless_array, index: uint, uv: float3, filter: uint, level: uint): float4
    BINDLESS_TEXTURE3D_SAMPLE_LEVEL_SAMPLER,     // (bindless_array, index: uint, uv: float3, level: float, filter: uint, level: uint): float4
    BINDLESS_TEXTURE3D_SAMPLE_GRAD_SAMPLER,      // (bindless_array, index: uint, uv: float3, ddx: float3, ddy: float3, filter: uint, level: uint): float4
    BINDLESS_TEXTURE3D_SAMPLE_GRAD_LEVEL_SAMPLER,// (bindless_array, index: uint, uv: float3, ddx: float3, ddy: float3,  mip_clamp: float, filter: uint, level: uint): float4

    // low-level pointer operations, for akari
    BUFFER_DEVICE_ADDRESS,         // (buffer) -> address: uint64
    BINDLESS_BUFFER_DEVICE_ADDRESS,// (bindless_array, index: uint) -> address: uint64

    // ray tracing
    RAY_TRACING_INSTANCE_TRANSFORM,      // (Accel, uint)
    RAY_TRACING_INSTANCE_USER_ID,        // (Accel, uint)
    RAY_TRACING_INSTANCE_VISIBILITY_MASK,// (Accel, uint)

    RAY_TRACING_TRACE_CLOSEST,// (Accel, ray, mask: uint): TriangleHit
    RAY_TRACING_TRACE_ANY,    // (Accel, ray, mask: uint): bool
    RAY_TRACING_QUERY_ALL,    // (Accel, ray, mask: uint): RayQuery
    RAY_TRACING_QUERY_ANY,    // (Accel, ray, mask: uint): RayQuery

    // ray tracing with motion blur
    RAY_TRACING_INSTANCE_MOTION_MATRIX,// (Accel, index: uint, key: uint): float4x4
    RAY_TRACING_INSTANCE_MOTION_SRT,   // (Accel, index: uint, key: uint): SRT

    RAY_TRACING_TRACE_CLOSEST_MOTION_BLUR,// (Accel, ray, time: float, mask: uint): TriangleHit
    RAY_TRACING_TRACE_ANY_MOTION_BLUR,    // (Accel, ray, time: float, mask: uint): bool
    RAY_TRACING_QUERY_ALL_MOTION_BLUR,    // (Accel, ray, time: float, mask: uint): RayQuery
    RAY_TRACING_QUERY_ANY_MOTION_BLUR,    // (Accel, ray, time: float, mask: uint): RayQuery
};

enum class ResourceReadOp {
    BUFFER_READ,     /// [(buffer, index) -> value]: reads the index-th element in buffer
    BYTE_BUFFER_READ,/// [(buffer, byte_index) -> value]: reads the index-th element in buffer

    TEXTURE2D_READ,/// [(texture, coord) -> value]
    TEXTURE3D_READ,/// [(texture, coord) -> value]

    BINDLESS_BUFFER_READ,     // (bindless_array, index: uint, elem_index: uint) -> T
    BINDLESS_BYTE_BUFFER_READ,// (bindless_array, index: uint, offset_bytes: uint64) -> T

    BINDLESS_TEXTURE2D_READ,      // (bindless_array, index: uint, coord: uint2): float4
    BINDLESS_TEXTURE3D_READ,      // (bindless_array, index: uint, coord: uint3): float4
    BINDLESS_TEXTURE2D_READ_LEVEL,// (bindless_array, index: uint, coord: uint2, level: uint): float4
    BINDLESS_TEXTURE3D_READ_LEVEL,// (bindless_array, index: uint, coord: uint3, level: uint): float4

    DEVICE_ADDRESS_READ,// (address: uint64) -> value: T
};

enum class ResourceWriteOp {

    // buffer write operations
    BUFFER_WRITE,     /// [(buffer, index, value) -> void]: writes value into the index-th element of buffer
    BYTE_BUFFER_WRITE,/// [(buffer, byte_index, value) -> void]: writes value into the index-th element of buffer

    // texture write operations
    TEXTURE2D_WRITE,/// [(texture, coord, value) -> void]
    TEXTURE3D_WRITE,/// [(texture, coord, value) -> void]

    // bindless array write operations
    BINDLESS_BUFFER_WRITE,     // (bindless_array, index: uint, elem_index: uint, value: T) -> void
    BINDLESS_BYTE_BUFFER_WRITE,// (bindless_array, index: uint, offset_bytes: uint64, value: T) -> void

    // low-level pointer operations, for akari
    DEVICE_ADDRESS_WRITE,// (address: uint64, value: T) -> void

    // ray tracing
    RAY_TRACING_SET_INSTANCE_TRANSFORM,      // (Accel, uint, float4x4)
    RAY_TRACING_SET_INSTANCE_VISIBILITY_MASK,// (Accel, uint, uint)
    RAY_TRACING_SET_INSTANCE_OPACITY,        // (Accel, uint, bool)
    RAY_TRACING_SET_INSTANCE_USER_ID,        // (Accel, uint, uint)
    RAY_TRACING_SET_INSTANCE_MOTION_MATRIX,  // (Accel, index: uint, key: uint, transform: float4x4)
    RAY_TRACING_SET_INSTANCE_MOTION_SRT,     // (Accel, index: uint, key: uint, transform: SRT)

    // indirect dispatch
    INDIRECT_DISPATCH_SET_KERNEL,// (Buffer, uint offset, uint3 block_size, uint3 dispatch_size, uint kernel_id)
    INDIRECT_DISPATCH_SET_COUNT, // (Buffer, uint count)
};

[[nodiscard]] LC_XIR_API luisa::string_view to_string(ResourceQueryOp op) noexcept;
[[nodiscard]] LC_XIR_API luisa::string_view to_string(ResourceReadOp op) noexcept;
[[nodiscard]] LC_XIR_API luisa::string_view to_string(ResourceWriteOp op) noexcept;

[[nodiscard]] LC_XIR_API ResourceQueryOp resource_query_op_from_string(luisa::string_view name) noexcept;
[[nodiscard]] LC_XIR_API ResourceReadOp resource_read_op_from_string(luisa::string_view name) noexcept;
[[nodiscard]] LC_XIR_API ResourceWriteOp resource_write_op_from_string(luisa::string_view name) noexcept;

class LC_XIR_API ResourceQueryInst final : public DerivedInstruction<ResourceQueryInst, DerivedInstructionTag::RESOURCE_QUERY>,
                                           public InstructionOpMixin<ResourceQueryOp> {
public:
    ResourceQueryInst(BasicBlock *parent_block,
                      const Type *type, ResourceQueryOp op,
                      luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] ResourceQueryInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

class LC_XIR_API ResourceReadInst final : public DerivedInstruction<ResourceReadInst, DerivedInstructionTag::RESOURCE_READ>,
                                          public InstructionOpMixin<ResourceReadOp> {
public:
    ResourceReadInst(BasicBlock *parent_block,
                     const Type *type, ResourceReadOp op,
                     luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] ResourceReadInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

class LC_XIR_API ResourceWriteInst final : public DerivedInstruction<ResourceWriteInst, DerivedInstructionTag::RESOURCE_WRITE>,
                                           public InstructionOpMixin<ResourceWriteOp> {
public:
    ResourceWriteInst(BasicBlock *parent_block, ResourceWriteOp op,
                      luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] ResourceWriteInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
