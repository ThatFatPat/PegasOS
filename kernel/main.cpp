#include <arch/arch.h>
#include <arch/console.h>

extern "C" [[noreturn]] void kernel_main() {
  arch::early_init();
  arch::console_puts("Welcome to PegasOS!\n");
  arch::halt();
}
