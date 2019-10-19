/**
 * @addtogroup psl
 * @{
 * @file span.h Span implementation.
 */

#pragma once

#include <psl/type_traits.h>

namespace psl {

/**
 * Represents a view into a contiguous sequence of objects.
 */
template <typename T>
class span {
public:
  using element_type = T;
  using value_type = remove_cv_t<element_type>;
  using index_type = size_t;
  using difference_type = ptrdiff_t;
  using pointer = element_type*;
  using const_pointer = const element_type*;
  using reference = element_type&;
  using const_reference = const element_type&;
  using iterator = element_type*;
  using const_iterator = const element_type*;

  /**
   * Construct an empty span.
   */
  constexpr span() noexcept {}

  /**
   * Construct a span from a pointer and element count.
   *
   * @param ptr A pointer to the first element.
   * @param count The number of elements to be represented in the span.
   */
  constexpr span(pointer ptr, index_type count) : data_(ptr), size_(count) {}

  /**
   * Construct a span referring to the contiguous sequence `[first, last)`.
   * @param first A pointer to the first element.
   * @param last A pointer past the last element.
   */
  constexpr span(pointer first, pointer last)
      : data_(first), size_(last - first) {}

  /**
   * Construct a span from a C-style array.
   * @param arr The array to be observed.
   */
  template <size_t N>
  constexpr span(element_type (&arr)[N]) noexcept : data_(arr), size_(N) {}

  /**
   * @return A view of the first `count` elements of the span.
   * @note The behavior is undefined if `count > size()`.
   */
  constexpr span first(index_type count) const {
    return {data_, data_ + count};
  }

  /**
   * @return A view of the last `count` elements of the span.
   * @note The behavior is undefined if `count > size()`.
   */
  constexpr span last(index_type count) const {
    return {data_ + size_ - count, data_ + size_};
  }

  /**
   * @return A view of the subsequence `[offset, offset + count)`.
   * @note The behavior is undefined if `offset > size()` or `offset + count >
   * size()`.
   */
  constexpr span subspan(index_type offset, index_type count) const {
    return {data_ + offset, data_ + offset + count};
  }

  /**
   * @return The number of elements in the span.
   */
  constexpr index_type size() const noexcept { return size_; }

  /**
   * @return True if the span is empty.
   */
  [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }

  /**
   * @param idx The index to retrieve.
   * @return The idx-th element in the span.
   */
  constexpr reference operator[](index_type idx) const { return data_[idx]; }

  /**
   * @return A reference to the first element of the span.
   */
  constexpr reference front() const { return *data_; }

  /**
   * @return A reference to the last element of the span.
   */
  constexpr reference back() const { return *(data_ + size_ - 1); }

  /**
   * @return A pointer to the raw data of the span.
   */
  constexpr pointer data() const noexcept { return data_; }

  /**
   * @return An iterator pointing to the beginning of the span.
   */
  constexpr iterator begin() const noexcept { return data_; }

  /**
   * @return An iterator pointing past the end of the span.
   */
  constexpr iterator end() const noexcept { return data_ + size_; }

  /**
   * @return A const iterator pointing to the beginning of the span.
   */
  constexpr const_iterator cbegin() const noexcept { return data_; }

  /**
   * @return A const iterator pointing past the end of the span.
   */
  constexpr const_iterator cend() const noexcept { return data_ + size_; }

private:
  pointer data_;
  index_type size_;
};

} // namespace psl
