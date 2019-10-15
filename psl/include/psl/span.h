/**
 * @addtogroup psl
 * @{
 * @file span.h Span implementation.
 */

#pragma once

namespace psl {

template <class T>
class span {
public:
  using element_type = T;
  using value_type = remove_cv_t(element_type);
  using index_type = size_t;
  using difference_type = ptrdiff_t;
  using pointer = element_type*;
  using const_pointer = const element_type*;
  using reference = element_type&;
  using const_reference = const element_type&;
  using iterator = element_type*;
  using const_iterator = const element_type*;

  constexpr span() noexcept {}
  constexpr span(pointer ptr, index_type count) : data_(ptr), size_(count) {}
  constexpr span(pointer first, pointer last)
      : data_(first), size_(last - first) {}
  template <size_t N>
  constexpr span(element_type (&arr)[N]) noexcept : data_(arr), size_(N) {}

  constexpr span<element_type, dynamic_extent> first(index_type count) const {
    return span(data_, data_ + count);
  }
  constexpr span<element_type, dynamic_extent> last(index_type count) const {
    return span(data_ + size_ - count, data_ + size_);
  }
  constexpr span<element_type, dynamic_extent> subspan(index_type offset,
                                                       index_type count) const {
    return span(data_ + offset, data_ + offset + count);
  }

  constexpr index_type size() const noexcept { return size_; }
  constexpr index_type size_bytes() const noexcept {
    return size_ * sizeof(value_type);
  }
  [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }

  constexpr reference operator[](index_type idx) const { return data_[idx]; }
  constexpr reference front() const { return *data_; }
  constexpr reference back() const { return *(data_ + size_ - 1); }
  constexpr pointer data() const noexcept { return data_; }

  constexpr iterator begin() const noexcept { return data_; }
  constexpr iterator end() const noexcept { return data_ + size_; }
  constexpr const_iterator cbegin() const noexcept {
    return const_cast<element_type>(data_);
  }
  constexpr const_iterator cend() const noexcept {
    return const_cast<element_type>(data_ + size_);
  }

  friend constexpr iterator begin(span s) noexcept { return s.begin(); }
  friend constexpr iterator end(span s) noexcept { return s.end(); }

private:
  pointer data_;
  index_type size_;
};

} // namespace psl
