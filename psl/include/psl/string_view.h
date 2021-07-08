/**
 * @addtogroup psl
 * @{
 * @file string_view.h
 */

#pragma once

#include <psl/numeric.h>
#include <stddef.h>
#include <string.h>

namespace psl {

/**
 * Primitive `char`-only string view class inspired by std::string_view.
 */
class string_view {
public:
  using iterator = const char*;
  using const_iterator = iterator;

  constexpr string_view() = default;

  /**
   * Construct string_view from a null-terminated string primitive. `size` is
   * set to `strlen(ptr)`.
   *
   * @param ptr The char pointer from which the string should be initialized.
   */
  constexpr string_view(const char* ptr) : ptr_{ptr}, size_{strlen(ptr)} {}

  /**
   * Construct string_view from string primitive with a set size.
   *
   * @param ptr The char pointer from which the string should be initialized.
   * @param size The size of the string specified by `ptr`.
   */
  constexpr string_view(const char* ptr, size_t size)
      : ptr_{ptr}, size_{size} {}


  /**
   *  @return An `iterator` pointing to the beginning of the string.
   */
  [[nodiscard]] constexpr iterator begin() const { return ptr_; }

  /**
   *  @return An `iterator` pointing past the end of the string.
   */
  [[nodiscard]] constexpr iterator end() const { return ptr_ + size_; }

  /**
   *  @return A `const_iterator` pointing to the beginning of the string.
   */
  [[nodiscard]] constexpr const_iterator cbegin() const { return ptr_; }

  /**
   *  @return A `const_iterator` pointing past the end of the string.
   */
  [[nodiscard]] constexpr const_iterator cend() const { return ptr_ + size_; }


  /**
   * @return A pointer to the raw data stored in the string.
   */
  [[nodiscard]] constexpr const char* data() const { return ptr_; }

  /**
   * @return The size of the string.
   */
  [[nodiscard]] constexpr size_t size() const { return size_; }
  /**
   * @return The size of the string.
   */
  [[nodiscard]] constexpr size_t length() const { return size_; }
  /**
   * @return True if the string is empty.
   */
  [[nodiscard]] constexpr bool empty() const { return size_ == 0; }


  /**
   * @return The first character of the stored string.
   */
  [[nodiscard]] constexpr const char& front() const { return ptr_[0]; }
  /**
   * @return The last character of the stored string.
   */
  [[nodiscard]] constexpr const char& back() const { return ptr_[size_]; }
  /**
   * @param i The position of the character to return.
   *
   * @return The i'th character of the stored string.
   */
  constexpr const char& operator[](size_t i) const { return ptr_[i]; }


  /**
   * Retrieve a view of the substring `[pos, min(size(), pos + count))`.
   * @param pos Position of start of substring.
   * @param count Requested size. The returned substring may be shorter if the
   * original string is not long enough.
   * @return View of the requested substring.
   * @note The behavior is undefined if `pos > size()`.
   */
  constexpr string_view substr(size_t pos, size_t count = -1) {
    return {ptr_ + pos, min(count, size_ - pos)};
  }

  /**
   * Advance the start of the view forward by `n` characters.
   * @note The behavior is undefined if `n > size()`.
   */
  constexpr void remove_prefix(size_t n) {
    ptr_ += n;
    size_ -= n;
  }

  /**
   * Move the end of the view back by `n` characters.
   * @note The behavior is undefined if `n > size()`.
   */
  constexpr void remove_suffix(size_t n) { size_ -= n; }

private:
  const char* ptr_ = nullptr;
  size_t size_ = 0;
};


/**
 * Compare two  @ref psl::string_view "string_view" objects for equality.
 */
constexpr bool operator==(const string_view& lhs, const string_view& rhs) {
  if (lhs.size() != rhs.size()) {
    return false;
  }

  for (size_t i = 0; i < lhs.size(); i++) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }

  return true;
}

/**
 * Compare two @ref psl::string_view "string_view" objects for inequality.
 */
constexpr bool operator!=(const string_view& lhs, const string_view& rhs) {
  return !(lhs == rhs);
}


inline namespace literals {
/**
 * Allow the creation of a @ref psl::string_view "string_view" in literal form
 * as `"str"_sv`.
 */
constexpr string_view operator""_sv(const char* ptr, size_t len) {
  return {ptr, len};
}

} // namespace literals
} // namespace psl

/** @} */
