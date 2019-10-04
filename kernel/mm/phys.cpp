#include <mm/phys.h>

#include <arch/kernel_vspace.h>

namespace mm {

void* paddr_to_phys_map(phys_addr_t paddr) {
  if (paddr >= ARCH_PHYS_MAP_SIZE) {
    return nullptr;
  }
  return reinterpret_cast<void*>(paddr + ARCH_PHYS_MAP_BASE);
}

} // namespace mm
