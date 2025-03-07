#include <luisa/xir/metadata/comment.h>

namespace luisa::compute::xir {

CommentMD::CommentMD(Pool *pool, luisa::string comment) noexcept
    : Super{pool}, _comment{std::move(comment)} {}

void CommentMD::set_comment(luisa::string_view comment) noexcept {
    _comment = comment;
}

CommentMD *CommentMD::clone(Pool *pool) const noexcept {
    return pool->create<CommentMD>(pool, this->comment());
}

}// namespace luisa::compute::xir
