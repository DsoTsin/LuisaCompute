#pragma once

#include <luisa/core/stl/string.h>
#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API PrintInst final : public DerivedInstruction<PrintInst, DerivedInstructionTag::PRINT> {

private:
    luisa::string _format;

public:
    explicit PrintInst(BasicBlock *parent_block, luisa::string format = {},
                       luisa::span<Value *const> operands = {}) noexcept;
    [[nodiscard]] auto format() const noexcept { return luisa::string_view{_format}; }
    void set_format(luisa::string_view format) noexcept { _format = format; }
    [[nodiscard]] PrintInst *clone(Builder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
