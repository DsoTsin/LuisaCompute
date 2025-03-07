#pragma once

#include <luisa/core/stl/unordered_map.h>
#include <luisa/xir/module.h>

namespace luisa::compute::xir {

// This pass is used to eliminate (trivially) dead code.

struct DCEInfo {
    luisa::unordered_set<Instruction *> removed_instructions;
};

[[nodiscard]] LC_XIR_API DCEInfo dce_pass_run_on_function(Function *function) noexcept;
[[nodiscard]] LC_XIR_API DCEInfo dce_pass_run_on_module(Module *module) noexcept;

}// namespace luisa::compute::xir
