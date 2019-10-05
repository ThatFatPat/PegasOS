/**
 * @addtogroup mm
 * @{
 * @file types.h Memory manager base types.
 */

#pragma once

#include <stdint.h>

namespace mm {

/** Represents a physical address. */
using phys_addr_t = uintptr_t;

/** Represents a virtual address. */
using virt_addr_t = uintptr_t;

} // namespace mm

/** @} */
