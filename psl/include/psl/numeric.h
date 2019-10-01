#pragma once

#include <psl/type_traits.h>

namespace psl {

template <typename T>
constexpr const T& min(const T& a, const T& b) {
  return a < b ? a : b;
}

template <typename T>
constexpr const T& max(const T& a, const T& b) {
  return a > b ? a : b;
}

template <typename T>
constexpr const T& clamp(const T& val, const T& low, const T& high) {
  return ::psl::min(::psl::max(val, low), high);
}

template <typename T, typename U>
constexpr common_type_t<T, U> round_down(T val, U block) {
  return val - val % block;
}

template <typename T, typename U>
constexpr common_type_t<T, U> round_up(T val, U block) {
  return ::psl::round_down(val + block - 1, block);
}

} // namespace psl
