/**
 * @addtogroup psl
 * @{
 * @file algorithm.h General-purpose algorithms, inspired by &lt;algorithm&gt;.
 */

#pragma once

namespace psl {

/**
 * Search forward for `val` in `[first, last)`.
 * @param first Input iterator indicating start of range.
 * @param last Input iterator past end of range.
 * @param val Value to search for.
 * @return An iterator pointing to the first element in the range that is equal
 * to `val`, or `last` if none is found.
 */
template <typename InIt, typename T>
constexpr InIt find(InIt first, InIt last, const T& val) {
  for (; first != last; ++first) {
    if (*first == val) {
      return first;
    }
  }

  return last;
}

} // namespace psl

/** @} */
