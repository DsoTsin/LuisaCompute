#pragma once

#include <luisa/xir/instruction.h>

namespace luisa::compute::xir {

class LC_XIR_API RasterDiscardInst final : public DerivedTerminatorInstruction<RasterDiscardInst, DerivedInstructionTag::RASTER_DISCARD> {
public:
    using Super::Super;
    [[nodiscard]] RasterDiscardInst *clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept override;
};

}// namespace luisa::compute::xir
