#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/assume.h>

namespace luisa::compute::xir {

AssumeInst::AssumeInst(BasicBlock *parent_block, Value *condition, luisa::string message) noexcept
    : Super{std::move(message), parent_block, nullptr} {
    set_operands(std::array{condition});
}

void AssumeInst::set_condition(Value *condition) noexcept {
    set_operand(operand_index_condition, condition);
}

Value *AssumeInst::condition() noexcept {
    return operand(operand_index_condition);
}

const Value *AssumeInst::condition() const noexcept {
    return operand(operand_index_condition);
}

AssumeInst *AssumeInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_cond = resolver.resolve(condition());
    return b.assume_(resolved_cond, message());
}

}// namespace luisa::compute::xir
