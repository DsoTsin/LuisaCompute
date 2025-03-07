#pragma once

#include <luisa/xir/module.h>

namespace luisa::compute::xir {

[[nodiscard]] LC_XIR_API luisa::string xir_to_json_translate(const Module *module) noexcept;

}// namespace luisa::compute::xir
