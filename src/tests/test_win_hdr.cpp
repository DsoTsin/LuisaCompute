#include <luisa/core/clock.h>
#include <luisa/runtime/device.h>
#include <luisa/runtime/context.h>
#include <luisa/runtime/shader.h>
#include <luisa/runtime/stream.h>
#include <luisa/gui/window.h>
#include <luisa/dsl/sugar.h>
#include <luisa/backends/ext/dx_hdr_ext.hpp>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
using namespace luisa;
using namespace luisa::compute;
int main(int argc, char *argv[]) {
    Context context{argv[0]};
    Device device = context.create_device("dx");
    auto image_width = 0;
    auto image_height = 0;
    auto image_channels = 0;
    auto image_pixels = stbi_load("genshin_start.jpg", &image_width, &image_height, &image_channels, 4);

    auto texture = device.create_image<float>(PixelStorage::BYTE4, uint2(image_width, image_height), 6u);
    Stream stream = device.create_stream(StreamTag::GRAPHICS);
    const uint2 resolution{(uint)image_width, (uint)image_height};
    Window window{"path tracing", resolution};
    float white_point{1.0f};
    float scale = 1.0f;
    auto dx_hdr_ext = device.extension<DXHDRExt>();
    bool use_hdr10 = true;
    if (dx_hdr_ext->device_support_hdr()) {
        auto display_data = dx_hdr_ext->get_display_data(window.native_handle());
        LUISA_INFO("\nDisplay data: \nbits_per_color: {}\ncolor_space: {}\nred_primary: {}\ngreen_primary: {}\nblue_primary: {}\nwhite_point: {}\nmin_luminance: {}\nmax_luminance: {}\nmax_full_frame_luminance: {}", display_data.bits_per_color, luisa::to_string(display_data.color_space), display_data.red_primary, display_data.green_primary, display_data.blue_primary, display_data.white_point, display_data.min_luminance, display_data.max_luminance, display_data.max_full_frame_luminance);
        white_point = display_data.max_luminance / 80.0f;
        use_hdr10 = display_data.bits_per_color <= 10;
    }
    const PixelStorage hdr_storage = use_hdr10 ? PixelStorage::R10G10B10A2 : PixelStorage::HALF4;
    const DXHDRExt::ColorSpace hdr_color_space = use_hdr10 ? DXHDRExt::ColorSpace::RGB_FULL_G2084_NONE_P2020 : DXHDRExt::ColorSpace::RGB_FULL_G10_NONE_P709;
    auto swap_chain = dx_hdr_ext->create_swapchain(
        stream,
        DXHDRExt::DXSwapchainOption{
            .window = window.native_handle(),
            .size = make_uint2(resolution),
            .storage = dx_hdr_ext->device_support_hdr() ? hdr_storage : PixelStorage::BYTE4,
            .wants_vsync = false,
        });
    dx_hdr_ext->set_color_space(swap_chain, dx_hdr_ext->device_support_hdr() ? hdr_color_space : DXHDRExt::ColorSpace::RGB_FULL_G22_NONE_P709);

    Callable rec709_to_rec2020 = [](Float3 color) {
        float3x3 conversion =
            transpose(make_float3x3(
                0.627402, 0.329292, 0.043306,
                0.069095, 0.919544, 0.011360,
                0.016394, 0.088028, 0.895578));
        return conversion * color;
    };

    Callable linear_to_st2084 = [](Float3 color) noexcept {
        Float m1 = 2610.f / 4096.f / 4.f;
        Float m2 = 2523.f / 4096.f * 128.f;
        Float c1 = 3424.f / 4096.f;
        Float c2 = 2413.f / 4096.f * 32.f;
        Float c3 = 2392.f / 4096.f * 32.f;
        Float3 cp = pow(abs(color), m1);
        return pow((c1 + c2 * cp) / (1.0f + c3 * cp), m2);
    };
    Callable linear_to_srgb = [&](Var<float3> x) noexcept {
        return saturate(select(1.055f * pow(x, 1.0f / 2.4f) - 0.055f,
                               12.92f * x,
                               x <= 0.00031308f));
    };

    Kernel2D hdr2ldr_kernel = [&](ImageFloat hdr_image, ImageFloat ldr_image, Float scale, Float inter_val, Int mode) noexcept {
        UInt2 coord = dispatch_id().xy();
        Float4 hdr = lerp(float4(1.f), hdr_image.read(coord), inter_val);
        Float3 ldr = hdr.xyz() * scale;
        $switch (mode) {
            // sRGB
            $case (0) {
                ldr = linear_to_srgb(ldr);
            };
            // 10-bit
            $case (1) {

                const float st2084max = 10000.0;
                const float hdrScalar = 80.0f / st2084max;

                // The HDR scene is in Rec.709, but the display is Rec.2020
                ldr = rec709_to_rec2020(ldr);

                // Apply the ST.2084 curve to the scene.
                ldr = linear_to_st2084(ldr * hdrScalar);
            };
            // 16-bit
            $case (2) {
                // LINEAR
            };
        };
        ldr_image.write(coord, make_float4(ldr, 1.f));
    };
    auto hdr2ldr_shader = device.compile(hdr2ldr_kernel);
    Image<float> ldr_image = device.create_image<float>(swap_chain.backend_storage(), resolution);
    stream << texture.copy_from(image_pixels) << synchronize();
    Clock clk;
    float inter_val = 0.0f;
    float light_scale = 0.f;    
    while (!window.should_close()) {
        window.poll_events();
        int mode = 0;
        if (swap_chain.backend_storage() == PixelStorage::R10G10B10A2) {
            mode = 1;
        } else if (swap_chain.backend_storage() == PixelStorage::HALF4) {
            mode = 2;
        }
        auto time = (clk.toc() * 1e-3) - 0.1;
        light_scale = std::clamp<double>(time / 3, 0, 1);
        inter_val = std::clamp<double>(time - 3.0, 0, 1);
        stream << hdr2ldr_shader(texture, ldr_image, white_point * light_scale, inter_val, mode).dispatch(resolution)
               << swap_chain.present(ldr_image);
    }
    stream << synchronize();
}