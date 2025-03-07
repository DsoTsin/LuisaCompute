#pragma once

#include <luisa/core/dll_export.h>
#include <luisa/core/stl/unordered_map.h>

namespace luisa::compute::xir {

class Module;
class Function;

class AllocaInst;
class LoadInst;
class StoreInst;
class PhiInst;

// This pass is similar to LLVM's mem2reg pass. It tries to rewrite
// alloca/load/store instructions into SSA values.
//
// Note: this pass does not guarantee that all alloca's are eliminated.
// Typically, the following cases are not handled:
// - Aggregates that are used by GEPs;
// - Shared memory alloca's; and
// - Alloca's that are used as reference arguments.
// It's recommended to run this pass after load elimination and dead
// code elimination passes.

struct Mem2RegInfo {
    luisa::unordered_set<AllocaInst *> promoted_alloca_instructions;
    luisa::unordered_set<StoreInst *> removed_store_instructions;
    luisa::unordered_set<LoadInst *> removed_load_instructions;
    luisa::unordered_set<PhiInst *> inserted_phi_instructions;
};

[[nodiscard]] LC_XIR_API Mem2RegInfo mem2reg_pass_run_on_function(Function *function) noexcept;
[[nodiscard]] LC_XIR_API Mem2RegInfo mem2reg_pass_run_on_module(Module *module) noexcept;

}// namespace luisa::compute::xir
