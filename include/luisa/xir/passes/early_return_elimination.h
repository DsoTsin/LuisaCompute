#pragma once

#include <luisa/core/dll_export.h>
#include <luisa/core/stl/vector.h>

namespace luisa::compute::xir {

class ReturnInst;
class Module;
class Function;

struct EarlyReturnEliminationInfo {
    luisa::vector<ReturnInst *> eliminated_instructions;
};

[[nodiscard]] LC_XIR_API EarlyReturnEliminationInfo early_return_elimination_pass_run_on_function(Function *function) noexcept;
[[nodiscard]] LC_XIR_API EarlyReturnEliminationInfo early_return_elimination_pass_run_on_module(Module *module) noexcept;

}// namespace luisa::compute::xir
