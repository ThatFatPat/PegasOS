#pragma once

#include <stddef.h>

namespace ksl {

template <typename T, size_t N>
constexpr size_t size(const T (&)[N]) {
  return N;
}

} // namespace ksl
