/**
 * @addtogroup x86_64
 * @{
 * @file arch.h General architecture-specific header for x86_64 architecture.
 */

#pragma once

namespace arch {

/**
 * Perform early architecture-specific initialization, including processing
 * bootloader data and setting up the map of physical memory.
 */
void early_init();

/**
 * Halt the current processor.
 */
[[noreturn]] inline void halt() {
  asm("cli");
  while (true) {
    asm("hlt");
  }
}

} // namespace arch

/** @} */
