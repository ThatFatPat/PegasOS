/** @file string_view.h */

#pragma once

#include <stddef.h>
#include <string.h>

namespace psl {

/**
 * Primitive string view class inspired by std::string_view.
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
  constexpr iterator begin() const { return ptr_; }
  /**
   *  @return An `iterator` pointing past the end of the string.
   */
  constexpr iterator end() const { return ptr_ + size_; }
  /**
   *  @return A `const_iterator` pointing to the beginning of the string.
   */
  constexpr const_iterator cbegin() const { return ptr_; }
  /**
   *  @return A `const_iterator` pointing past the end of the string.
   */
  constexpr const_iterator cend() const { return ptr_ + size_; }

  /**
   * @return A pointer to the raw data stored in the string.
   */
  constexpr const char* data() const { return ptr_; }

  /**
   * @return The size of the string.
   */
  constexpr size_t size() const { return size_; }
  /**
   * @return The size of the string.
   */
  constexpr size_t length() const { return size_; }
  /**
   * @return True if the string is empty.
   */
  constexpr bool empty() const { return !size_; }

  /**
   * @return The first character of the stored string.
   */
  constexpr const char& front() const { return ptr_[0]; }
  /**
   * @return The last character of the stored string.
   */
  constexpr const char& back() const { return ptr_[size_]; }
  /**
   * @param i The position of the character to return.
   *
   * @return The i'th character of the stored string.
   */
  constexpr const char& operator[](size_t i) const { return ptr_[i]; }

private:
  const char* ptr_ = nullptr;
  size_t size_ = 0;
};


/**
 * Compare two  @ref psl::string_view "string_views" for equality.
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
 * Compare two @ref psl::string_view "string_views" for inequality.
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
