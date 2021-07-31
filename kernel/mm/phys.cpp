/**
 * @addtogroup mm
 * @{
 * @file phys.cpp
 */

#include "mm/kern_layout.h"
#include <mm/phys.h>
#include <psl/numeric.h>

#include <arch/kernel_vspace.h>

#include <stddef.h>
#include <stdint.h>

namespace mm {
namespace {

phys_addr_t bump_mem_end_ptr;

constexpr size_t BUMP_MEMORY_ALIGNMENT = alignof(max_align_t);

} // namespace

void init_bump_memory() {
  bump_mem_end_ptr = get_kernel_end();
}

phys_addr_t get_bump_memory_end() {
  return bump_mem_end_ptr;
}

void* allocate_bump_memory(size_t size) {
  phys_addr_t ret = bump_mem_end_ptr;
  bump_mem_end_ptr =
      psl::round_up(bump_mem_end_ptr + size, BUMP_MEMORY_ALIGNMENT);
  return paddr_to_phys_map(ret);
}

/**
 * Manages a contiguous range of memory.
 */
class PhysRange {
public:
  explicit PhysRange(phys_addr_t paddr, size_t size);
};

class PhysMM {
public:
  explicit PhysMM(PhysRange* ranges, size_t range_count);
};

void* paddr_to_phys_map(phys_addr_t paddr) {
  if (paddr >= ARCH_PHYS_MAP_SIZE) {
    return nullptr;
  }
  return reinterpret_cast<void*>(paddr + ARCH_PHYS_MAP_BASE);
}

} // namespace mm

/** @} */
