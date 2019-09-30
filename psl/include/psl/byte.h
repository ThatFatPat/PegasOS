#include <stdint.h>

namespace psl {

enum class [[gnu::may_alias]] byte : uint8_t{};

template <typename I>
constexpr byte operator<<(byte b, I shift) {
  return static_cast<byte>(
      static_cast<uint8_t>(static_cast<unsigned int>(b) << shift));
}

template <typename I>
constexpr byte operator>>(byte b, I shift) {
  return static_cast<byte>(
      static_cast<uint8_t>(static_cast<unsigned int>(b) >> shift));
}

constexpr byte operator|(byte lhs, byte rhs) {
  return static_cast<byte>(static_cast<uint8_t>(
      static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs)));
}

constexpr byte operator&(byte lhs, byte rhs) {
  return static_cast<byte>(static_cast<uint8_t>(
      static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs)));
}

constexpr byte operator^(byte lhs, byte rhs) {
  return static_cast<byte>(static_cast<uint8_t>(
      static_cast<unsigned int>(lhs) ^ static_cast<unsigned int>(rhs)));
}

constexpr byte operator~(byte b) {
  return static_cast<byte>(static_cast<uint8_t>(~static_cast<unsigned int>(b)));
}


template <typename I>
constexpr byte& operator>>=(byte& b, I shift) {
  return b = b >> shift;
}

template <typename I>
constexpr byte& operator<<=(byte& b, I shift) {
  return b = b << shift;
}

constexpr byte& operator|=(byte& lhs, byte rhs) {
  return lhs = lhs | rhs;
}

constexpr byte& operator&=(byte& lhs, byte rhs) {
  return lhs = lhs & rhs;
}

constexpr byte& operator^=(byte& lhs, byte rhs) {
  return lhs = lhs ^ rhs;
}

} // namespace psl
