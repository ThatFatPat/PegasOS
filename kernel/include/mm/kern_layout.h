#pragma once

#include <mm/types.h>
#include <stddef.h>

namespace mm {

/**
 * Get the physical address of the kernel.
 */
phys_addr_t get_kernel_base();

/**
 * Get the physical address of the kernel end.
 */
phys_addr_t get_kernel_end();

/**
 * Get the size in memory of our kernel.
 */
size_t get_kernel_size();

} // namespace mm