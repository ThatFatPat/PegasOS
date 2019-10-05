/**
 * @addtogroup mm
 * @{
 * @file phys.h Physical memory manager.
 */

#pragma once

#include <mm/types.h>

namespace mm {

/**
 * Convert a physical address to a virtual address in the phys map.
 * @param paddr The physical address.
 * @return The virtual phys map pointer corresponding to `paddr`.
 */
void* paddr_to_phys_map(phys_addr_t paddr);

} // namespace mm

/** @} */
