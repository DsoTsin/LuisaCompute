#include <luisa/ast/type_registry.h>
#include <luisa/xir/argument.h>

namespace luisa::compute::xir {

Argument::Argument(Function *parent_function, const Type *type) noexcept
    : Super{parent_function, type} {}

}// namespace luisa::compute::xir
