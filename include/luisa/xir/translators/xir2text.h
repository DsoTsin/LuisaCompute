#pragma once

#include <luisa/xir/module.h>

namespace luisa::compute::xir {

[[nodiscard]] LC_XIR_API luisa::string xir_to_text_translate(const Module *module, bool debug_info) noexcept;

}// namespace luisa::compute::xir
