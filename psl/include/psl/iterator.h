#pragma once

#include <stddef.h>

namespace psl {

template <typename C>
constexpr auto size(const C& cont) -> decltype(cont.size()) {
  return cont.size();
}

template <typename T, size_t N>
constexpr size_t size(const T (&)[N]) {
  return N;
}


template <typename C>
constexpr auto data(const C& cont) -> decltype(cont.data()) {
  return cont.data();
}

template <typename T, size_t N>
constexpr T* data(T (&arr)[N]) {
  return arr;
}


template <typename C>
constexpr auto begin(C& cont) -> decltype(cont.begin()) {
  return cont.begin();
}

template <typename T, size_t N>
constexpr T* begin(T (&arr)[N]) {
  return arr;
}


template <typename C>
constexpr auto end(C& cont) -> decltype(cont.end()) {
  return cont.end();
}

template <typename T, size_t N>
constexpr T* end(T (&arr)[N]) {
  return arr + N;
}

} // namespace psl
