#include <arch/x86_64/control_regs.h>
#include <stdint.h>

extern "C" void x86_64_early_entry(uintptr_t multiboot_info_paddr) {
  // Display L in light gray
  *reinterpret_cast<uint16_t*>(0xb8000) = 0x700 | 'L';
}
