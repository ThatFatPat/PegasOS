/**
 * @addtogroup kernel
 * @{
 * @file main.cpp
 */

#include <arch/arch.h>
#include <arch/console.h>

/**
 * Main kernel entry point.
 */
extern "C" [[noreturn]] void kernel_main() {
  arch::early_init();
  arch::console_puts("Welcome to PegasOS!\n");
  arch::halt();
}

/** @} */
