/**
 * @file test_soa.cpp
 * @brief The SOA test case
 * @author sailing-innocent
 * @date 2025-03-07
 */
#include "common/config.h"
#include <luisa/runtime/device.h>
#include <luisa/runtime/stream.h>
#include <luisa/runtime/buffer.h>
#include <luisa/dsl/sugar.h>

#include <random>
#include <cmath>
using namespace luisa;
using namespace luisa::compute;

namespace luisa::test {
struct SampleSOA {
    int a;
    float b;
};

}// namespace luisa::test

LUISA_STRUCT(luisa::test::SampleSOA, a, b) {};

namespace luisa::test {

template<int N>
bool test_soa_element(Device& device) {
    // Element-wise SOA test
    auto soa = device.create_soa<SampleSOA>(N);
    luisa::vector<int> host_a(N);
    luisa::vector<float> host_b(N);
    std::mt19937 engine{std::random_device{}()};

    auto rand = [](auto& engine) noexcept {
        std::uniform_real_distribution<float> dist{0.0f, 10.0f};
        return SampleSOA{static_cast<int>(std::ceil(dist(engine))), dist(engine)};
    };
    for (auto i = 0u; i < N; i++) {
        auto sample = rand(engine);
        host_a[i] = sample.a;
        host_b[i] = sample.b;
    }
    // initialize with element
    auto stream = device.create_stream();
    auto soa_view = soa.view();

    LUISA_INFO("soa.a.element_offset() = {}", soa.a.element_offset());
    LUISA_INFO("soa.a.element_size() = {}", soa.a.element_size());
    LUISA_INFO("soa.a.soa_offset() = {}", soa.a.soa_offset());
    LUISA_INFO("soa.b.element_offset() = {}", soa.b.element_offset());

    stream << soa_view.a.buffer().subview(
        soa.a.soa_offset() + (soa.a.element_offset() + 0u) * soa.a.element_stride,
        soa.a.element_size()
    ).as<int>().copy_from(host_a.data())
           << soa_view.b.buffer().subview(
            soa.b.soa_offset() + (soa.b.element_offset() + 0u) * soa.b.element_stride,
            soa.b.element_size()
           ).as<float>().copy_from(host_b.data())
           << synchronize();

    // read back
    luisa::vector<int> host_a_download(N);
    luisa::vector<float> host_b_download(N);



    stream << soa_view.a.buffer().subview(
        soa.a.soa_offset() + (soa.a.element_offset() + 0u) * soa.a.element_stride,
        soa.a.element_size()
    ).as<int>().copy_to(host_a_download.data())
           << soa_view.b.buffer().subview(
            soa.b.soa_offset() + (soa.b.element_offset() + 0u) * soa.b.element_stride,
            soa.b.element_size()
           ).as<float>().copy_to(host_b_download.data())
           << synchronize();

    for (auto i = 0u; i < N; i++) {
        CHECK_MESSAGE(host_a[i] == doctest::Approx(host_a_download[i]), "SOA element-wise upload/download mismatch at index {}\n Expected: {}\n  Actual:   {}", i, host_a[i], host_a_download[i]);
        CHECK_MESSAGE(host_b[i] == doctest::Approx(host_b_download[i]), "SOA element-wise upload/download mismatch at index {}\n Expected: {}\n  Actual:   {}", i, host_b[i], host_b_download[i]);
    }

    return true;
}


template<int N>
bool test_soa_simple(Device &device) {
    auto soa = device.create_soa<float3>(N);
    auto rand = [](auto &engine) noexcept {
        std::uniform_real_distribution<float> dist{0.0f, 1.0f};
        return float3{dist(engine), dist(engine), dist(engine)};
    };
    luisa::vector<float3> host_upload(N);
    std::mt19937 engine{std::random_device{}()};
    for (auto i = 0u; i < N; i++) { host_upload[i] = rand(engine); }
    auto buffer_upload = device.create_buffer<float3>(N);
    auto buffer_download = device.create_buffer<float3>(N);
    auto stream = device.create_stream();
    auto shader_upload = device.compile<1u>([](SOAVar<float3> soa, BufferVar<float3> upload) noexcept {
        auto i = dispatch_x();
        soa.write(i, upload.read(i));
    });
    auto shader_download = device.compile<1u>([](SOAVar<float3> soa, BufferVar<float3> download) noexcept {
        auto i = dispatch_x();
        download.write(i, soa.read(i));
    });

    luisa::vector<float3> host_download(N);
    stream << buffer_upload.copy_from(host_upload.data())
           << shader_upload(soa, buffer_upload).dispatch(N)
           << shader_download(soa, buffer_download).dispatch(N)
           << buffer_download.copy_to(host_download.data())
           << synchronize();

    for (auto i = 0u; i < N; i++) {
        for (auto j = 0u; j < 3u; j++)
            CHECK_MESSAGE(host_upload[i][j] == doctest::Approx(host_download[i][j]), "SOA upload/download mismatch at index {}\n Expected: {}\n  Actual:   {}", i, host_upload[i], host_download[i]);
    }
    return true;
}

}// namespace luisa::test

TEST_SUITE("dsl") {
    LUISA_TEST_CASE_WITH_DEVICE("dsl_soa_simple", 
        (luisa::test::test_soa_simple<1024>(device)
        & luisa::test::test_soa_simple<2048>(device)
        & luisa::test::test_soa_simple<4096>(device))
    );

    LUISA_TEST_CASE_WITH_DEVICE("dsl_soa_element", 
        (luisa::test::test_soa_element<1024>(device)
        & luisa::test::test_soa_element<2048>(device)
        & luisa::test::test_soa_element<4096>(device))
    );
}