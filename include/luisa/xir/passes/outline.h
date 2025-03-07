#pragma once

#include <luisa/core/stl/unordered_map.h>
#include <luisa/xir/module.h>
#include <luisa/xir/instructions/outline.h>

namespace luisa::compute::xir {

// This pass will outline all outline instructions in the module.
// Information about the outlined functions will be returned.

struct OutlineInfo {
    luisa::unordered_map<OutlineInst *, Function *> outlines;
};

LC_XIR_API OutlineInfo outline_pass_run_on_function(Module *module, Function *function) noexcept;
LC_XIR_API OutlineInfo outline_pass_run_on_module(Module *module) noexcept;

}// namespace luisa::compute::xir
