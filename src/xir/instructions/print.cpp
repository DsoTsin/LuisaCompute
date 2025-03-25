#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/print.h>

namespace luisa::compute::xir {

PrintInst::PrintInst(BasicBlock *parent_block, luisa::string format,
                     luisa::span<Value *const> operands) noexcept
    : Super{std::move(format), parent_block, nullptr} {
    set_operands(operands);
}

PrintInst *PrintInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    luisa::fixed_vector<Value *, 16u> resolved_operands;
    resolved_operands.reserve(operand_count());
    for (auto op_use : operand_uses()) {
        resolved_operands.emplace_back(resolver.resolve(op_use->value()));
    }
    return b.print(message(), resolved_operands);
}

}// namespace luisa::compute::xir
