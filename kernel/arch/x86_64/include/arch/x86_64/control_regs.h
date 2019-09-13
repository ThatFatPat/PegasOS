#pragma once

// Control registers

#define X86_64_CR0_PE (1 << 0)
#define X86_64_CR0_WP (1 << 16)
#define X86_64_CR0_PG (1 << 31)

#define X86_64_CR4_PAE (1 << 5)

// MSRs

#define X86_64_MSR_EFER 0xC0000080
#define X86_64_MSR_EFER_LME (1 << 8)

#ifndef __ASSEMBLER__

#include <stdint.h>

namespace arch::x86_64 {

[[gnu::always_inline]] inline uint64_t read_cr0() {
  uint64_t ret;
  asm("mov %%cr0, %0" : "=r"(ret));
  return ret;
}

[[gnu::always_inline]] inline void write_cr0(uint64_t value) {
  asm("mov %0, %%cr0" : : "r"(value));
}

[[gnu::always_inline]] inline uint64_t read_cr2() {
  uint64_t ret;
  asm("mov %%cr2, %0" : "=r"(ret));
  return ret;
}

[[gnu::always_inline]] inline uint64_t read_cr3() {
  uint64_t ret;
  asm("mov %%cr3, %0" : "=r"(ret));
  return ret;
}

[[gnu::always_inline]] inline void write_cr3(uint64_t value) {
  asm("mov %0, %%cr3" : : "r"(value) : "memory");
}

[[gnu::always_inline]] inline uint64_t read_cr4() {
  uint64_t ret;
  asm("mov %%cr4, %0" : "=r"(ret));
  return ret;
}

[[gnu::always_inline]] inline void write_cr4(uint64_t value) {
  asm("mov %0, %%cr4" : : "r"(value));
}

[[gnu::always_inline]] inline uint64_t read_msr(uint32_t id) {
  uint32_t low, high;
  asm("rdmsr" : "=d"(high), "=a"(low) : "c"(id));
  return static_cast<uint64_t>(high) << 32 | low;
}

[[gnu::always_inline]] inline void write_msr(uint32_t id, uint64_t value) {
  uint32_t low = static_cast<uint32_t>(value);
  uint32_t high = static_cast<uint32_t>(value >> 32);

  asm("wrmsr" : : "c"(id), "d"(high), "a"(low));
}

} // namespace arch::x86_64

#endif
