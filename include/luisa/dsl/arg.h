#pragma once

namespace luisa::compute::detail {

/**
 * @brief Tag type used to indicate that an argument is being created.
 *
 * This tag is used when constructing a `Var` object as a function argument.
 * It signals that the variable should be treated as a new argument
 * rather than a copy of an existing value.
 */
struct ArgumentCreation {};

/**
 * @brief Tag type used to indicate that a reference argument is being created.
 *
 * This tag is used when constructing a `Var` object as a reference argument
 * in a function. It signals that the variable should be treated as a
 * reference to an existing value rather than a copy.
 */
struct ReferenceArgumentCreation {};

}// namespace luisa::compute::detail
