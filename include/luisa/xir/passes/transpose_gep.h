#pragma once

#include <luisa/core/dll_export.h>
#include <luisa/core/stl/unordered_map.h>

namespace luisa::compute::xir {

class Function;
class Module;
class LoadInst;
class StoreInst;
class ArithmeticInst;

// This pass converts load/store instructions on aggregate GEPs to extract/insert.
// Specifically, it follows the template below:
// - Load(GEP(agg, indices...)) => Extract(Load(agg), indices...)
// - Store(GEP(agg, indices...), elem) => Store(agg, Insert(Load(agg), elem, indices...))
// This pass is designed to help the mem2reg pass handle aggregates.

struct TransposeGEPInfo {
    luisa::unordered_map<LoadInst *, ArithmeticInst *> transposed_load_instructions;
    luisa::unordered_map<StoreInst *, StoreInst *> transposed_store_instructions;
};

[[nodiscard]] LC_XIR_API TransposeGEPInfo transpose_gep_pass_run_on_function(Function *function) noexcept;
[[nodiscard]] LC_XIR_API TransposeGEPInfo transpose_gep_pass_run_on_module(Module *module) noexcept;

}// namespace luisa::compute::xir
