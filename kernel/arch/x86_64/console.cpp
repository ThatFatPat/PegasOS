#include <arch/console.h>

#include <arch/kernel_vspace.h>
#include <stdint.h>
#include <string.h>

namespace arch {
namespace {

using console_word_t = volatile uint16_t;

constexpr console_word_t default_attr = 0x700; // Light gray on black

constexpr uintptr_t console_mem_phys_addr = 0xb8000;

constexpr size_t console_rows = 25;
constexpr size_t console_cols = 80;

size_t curr_row;
size_t curr_col;

console_word_t* get_console_word(size_t row, size_t col) {
  return reinterpret_cast<console_word_t*>(ARCH_PHYS_MAP_BASE +
                                           console_mem_phys_addr) +
         console_cols * row + col;
}

void clear_rows(size_t row, size_t count) {
  for (size_t i = 0; i < console_cols * count; i++) {
    *get_console_word(row, i) = default_attr;
  }
}

void scroll(size_t lines) {
  size_t move_region_rows = console_rows - lines;
  size_t region_size = move_region_rows * console_cols;

  // Note: can't memmove as console_word_t is volatile
  for (size_t i = 0; i < region_size; i++) {
    *get_console_word(0, i) = *get_console_word(lines, i);
  }

  clear_rows(move_region_rows, lines);
}

void advance_line() {
  curr_col = 0;
  if (curr_row < console_rows - 1) {
    curr_row++;
  } else {
    scroll(1);
  }
}

void advance_col() {
  if (curr_col < console_cols - 1) {
    curr_col++;
  } else {
    advance_line();
  }
}

void do_putc(char c) {
  switch (c) {
  case '\r':
    curr_col = 0;
    break;
  case '\n':
    advance_line();
    break;
  default:
    if (c >= ' ') {
      *get_console_word(curr_row, curr_col) = c | default_attr;
      advance_col();
    }
  }
}

} // namespace

void console_clear() {
  clear_rows(0, console_rows);
  curr_row = 0;
  curr_col = 0;
}

void console_write(psl::string_view str) {
  for (char c : str) {
    do_putc(c);
  }
}

} // namespace arch
