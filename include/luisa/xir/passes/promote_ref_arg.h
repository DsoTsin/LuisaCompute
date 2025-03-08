#pragma once

#include <luisa/core/dll_export.h>
#include <luisa/core/stl/unordered_map.h>

namespace luisa::compute::xir {

class ReferenceArgument;
class ValueArgument;
class Module;

struct PromoteRefArgInfo {
    luisa::unordered_map<ReferenceArgument *, ValueArgument *> promoted_ref_args;
};

[[nodiscard]] LC_XIR_API PromoteRefArgInfo promote_ref_arg_pass_run_on_module(Module *module) noexcept;

}// namespace luisa::compute::xir
