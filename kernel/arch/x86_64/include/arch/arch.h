#pragma once

namespace arch {

void early_init();

[[noreturn]] inline void halt() {
  asm("cli");
  while (true) {
    asm("hlt");
  }
}

} // namespace arch
