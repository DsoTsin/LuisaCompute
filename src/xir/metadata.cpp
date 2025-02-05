#include <luisa/core/logging.h>
#include <luisa/xir/metadata/name.h>
#include <luisa/xir/metadata/location.h>
#include <luisa/xir/metadata/comment.h>
#include <luisa/xir/metadata.h>

namespace luisa::compute::xir {

Metadata::Metadata(Pool *pool) noexcept : _pool{pool} {
    LUISA_DEBUG_ASSERT(pool != nullptr, "Pool must not be null.");
}

namespace detail {

Metadata *luisa_xir_metadata_list_mixin_find_metadata(MetadataList &list, DerivedMetadataTag tag) noexcept {
    for (auto &m : list) {
        if (m.derived_metadata_tag() == tag) { return &m; }
    }
    return nullptr;
}

Metadata *luisa_xir_metadata_list_mixin_create_metadata(MetadataList &list, Pool *pool, DerivedMetadataTag tag) noexcept {
    switch (tag) {
#define LUISA_XIR_MAKE_METADATA_CREATE_CASE(type)   \
    case type##MD::static_derived_metadata_tag(): { \
        auto m = pool->create<type##MD>(pool);      \
        m->add_to_list(list);                       \
        return m;                                   \
    }
        LUISA_XIR_MAKE_METADATA_CREATE_CASE(Name)
        LUISA_XIR_MAKE_METADATA_CREATE_CASE(Location)
        LUISA_XIR_MAKE_METADATA_CREATE_CASE(Comment)
#undef LUISA_XIR_MAKE_METADATA_CREATE_CASE
    }
    LUISA_ERROR_WITH_LOCATION("Unknown derived metadata tag 0x{:x}.",
                              static_cast<uint32_t>(tag));
}

Metadata *luisa_xir_metadata_list_mixin_find_or_create_metadata(MetadataList &list, Pool *pool, DerivedMetadataTag tag) noexcept {
    if (auto m = luisa_xir_metadata_list_mixin_find_metadata(list, tag)) { return m; }
    return luisa_xir_metadata_list_mixin_create_metadata(list, pool, tag);
}

void luisa_xir_metadata_list_mixin_set_name(MetadataList &list, Pool *pool, std::string_view name) noexcept {
    auto m = luisa_xir_metadata_list_mixin_find_or_create_metadata(list, pool, DerivedMetadataTag::NAME);
    LUISA_DEBUG_ASSERT(m->isa<NameMD>(), "Invalid metadata type.");
    static_cast<NameMD *>(m)->set_name(name);
}

void luisa_xir_metadata_list_mixin_set_location(MetadataList &list, Pool *pool, const std::filesystem::path &file, int line) noexcept {
    auto m = luisa_xir_metadata_list_mixin_find_or_create_metadata(list, pool, DerivedMetadataTag::LOCATION);
    LUISA_DEBUG_ASSERT(m->isa<LocationMD>(), "Invalid metadata type.");
    static_cast<LocationMD *>(m)->set_location(file, line);
}

void luisa_xir_metadata_list_mixin_add_comment(MetadataList &list, Pool *pool, std::string_view comment) noexcept {
    auto m = luisa_xir_metadata_list_mixin_create_metadata(list, pool, DerivedMetadataTag::COMMENT);
    LUISA_DEBUG_ASSERT(m->isa<CommentMD>(), "Invalid metadata type.");
    static_cast<CommentMD *>(m)->set_comment(comment);
}

luisa::optional<luisa::string_view> luisa_xir_metadata_list_mixin_get_name(const MetadataList &list) noexcept {
    auto m = const_cast<const Metadata *>(luisa_xir_metadata_list_mixin_find_metadata(const_cast<MetadataList &>(list), DerivedMetadataTag::NAME));
    LUISA_DEBUG_ASSERT(m == nullptr || m->isa<NameMD>(), "Invalid metadata type.");
    if (m == nullptr) { return luisa::nullopt; }
    return luisa::string_view{static_cast<const NameMD *>(m)->name()};
}

}// namespace detail

}// namespace luisa::compute::xir
