#pragma once

#include <luisa/core/dll_export.h>
#include <luisa/core/stl/unordered_map.h>

namespace luisa::compute::xir {

class Module;
class Function;

struct AutodiffInfo {
};

struct AutodiffOptions {
    bool run_forward{true};
    bool run_backward{true};
};

LC_XIR_API void autodiff_pass_run_on_function(Function *function, const AutodiffOptions &options = {}) noexcept;
LC_XIR_API void autodiff_pass_run_on_module(Module *module, const AutodiffOptions &options = {}) noexcept;

}// namespace luisa::compute::xir
