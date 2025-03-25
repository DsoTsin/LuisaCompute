#pragma once

#include <luisa/core/dll_export.h>

namespace luisa::compute::xir {

class AllocaInst;
class PhiInst;
class Value;
class Instruction;
class XIRBuilder;
class FunctionDefinition;

struct InstructionCloneValueResolver;

[[nodiscard]] LC_XIR_API Value *trace_pointer_base_value(Value *pointer) noexcept;
[[nodiscard]] LC_XIR_API AllocaInst *trace_pointer_base_local_alloca_inst(Value *pointer) noexcept;
[[nodiscard]] LC_XIR_API bool remove_redundant_phi_instruction(PhiInst *phi) noexcept;
LC_XIR_API void lower_phi_node_to_local_variable(PhiInst *phi) noexcept;
LC_XIR_API void hoist_alloca_instructions_to_entry_block(FunctionDefinition *f) noexcept;

}// namespace luisa::compute::xir
