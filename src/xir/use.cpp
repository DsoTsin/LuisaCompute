#include <luisa/core/logging.h>
#include <luisa/xir/value.h>
#include <luisa/xir/user.h>
#include <luisa/xir/use.h>

namespace luisa::compute::xir {

Use::Use(User *user, Value *value) noexcept : _user{user}, _value{value} {
    LUISA_DEBUG_ASSERT(user != nullptr, "User must not be null.");
    LUISA_DEBUG_ASSERT(value == nullptr || value->pool() == pool(), "User and value should be in the same pool.");
}

void Use::set_value(Value *value) noexcept {
    LUISA_DEBUG_ASSERT(value == nullptr || value->pool() == pool(), "Use and value should be in the same pool.");
    _value = value;
}

Pool *Use::pool() noexcept { return user()->pool(); }

}// namespace luisa::compute::xir
