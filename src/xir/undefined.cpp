#include <luisa/xir/undefined.h>

namespace luisa::compute::xir {

Undefined::Undefined(Module *module, const Type *type) noexcept
    : Super{module, type} {}

}// namespace luisa::compute::xir
