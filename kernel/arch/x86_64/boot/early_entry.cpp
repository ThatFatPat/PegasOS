#include <arch/kernel_addrspace.h>
#include <arch/x86_64/control_regs.h>
#include <arch/x86_64/mmu.h>
#include <arch/x86_64/mp.h>
#include <stdint.h>

extern "C" [[noreturn]] void x86_64_early_entry(
    uintptr_t multiboot_info_paddr) {
  arch::x86_64::init_bsp();

  // Display L in light gray
  *reinterpret_cast<uint16_t*>(ARCH_PHYS_MAP_BASE + 0xb8000) = 0x700 | 'L';

  while (true) {
    asm("hlt");
  }
}
