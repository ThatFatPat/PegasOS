#include <stddef.h>
#include <string.h>

namespace psl {

class string_view {
public:
  using iterator = const char*;
  using const_iterator = iterator;

  constexpr string_view() = default;
  constexpr string_view(const char* ptr) : ptr_{ptr}, size_{strlen(ptr)} {}
  constexpr string_view(const char* ptr, size_t size)
      : ptr_{ptr}, size_{size} {}

  constexpr iterator begin() const { return ptr_; }
  constexpr iterator end() const { return ptr_ + size_; }
  constexpr const_iterator cbegin() const { return ptr_; }
  constexpr const_iterator cend() const { return ptr_ + size_; }

  constexpr const char* data() const { return ptr_; }

  constexpr size_t size() const { return size_; }
  constexpr size_t length() const { return size_; }
  constexpr bool empty() const { return !size_; }

  constexpr const char& front() const { return ptr_[0]; }
  constexpr const char& back() const { return ptr_[size_]; }
  constexpr const char& operator[](size_t i) const { return ptr_[i]; }

private:
  const char* ptr_ = nullptr;
  size_t size_ = 0;
};


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

constexpr bool operator!=(const string_view& lhs, const string_view& rhs) {
  return !(lhs == rhs);
}


inline namespace literals {

constexpr string_view operator""_sv(const char* ptr, size_t len) {
  return {ptr, len};
}

} // namespace literals
} // namespace psl
