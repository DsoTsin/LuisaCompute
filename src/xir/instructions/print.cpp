#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/print.h>

namespace luisa::compute::xir {

PrintInst::PrintInst(BasicBlock *parent_block, luisa::string format,
                     luisa::span<Value *const> operands) noexcept
    : Super{parent_block, nullptr}, _format{std::move(format)} {
    set_operands(operands);
}

PrintInst *PrintInst::clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept {
    luisa::fixed_vector<Value *, 16u> resolved_operands;
    resolved_operands.reserve(operand_count());
    for (auto op_use : operand_uses()) {
        resolved_operands.emplace_back(resolver.resolve(op_use->value()));
    }
    return b.print(_format, resolved_operands);
}

}// namespace luisa::compute::xir
