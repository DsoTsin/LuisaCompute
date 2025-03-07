#pragma once

#include <luisa/xir/value.h>

namespace luisa::compute::xir {

class LC_XIR_API Undefined final : public IntrusiveForwardNode<Undefined, DerivedGlobalValue<Undefined, DerivedValueTag::UNDEFINED>> {
public:
    Undefined(Module *module, const Type *type) noexcept;
};

using UndefinedList = IntrusiveForwardList<Undefined>;

}// namespace luisa::compute::xir
