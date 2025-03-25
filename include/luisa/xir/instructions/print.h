#pragma once

#include <luisa/core/stl/string.h>
#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API PrintInst final : public PrintMessageMixin<DerivedInstruction<PrintInst, DerivedInstructionTag::PRINT>> {
public:
    explicit PrintInst(BasicBlock *parent_block, luisa::string format = {},
                       luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] decltype(auto) format() const noexcept { return Super::message(); }
    void set_format(luisa::string_view format) noexcept { Super::set_message(format); }
    [[nodiscard]] PrintInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
