#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/assume.h>

namespace luisa::compute::xir {

AssumeInst::AssumeInst(BasicBlock *parent_block, Value *condition, luisa::string message) noexcept
    : Super{parent_block, nullptr}, _message{std::move(message)} {
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

void AssumeInst::set_message(luisa::string_view message) noexcept {
    _message = message;
}

luisa::string_view AssumeInst::message() const noexcept {
    return _message;
}

AssumeInst *AssumeInst::clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept {
    auto resolved_cond = resolver.resolve(condition());
    return b.assume_(resolved_cond, _message);
}

}// namespace luisa::compute::xir
