#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/unreachable.h>

namespace luisa::compute::xir {

UnreachableInst::UnreachableInst(BasicBlock *parent_block, luisa::string message) noexcept
    : Super{parent_block}, _message{std::move(message)} {}

void UnreachableInst::set_message(luisa::string_view message) noexcept {
    _message = message;
}

luisa::string_view UnreachableInst::message() const noexcept {
    return _message;
}

UnreachableInst *UnreachableInst::clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept {
    return b.unreachable_(_message);
}

}// namespace luisa::compute::xir
