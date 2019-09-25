#include <arch/console.h>
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
  console_init();
}

} // namespace arch
