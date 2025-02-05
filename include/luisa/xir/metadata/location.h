#pragma once

#include <luisa/core/stl/filesystem.h>
#include <luisa/xir/metadata.h>

namespace luisa::compute::xir {

class LC_XIR_API LocationMD final : public DerivedMetadata<LocationMD, DerivedMetadataTag::LOCATION> {

private:
    luisa::filesystem::path _file;
    int _line;

public:
    explicit LocationMD(Pool *pool, luisa::filesystem::path file = {}, int line = -1) noexcept;
    void set_location(luisa::filesystem::path file, int line = -1) noexcept;
    void set_file(luisa::filesystem::path file) noexcept { _file = std::move(file); }
    void set_line(int line) noexcept { _line = line; }
    [[nodiscard]] auto &file() const noexcept { return _file; }
    [[nodiscard]] auto line() const noexcept { return _line; }
    [[nodiscard]] LocationMD *clone(Pool *pool) const noexcept override;
};

}// namespace luisa::compute::xir