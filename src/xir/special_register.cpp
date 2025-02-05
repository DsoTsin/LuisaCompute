#include <luisa/core/logging.h>
#include <luisa/ast/type_registry.h>
#include <luisa/xir/module.h>
#include <luisa/xir/special_register.h>

namespace luisa::compute::xir {

namespace detail {
const Type *special_register_type_uint() noexcept { return Type::of<uint>(); }
const Type *special_register_type_uint3() noexcept { return Type::of<uint3>(); }
}// namespace detail

}// namespace luisa::compute::xir
