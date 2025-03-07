#include <luisa/core/logging.h>
#include <luisa/xir/pool.h>

namespace luisa::compute::xir {

namespace detail {
void luisa_xir_pooled_object_check_pool(PooledObject *object, Pool *pool) noexcept {
    LUISA_DEBUG_ASSERT(object->pool() == pool, "Detected object from another pool.");
}
}// namespace detail

Pool::Pool(size_t init_cap) noexcept {
    if (init_cap != 0u) {
        _objects.reserve(init_cap);
    }
}

Pool::~Pool() noexcept {
    for (auto object : _objects) { luisa::delete_with_allocator(object); }
}

PoolOwner::PoolOwner(size_t init_pool_cap) noexcept
    : _pool{luisa::make_unique<Pool>(init_pool_cap)} {}

}// namespace luisa::compute::xir
