#include <arch/kernel_vspace.h>
#include <arch/x86_64/mmu.h>
#include <arch/x86_64/mp.h>

#include <stdint.h>

// Set by boot.S
uintptr_t multiboot_info_phys_addr;

namespace arch {

void early_init() {
  x86_64::init_bsp();
  x86_64::mmu_init_phys_map();

  *reinterpret_cast<uint16_t*>(ARCH_PHYS_MAP_BASE + 0xb8000) = 0x700 | 'I';
}

} // namespace arch
