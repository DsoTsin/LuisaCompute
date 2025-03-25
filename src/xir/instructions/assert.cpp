#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/assert.h>

namespace luisa::compute::xir {

AssertInst::AssertInst(BasicBlock *parent_block, Value *condition, luisa::string message) noexcept
    : Super{std::move(message), parent_block, nullptr} {
    set_operands(std::array{condition});
}

void AssertInst::set_condition(Value *condition) noexcept {
    set_operand(operand_index_condition, condition);
}

Value *AssertInst::condition() noexcept {
    return operand(operand_index_condition);
}

const Value *AssertInst::condition() const noexcept {
    return operand(operand_index_condition);
}

AssertInst *AssertInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_cond = resolver.resolve(condition());
    return b.assert_(resolved_cond, message());
}

}// namespace luisa::compute::xir
