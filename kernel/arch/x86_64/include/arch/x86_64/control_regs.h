#pragma once

// Control registers

#define X86_64_CR0_PE (1 << 0)
#define X86_64_CR0_WP (1 << 16)
#define X86_64_CR0_PG (1 << 31)

#define X86_64_CR4_PAE (1 << 5)
#define X86_64_CR4_OSFXSR (1 << 9)
#define X86_64_CR4_OXMMEXCPT (1 << 10)

// MSRs

#define X86_64_MSR_EFER 0xC0000080
#define X86_64_MSR_EFER_SCE (1 << 0)
#define X86_64_MSR_EFER_LME (1 << 8)
#define X86_64_MSR_EFER_NXE (1 << 11)

#ifndef __ASSEMBLER__

#include <stdint.h>

namespace arch::x86_64 {

inline uint64_t read_cr0() {
  uint64_t ret;
  asm volatile("mov %%cr0, %0" : "=r"(ret));
  return ret;
}

inline void write_cr0(uint64_t value) {
  asm volatile("mov %0, %%cr0" : : "r"(value));
}

inline uint64_t read_cr2() {
  uint64_t ret;
  asm volatile("mov %%cr2, %0" : "=r"(ret));
  return ret;
}

inline uint64_t read_cr3() {
  uint64_t ret;
  asm volatile("mov %%cr3, %0" : "=r"(ret));
  return ret;
}

inline void write_cr3(uint64_t value) {
  asm volatile("mov %0, %%cr3" : : "r"(value) : "memory");
}

inline uint64_t read_cr4() {
  uint64_t ret;
  asm volatile("mov %%cr4, %0" : "=r"(ret));
  return ret;
}

inline void write_cr4(uint64_t value) {
  asm volatile("mov %0, %%cr4" : : "r"(value));
}

inline uint64_t read_msr(uint32_t id) {
  uint32_t low, high;
  asm volatile("rdmsr" : "=d"(high), "=a"(low) : "c"(id));
  return static_cast<uint64_t>(high) << 32 | low;
}

inline void write_msr(uint32_t id, uint64_t value) {
  uint32_t low = static_cast<uint32_t>(value);
  uint32_t high = static_cast<uint32_t>(value >> 32);

  asm volatile("wrmsr" : : "c"(id), "d"(high), "a"(low));
}

} // namespace arch::x86_64

#endif
