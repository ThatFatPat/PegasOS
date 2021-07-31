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

/**
 * Search forward for an element satisfying the given predicate in `[first,
 * last)`.
 * @param first Input iterator indicating start of range.
 * @param last Input iterator past end of range.
 * @param pred Predicate to use. The expression `pred(v)` must be convertible to
 * `bool`, where `v` is a value of the iterator's value type.
 * @return An iterator pointing to the first element in the range that satisfies
 * `pred`, or `last` if none is found.
 */
template <typename InIt, typename P>
constexpr InIt find_if(InIt first, InIt last, P pred) {
  for (; first != last; ++first) {
    if (pred(*first)) {
      return first;
    }
  }

  return last;
}


} // namespace psl

/** @} */
