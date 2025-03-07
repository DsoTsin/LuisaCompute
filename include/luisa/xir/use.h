#pragma once

#include <luisa/xir/ilist.h>

namespace luisa::compute::xir {

class Value;
class User;

class LC_XIR_API Use final : public IntrusiveForwardNode<Use> {

private:
    User *_user;
    Value *_value;

public:
    explicit Use(User *user, Value *value = nullptr) noexcept;
    void set_value(Value *value) noexcept;
    [[nodiscard]] Pool *pool() noexcept override;
    [[nodiscard]] auto value() noexcept { return _value; }
    [[nodiscard]] auto value() const noexcept { return const_cast<const Value *>(_value); }
    [[nodiscard]] auto user() noexcept { return _user; }
    [[nodiscard]] auto user() const noexcept { return const_cast<const User *>(_user); }
};

using UseList = IntrusiveForwardList<Use>;

}// namespace luisa::compute::xir
