/**
 * @addtogroup psl
 * @{
 * @file iterator.h Iterator utilites, inspired by &lt;iterator&gt;.
 */

#pragma once

#include <stddef.h>

namespace psl {

/**
 * @return The size of the container `cont`.
 */
template <typename C>
constexpr auto size(const C& cont) -> decltype(cont.size()) {
  return cont.size();
}

/**
 * @return The size of the specified array.
 */
template <typename T, size_t N>
constexpr size_t size(const T (&/*arr*/)[N]) {
  return N;
}


/**
 * Retrieve a pointer to the data stored in the container `cont`, if it is a
 * contiguous container.
 * @return A pointer to the data stored in `cont`.
 */
template <typename C>
constexpr auto data(const C& cont) -> decltype(cont.data()) {
  return cont.data();
}

/**
 * Retrieve a pointer to the data stored in the array `arr`.
 */
template <typename T, size_t N>
constexpr T* data(T (&arr)[N]) {
  return arr;
}


/**
 * Retrieve an iterator pointing to the beginning of `cont`'s contents.
 */
template <typename C>
constexpr auto begin(C& cont) -> decltype(cont.begin()) {
  return cont.begin();
}

/**
 * Retrieve an iterator pointing to the beginning of array `arr`.
 */
template <typename T, size_t N>
constexpr T* begin(T (&arr)[N]) {
  return arr;
}


/**
 * Retrieve an iterator pointing past the end of `cont`'s contents.
 */
template <typename C>
constexpr auto end(C& cont) -> decltype(cont.end()) {
  return cont.end();
}

/**
 * Retrieve an iterator pointing past the end of array `arr`.
 */
template <typename T, size_t N>
constexpr T* end(T (&arr)[N]) {
  return arr + N;
}

} // namespace psl

/** @} */
