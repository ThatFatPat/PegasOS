/**
 * @addtogroup psl
 * @{
 * @file numeric.h Numeric utilities.
 */

#pragma once

#include <psl/type_traits.h>

namespace psl {

/**
 * @return The minimum of `a` and `b`.
 */
template <typename T>
constexpr const T& min(const T& a, const T& b) {
  return a < b ? a : b;
}

/**
 * @return The maximum of `a` and `b`.
 */
template <typename T>
constexpr const T& max(const T& a, const T& b) {
  return a > b ? a : b;
}

/**
 * Clamp `val` to the range [`low`, `high`].
 * @note The behavior is undefined if `low > high`.
 */
template <typename T>
constexpr const T& clamp(const T& val, const T& low, const T& high) {
  return ::psl::min(::psl::max(val, low), high);
}

/**
 * Round `val` down to the nearest multiple of `block`.
 * @note The behavior is undefined if `block` is 0.
 * @param val The value to round.
 * @param block The block size to round down to.
 */
template <typename T, typename U>
constexpr common_type_t<T, U> round_down(T val, U block) {
  return val - val % block;
}

/**
 * Round `val` up to the nearest multiple of `block`.
 * @note The behavior is undefined if `block` is 0.
 * @param val The value to round.
 * @param block The block size to round up to.
 */
template <typename T, typename U>
constexpr common_type_t<T, U> round_up(T val, U block) {
  return ::psl::round_down(val + block - 1, block);
}

} // namespace psl

/** @} */
