/**
 * @addtogroup kernel
 * @{
 * @file main.cpp
 */

#include <arch/arch.h>
#include <lib/log.h>

/**
 * Main kernel entry point.
 */
extern "C" [[noreturn]] void kernel_main() {
  arch::early_init();
  log::write("Welcome to PegasOS!");
  arch::halt();
}

/** @} */
