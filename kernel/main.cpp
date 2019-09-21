#include <arch/arch.h>

extern "C" [[noreturn]] void kernel_main() {
  arch::halt();
}
