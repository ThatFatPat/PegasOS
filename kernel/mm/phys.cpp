#include <mm/phys.h>

#include <arch/kernel_vspace.h>

namespace mm {

void* paddr_to_phys_map(phys_addr_t phys) {
  if (phys >= ARCH_PHYS_MAP_SIZE) {
    return nullptr;
  }
  return reinterpret_cast<void*>(phys + ARCH_PHYS_MAP_BASE);
}

} // namespace mm
