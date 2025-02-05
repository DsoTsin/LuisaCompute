#pragma once

#include <luisa/xir/module.h>

namespace luisa::compute {
class Function;
class ExternalFunction;
}// namespace luisa::compute

namespace luisa::compute::xir {

class AST2XIRContext;

struct AST2XIRConfig {
};

using ASTFunction = compute::Function;
using ASTExternalFunction = compute::ExternalFunction;

[[nodiscard]] LC_XIR_API AST2XIRContext *ast_to_xir_translate_begin(const AST2XIRConfig &config) noexcept;
void LC_XIR_API ast_to_xir_translate_add_function(AST2XIRContext *ctx, const ASTFunction &f) noexcept;
void LC_XIR_API ast_to_xir_translate_add_external_function(AST2XIRContext *ctx, const ASTExternalFunction &f) noexcept;
[[nodiscard]] LC_XIR_API luisa::unique_ptr<Module> ast_to_xir_translate_finalize(AST2XIRContext *ctx) noexcept;

[[nodiscard]] LC_XIR_API luisa::unique_ptr<Module> ast_to_xir_translate(const ASTFunction &kernel, const AST2XIRConfig &config) noexcept;

}// namespace luisa::compute::xir
