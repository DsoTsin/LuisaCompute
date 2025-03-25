#include <luisa/core/logging.h>
#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/store.h>

namespace luisa::compute::xir {

StoreInst::StoreInst(BasicBlock *parent_block, Value *variable, Value *value) noexcept
    : Super{parent_block, nullptr} {
    auto operands = std::array{variable, value};
    LUISA_DEBUG_ASSERT(operands[operand_index_variable] == variable, "Unexpected operand index.");
    LUISA_DEBUG_ASSERT(operands[operand_index_value] == value, "Unexpected operand index.");
    set_operands(operands);
}

void StoreInst::set_variable(Value *variable) noexcept {
    set_operand(operand_index_variable, variable);
}

void StoreInst::set_value(Value *value) noexcept {
    set_operand(operand_index_value, value);
}

StoreInst *StoreInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_variable = resolver.resolve(variable());
    auto resolved_value = resolver.resolve(value());
    return b.store(resolved_variable, resolved_value);
}

}// namespace luisa::compute::xir
