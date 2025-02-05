#pragma once

#include <luisa/core/dll_export.h>
#include <luisa/core/stl/vector.h>

namespace luisa::compute::xir {

class PhiInst;
class Function;
class Module;

struct Reg2MemInfo {
    luisa::vector<PhiInst *> lowered_phi_nodes;
};

[[nodiscard]] LC_XIR_API Reg2MemInfo reg2mem_pass_run_on_function(Function *function) noexcept;
[[nodiscard]] LC_XIR_API Reg2MemInfo reg2mem_pass_run_on_module(Module *module) noexcept;

}// namespace luisa::compute::xir
