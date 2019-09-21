#pragma once

namespace arch {

[[noreturn]] inline void halt() {
  asm("cli");
  while (true) {
    asm("hlt");
  }
}

} // namespace arch
