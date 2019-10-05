/** @file ioport.h Processor I/O port helpers. */

#pragma once

#include <stdint.h>

namespace arch::x86_64 {

/**
 * Read byte from a processor I/O port.
 * @param port The port to be accessed.
 * @return A byte read from `port`.
 */
inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
  return ret;
}

/**
 * Write byte to a processor I/O port.
 * @param port The port to be accessed.
 * @param val The byte to be written.
 */
inline void outb(uint16_t port, uint8_t val) {
  asm volatile("outb %0, %1" : : "a"(val), "dN"(port));
}

/**
 * Read word from a processor I/O port.
 * @param port The port to be accessed.
 * @return A word read from `port`.
 */
inline uint16_t inw(uint16_t port) {
  uint16_t ret;
  asm volatile("inw %1, %0" : "=a"(ret) : "dN"(port));
  return ret;
}

/**
 * Write word to a processor I/O port.
 * @param port The port to be accessed.
 * @param val The word to be written.
 */
inline void outw(uint16_t port, uint16_t val) {
  asm volatile("outw %0, %1" : : "a"(val), "dN"(port));
}

/**
 * Read 32-bit value from a processor I/O port.
 * @param port The port to be accessed.
 * @return A word read from `port`.
 */
inline uint32_t inl(uint16_t port) {
  uint32_t ret;
  asm volatile("inl %1, %0" : "=a"(ret) : "dN"(port));
  return ret;
}

/**
 * Write a 32-bit value to a processor I/O port.
 * @param port The port to be accessed.
 * @param val The value to be written.
 */
inline void outl(uint16_t port, uint32_t val) {
  asm volatile("outl %0, %1" : : "a"(val), "dN"(port));
}

} // namespace arch::x86_64
