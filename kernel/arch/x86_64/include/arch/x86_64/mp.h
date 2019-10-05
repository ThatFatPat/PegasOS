/**
 * @addtogroup x86_64
 * @{
 * @file mp.h Multi-processor utilities.
 */

#pragma once

namespace arch::x86_64 {

/**
 * Initialize early control registers on the bootstrap processor.
 */
void init_bsp();

} // namespace arch::x86_64

/** @} */
