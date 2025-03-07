#include <luisa/xir/metadata/location.h>

namespace luisa::compute::xir {

LocationMD::LocationMD(Pool *pool, luisa::filesystem::path file, int line) noexcept
    : Super{pool}, _file{std::move(file)}, _line{line} {}

void LocationMD::set_location(luisa::filesystem::path file, int line) noexcept {
    set_file(std::move(file));
    set_line(line);
}

LocationMD *LocationMD::clone(Pool *pool) const noexcept {
    return pool->create<LocationMD>(pool, file(), line());
}

}// namespace luisa::compute::xir
