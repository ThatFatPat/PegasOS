/**
 * @addtogroup psl
 * @{
 * @file util.h Various general-purpose utilities.
 */

#pragma once

#include <psl/type_traits.h>
#include <stdint.h>

namespace psl {

/**
 * Byte type inspired by `std::byte`. Prefer this type to integer types when
 * performing byte-level access on objects. This type is not an integer, but can
 * be converted with the help of `static_cast`.
 */
enum class [[gnu::may_alias]] byte : uint8_t{};

/**
 * Byte left-shift operator.
 * @note This function only participates in overload
 * resolution if `I` is an integer type.
 */
template <typename I, typename = enable_if_t<is_integral_v<I>>>
constexpr byte operator<<(byte b, I shift) {
  return static_cast<byte>(
      static_cast<uint8_t>(static_cast<unsigned int>(b) << shift));
}

/**
 * Byte right-shift operator.
 * @note This function only participates in overload
 * resolution if `I` is an integer type.
 */
template <typename I, typename = enable_if_t<is_integral_v<I>>>
constexpr byte operator>>(byte b, I shift) {
  return static_cast<byte>(
      static_cast<uint8_t>(static_cast<unsigned int>(b) >> shift));
}

/**
 * Byte bitwise OR operator.
 */
constexpr byte operator|(byte lhs, byte rhs) {
  return static_cast<byte>(static_cast<uint8_t>(
      static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs)));
}

/**
 * Byte bitwise AND operator.
 */
constexpr byte operator&(byte lhs, byte rhs) {
  return static_cast<byte>(static_cast<uint8_t>(
      static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs)));
}

/**
 * Byte bitwise XOR operator.
 */
constexpr byte operator^(byte lhs, byte rhs) {
  return static_cast<byte>(static_cast<uint8_t>(
      static_cast<unsigned int>(lhs) ^ static_cast<unsigned int>(rhs)));
}

/**
 * Byte bitwise negation operator.
 */
constexpr byte operator~(byte b) {
  return static_cast<byte>(static_cast<uint8_t>(~static_cast<unsigned int>(b)));
}


/**
 * Byte left-shift-assignment operator.
 * @note This function only participates in overload
 * resolution if `I` is an integer type.
 */
template <typename I, typename = enable_if_t<is_integral_v<I>>>
constexpr byte& operator>>=(byte& b, I shift) {
  return b = b >> shift;
}

/**
 * Byte right-shift-assignment operator.
 * @note This function only participates in overload
 * resolution if `I` is an integer type.
 */
template <typename I, typename = enable_if_t<is_integral_v<I>>>
constexpr byte& operator<<=(byte& b, I shift) {
  return b = b << shift;
}

/**
 * Byte OR-assignment operator.
 */
constexpr byte& operator|=(byte& lhs, byte rhs) {
  return lhs = lhs | rhs;
}

/**
 * Byte AND-assignment operator.
 */
constexpr byte& operator&=(byte& lhs, byte rhs) {
  return lhs = lhs & rhs;
}

/**
 * Byte XOR-assignment operator.
 */
constexpr byte& operator^=(byte& lhs, byte rhs) {
  return lhs = lhs ^ rhs;
}

} // namespace psl

/** @} */
