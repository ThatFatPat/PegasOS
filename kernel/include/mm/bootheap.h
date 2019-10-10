/**
 * @addtogroup mm
 * @{
 * @file bootheap.h Early heap, used before the memory manager and heap are
 * initialized. Allocates memory off the end of the kernel image.
 */

#pragma once

#include <stddef.h>

namespace mm {

/**
 * Initialize the boot heap to start at the end of the physical kernel image.
 */
void bootheap_init();

/**
 * Allocate a memory region with the specified size and alignment from the boot
 * heap.
 *
 * @return A pointer to the allocated memory on success, `nullptr` on failure
 * (Not enough space).
 */
void* bootheap_malloc(size_t size, size_t align = alignof(max_align_t));

} // namespace mm

/** @} */
