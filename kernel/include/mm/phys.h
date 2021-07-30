/**
 * @addtogroup mm
 * @{
 * @file phys.h Physical memory manager.
 */

#pragma once

#include <mm/types.h>

#include <stddef.h>

namespace mm {
/**
 * Initialize the bump allocator.
 * NOTE: CALL BEFORE ALLOCATING MEMORY.
 */
void init_bump_memory();

/**
 * Get the end of allocated bump memory.
 */
phys_addr_t get_bump_memory_end();


/**
 * Allocate bump memory. The returned pointer will be suitably aligned for all
 * non-over-aligned objects.
 * @param size: The amount of memory to allocate.
 */
void* allocate_bump_memory(size_t size);

/**
 * Convert a physical address to a virtual address in the phys map.
 * @param paddr The physical address.
 * @return The virtual phys map pointer corresponding to `paddr`.
 */
void* paddr_to_phys_map(phys_addr_t paddr);

} // namespace mm

/** @} */
