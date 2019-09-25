#include <arch/console.h>

#include <arch/kernel_vspace.h>
#include <stdint.h>
#include <string.h>

namespace arch {
namespace {

using console_word_t = uint16_t;

constexpr uintptr_t console_mem_phys_addr = 0xb8000;

constexpr size_t console_rows = 25;
constexpr size_t console_cols = 80;

struct pos {
  size_t row;
  size_t col;
} curr_pos;

console_word_t* get_console_word(size_t row, size_t col) {
  return reinterpret_cast<console_word_t*>(ARCH_PHYS_MAP_BASE +
                                           console_mem_phys_addr) +
         console_cols * row + col;
}

} // namespace

void console_clear() {
  memset(get_console_word(0, 0), 0,
         console_rows * console_cols * sizeof(console_word_t));
  curr_pos.row = 0;
  curr_pos.col = 0;
}

} // namespace arch
