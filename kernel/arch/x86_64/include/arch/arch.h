#pragma once

namespace arch {

inline void halt() {
  asm("cli");
  while (true) {
    asm("hlt");
  }
}

} // namespace arch
