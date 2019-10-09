/**
 * @addtogroup psl
 * @{
 * @file charconv.h Integer <-> string conversions.
 */

#pragma once

#include <limits.h>
#include <psl/iterator.h>
#include <psl/type_traits.h>
#include <string.h>

namespace psl {
namespace impl {

/**
 * Lookup table used to convert digits to their character representation.
 */
constexpr char to_chars_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

} // namespace impl

/**
 * Convert an integer value into a character string and store it in `[first,
 * last)`, which must be a valid range.
 * @param first Pointer to the start of the result buffer.
 * @param last Pointer past the end of the result buffer.
 * @param value Value to convert.
 * @param base Numerical base to use. Must be between 2 and 36.
 * @return On success, a pointer past the end of the converted integer. On error
 * (if the buffer is too small), returns `nullptr`.
 * @note This function does not null terminate the output buffer.
 */
template <typename I, typename = enable_if_t<is_integral_v<I>>>
char* to_chars(char* first, char* last, I value, int base = 10) {
  auto uvalue = static_cast<psl::make_unsigned_t<I>>(value);

  if constexpr (is_signed_v<I>) {
    if (value < 0) {
      // No need to check here since [first, last) is required to be a valid
      // range.
      *first++ = '-';
      uvalue = -value;
    }
  }

  // Conservative size estimate, sufficient even for smallest base.
  char buf[sizeof(I) * CHAR_BIT];
  char* rnext = psl::end(buf);

  switch (base) {
    // Improved codegen for common cases
  case 10:
    do {
      *--rnext = static_cast<char>(uvalue % 10 + '0');
      uvalue /= 10;
    } while (uvalue);
    break;

  case 16:
    do {
      *--rnext = impl::to_chars_digits[uvalue & 0xf];
      uvalue >>= 4;
    } while (uvalue);
    break;

    // Generic path
  default:
    do {
      *--rnext = impl::to_chars_digits[uvalue % base];
      uvalue /= base;
    } while (uvalue);
    break;
  }

  ptrdiff_t str_size = psl::end(buf) - rnext;
  if (str_size > last - first) {
    return nullptr;
  }

  memcpy(first, rnext, str_size);
  return first + str_size;
}

} // namespace psl

/** @} */
