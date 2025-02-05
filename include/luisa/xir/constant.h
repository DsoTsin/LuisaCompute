#pragma once

#include <luisa/xir/value.h>

namespace luisa::compute::xir {

class LC_XIR_API Constant final : public IntrusiveForwardNode<Constant, DerivedGlobalValue<Constant, DerivedValueTag::CONSTANT>> {

private:
    union {
        std::byte _small[sizeof(void *)] = {};
        void *_large;
    };
    uint64_t _hash = {};

    [[nodiscard]] bool _is_small() const noexcept;
    [[nodiscard]] void *_data() noexcept;
    void _update_hash(luisa::optional<uint64_t> hash) noexcept;
    void _check_reinterpret_cast_type_size(size_t size) const noexcept;

private:
    Constant(Module *module, const Type *type) noexcept;

protected:
    friend class Module;
    struct ctor_tag_zero {};
    struct ctor_tag_one {};

public:
    Constant(Module *parent_module, const Type *type, const void *data,
             luisa::optional<uint64_t> hash = luisa::nullopt) noexcept;
    Constant(Module *parent_module, const Type *type, ctor_tag_zero,
             luisa::optional<uint64_t> hash = luisa::nullopt) noexcept;
    Constant(Module *parent_module, const Type *type, ctor_tag_one,
             luisa::optional<uint64_t> hash = luisa::nullopt) noexcept;
    ~Constant() noexcept override;

    [[nodiscard]] const void *data() const noexcept;
    [[nodiscard]] auto hash() const noexcept { return _hash; }

    template<typename T>
    [[nodiscard]] const T &as() const noexcept {
        _check_reinterpret_cast_type_size(sizeof(T));
        return *static_cast<const T *>(data());
    }
};

using ConstantList = IntrusiveForwardList<Constant>;

}// namespace luisa::compute::xir
