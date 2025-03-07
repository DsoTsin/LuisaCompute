#pragma once

#include <luisa/core/stl/optional.h>
#include <luisa/core/stl/filesystem.h>
#include <luisa/xir/ilist.h>

namespace luisa::compute::xir {

enum struct DerivedMetadataTag {
    NAME,
    LOCATION,
    COMMENT,
};

class LC_XIR_API Metadata : public IntrusiveForwardNode<Metadata> {

private:
    Pool *_pool{nullptr};

public:
    explicit Metadata(Pool *pool) noexcept;
    [[nodiscard]] Pool *pool() noexcept override { return _pool; }
    [[nodiscard]] virtual DerivedMetadataTag derived_metadata_tag() const noexcept = 0;
    [[nodiscard]] virtual Metadata *clone(Pool *pool) const noexcept = 0;
    LUISA_XIR_DEFINED_ISA_METHOD(Metadata, metadata)
};

template<typename Derived, DerivedMetadataTag tag, typename Base = Metadata>
    requires std::derived_from<Base, Metadata>
class LC_XIR_API DerivedMetadata : public Base {
public:
    using derived_metadata_type = Derived;
    using Super = DerivedMetadata;
    using Base::Base;

    [[nodiscard]] static constexpr auto
    static_derived_metadata_tag() noexcept { return tag; }

    [[nodiscard]] DerivedMetadataTag
    derived_metadata_tag() const noexcept final { return static_derived_metadata_tag(); }
};

using MetadataList = IntrusiveForwardList<Metadata>;

namespace detail {
[[nodiscard]] LC_XIR_API Metadata *luisa_xir_metadata_list_mixin_find_metadata(MetadataList &list, DerivedMetadataTag tag) noexcept;
[[nodiscard]] LC_XIR_API Metadata *luisa_xir_metadata_list_mixin_create_metadata(MetadataList &list, Pool *pool, DerivedMetadataTag tag) noexcept;
[[nodiscard]] LC_XIR_API Metadata *luisa_xir_metadata_list_mixin_find_or_create_metadata(MetadataList &list, Pool *pool, DerivedMetadataTag tag) noexcept;
[[nodiscard]] LC_XIR_API luisa::optional<luisa::string_view> luisa_xir_metadata_list_mixin_get_name(const MetadataList &list) noexcept;
LC_XIR_API void luisa_xir_metadata_list_mixin_set_name(MetadataList &list, Pool *pool, std::string_view name) noexcept;
LC_XIR_API void luisa_xir_metadata_list_mixin_set_location(MetadataList &list, Pool *pool, const std::filesystem::path &file, int line) noexcept;
LC_XIR_API void luisa_xir_metadata_list_mixin_add_comment(MetadataList &list, Pool *pool, std::string_view comment) noexcept;
}// namespace detail

template<typename Base>
class MetadataListMixin : public Base {

private:
    MetadataList _metadata_list;

private:
    [[nodiscard]] Pool *_pool_from_base() noexcept { return static_cast<Base *>(this)->pool(); }

public:
    using Super = MetadataListMixin;
    using Base::Base;

    [[nodiscard]] auto &metadata_list() noexcept { return _metadata_list; }
    [[nodiscard]] auto &metadata_list() const noexcept { return _metadata_list; }

    [[nodiscard]] Metadata *find_metadata(DerivedMetadataTag tag) noexcept {
        return detail::luisa_xir_metadata_list_mixin_find_metadata(_metadata_list, tag);
    }
    [[nodiscard]] const Metadata *find_metadata(DerivedMetadataTag tag) const noexcept {
        return const_cast<MetadataListMixin *>(this)->find_metadata(tag);
    }
    [[nodiscard]] Metadata *create_metadata(DerivedMetadataTag tag) noexcept {
        return detail::luisa_xir_metadata_list_mixin_create_metadata(_metadata_list, _pool_from_base(), tag);
    }
    [[nodiscard]] Metadata *find_or_create_metadata(DerivedMetadataTag tag) noexcept {
        return detail::luisa_xir_metadata_list_mixin_find_or_create_metadata(_metadata_list, _pool_from_base(), tag);
    }

    template<typename T>
    [[nodiscard]] auto find_metadata() noexcept {
        return static_cast<T *>(find_metadata(T::static_derived_metadata_tag()));
    }
    template<typename T>
    [[nodiscard]] auto find_metadata() const noexcept {
        return static_cast<const T *>(find_metadata(T::static_derived_metadata_tag()));
    }
    template<typename T>
    [[nodiscard]] auto create_metadata() noexcept {
        return static_cast<T *>(create_metadata(T::static_derived_metadata_tag()));
    }
    template<typename T>
    [[nodiscard]] auto find_or_create_metadata() noexcept {
        return static_cast<T *>(find_or_create_metadata(T::static_derived_metadata_tag()));
    }

    void set_name(std::string_view name) noexcept {
        detail::luisa_xir_metadata_list_mixin_set_name(_metadata_list, _pool_from_base(), name);
    }
    void set_location(const std::filesystem::path &file, int line = -1) noexcept {
        detail::luisa_xir_metadata_list_mixin_set_location(_metadata_list, _pool_from_base(), file, line);
    }
    void add_comment(std::string_view comment) noexcept {
        detail::luisa_xir_metadata_list_mixin_add_comment(_metadata_list, _pool_from_base(), comment);
    }

    [[nodiscard]] luisa::optional<luisa::string_view> name() const noexcept {
        return detail::luisa_xir_metadata_list_mixin_get_name(_metadata_list);
    }
};

}// namespace luisa::compute::xir
