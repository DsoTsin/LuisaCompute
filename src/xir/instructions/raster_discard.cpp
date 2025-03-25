#include <luisa/xir/builder.h>
#include <luisa/xir/instructions/raster_discard.h>

namespace luisa::compute::xir {

RasterDiscardInst *RasterDiscardInst::clone(XIRBuilder &b, InstructionCloneValueResolver &resolver) const noexcept {
    return b.raster_discard();
}

}// namespace luisa::compute::xir
