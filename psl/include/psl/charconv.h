/**
 * @addtogroup psl
 * @{
 * @file charconv.h Integer <-> string conversions.
 */

#pragma once

#include <psl/type_traits.h>

namespace psl {

/**
 * Represents a status code returned by the character conversion functions.
 */
enum class charconv_status {
  ok,
  overflow,
  invalid_arg,
};

/**
 * Result returned by to_chars.
 */
struct to_chars_result {
  char* ptr;              ///< Indicates the end of the formatted number.
  charconv_status status; ///< Status code.
};


/**
 * Convert an integer value into a character string and store it in `[begin,
 * end)`.
 * @param begin Pointer to the start of the result buffer.
 * @param end Pointer past the end of the result buffer.
 * @param value Value to convert.
 * @param base Numerical base to use. Must be between 2 and 36.
 * @return On success, a result with @ref psl::to_chars_result::status "status"
 * set to `ok` and @ref psl::to_chars_result::ptr "ptr"
 * set past the end of the formatted number. On error, a result with @ref
 * psl::to_chars_result::status "status" set to the appropriate error code and
 * @ref psl::to_chars_result::ptr "ptr" set to `end`.
 * @note This function only participates in overload resolution if `I` is an
 * integer type.
 */
template <typename I, typename = enable_if_t<is_integral_v<I>>>
to_chars_result to_chars(char* begin, char* end, I value, int base = 10) {}

} // namespace psl

/** @} */
