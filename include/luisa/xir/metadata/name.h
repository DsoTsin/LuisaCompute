#pragma once

#include <luisa/core/stl/string.h>
#include <luisa/xir/metadata.h>

namespace luisa::compute::xir {

class LC_XIR_API NameMD final : public DerivedMetadata<NameMD, DerivedMetadataTag::NAME> {

private:
    luisa::string _name;

public:
    explicit NameMD(Pool *pool, luisa::string name = {}) noexcept;
    void set_name(luisa::string_view name) noexcept;
    [[nodiscard]] auto &name() noexcept { return _name; }
    [[nodiscard]] auto &name() const noexcept { return _name; }
    [[nodiscard]] NameMD *clone(Pool *pool) const noexcept override;
};

}// namespace luisa::compute::xir
