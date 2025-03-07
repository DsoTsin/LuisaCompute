#include <yyjson.h>
#include <luisa/xir/translators/xir2json.h>

namespace luisa::compute::xir {

luisa::string xir_to_json_translate(const Module *module) noexcept {
    yyjson_alc alc{
        .malloc = [](void *, size_t size) noexcept { return luisa::detail::allocator_allocate(size, 16u); },
        .realloc = [](void *, void *ptr, size_t, size_t size) noexcept { return luisa::detail::allocator_reallocate(ptr, size, 16u); },
        .free = [](void *, void *ptr) noexcept { luisa::detail::allocator_deallocate(ptr, 16u); },
        .ctx = nullptr,
    };
    auto doc = yyjson_mut_doc_new(&alc);
    auto arr = yyjson_mut_arr(doc);
    auto str1 = yyjson_mut_str(doc, "hello");
    auto str2 = yyjson_mut_str(doc, "world");
    yyjson_mut_arr_append(arr, str1);
    yyjson_mut_arr_append(arr, str2);
    yyjson_mut_doc_set_root(doc, arr);
    auto size = static_cast<size_t>(0u);
    auto json = yyjson_mut_write(doc, YYJSON_WRITE_PRETTY | YYJSON_WRITE_NEWLINE_AT_END, &size);
    auto result = luisa::string{json, size};
    yyjson_mut_doc_free(doc);
    return result;
}

}// namespace luisa::compute::xir