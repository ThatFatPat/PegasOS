#include <arch/console.h>

#include <arch/x86_64/ioport.h>
#include <mm/phys.h>

#include <stdint.h>
#include <string.h>

namespace arch {
namespace {

using console_word_t = volatile uint16_t;

constexpr uint16_t port_vga_reg_num = 0x3d4;
constexpr uint16_t port_vga_reg_val = 0x3d5;

constexpr uint8_t vga_reg_cursor_start = 0xa;
constexpr uint8_t vga_reg_cursor_end = 0xb;
constexpr uint8_t vga_reg_cursor_loc_high = 0xe;
constexpr uint8_t vga_reg_cursor_loc_low = 0xf;

constexpr uint8_t cursor_scanline_start = 0x9;
constexpr uint8_t cursor_scanline_end = 0xc;

constexpr uint16_t default_attr = 0x700; // Light gray on black

constexpr mm::phys_addr_t console_mem_phys_addr = 0xb8000;

constexpr size_t console_rows = 25;
constexpr size_t console_cols = 80;

size_t curr_row;
size_t curr_col;

void enable_cursor() {
  x86_64::outb(port_vga_reg_num, vga_reg_cursor_start);
  x86_64::outb(port_vga_reg_val, cursor_scanline_start);

  x86_64::outb(port_vga_reg_num, vga_reg_cursor_end);
  x86_64::outb(port_vga_reg_val, cursor_scanline_end);
}

void set_cursor_pos(size_t row, size_t col) {
  auto pos = static_cast<uint16_t>(row * console_cols + col);

  x86_64::outb(port_vga_reg_num, vga_reg_cursor_loc_low);
  x86_64::outb(port_vga_reg_val, pos & 0xff);

  x86_64::outb(port_vga_reg_num, vga_reg_cursor_loc_high);
  x86_64::outb(port_vga_reg_val, (pos >> 8) & 0xff);
}

void update_cursor() {
  set_cursor_pos(curr_row, curr_col);
}

console_word_t* get_console_word(size_t row, size_t col) {
  return static_cast<console_word_t*>(
             mm::phys_addr_to_phys_map(console_mem_phys_addr)) +
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

void console_init() {
  enable_cursor();
  console_clear();
}

void console_clear() {
  clear_rows(0, console_rows);
  curr_row = 0;
  curr_col = 0;
  update_cursor();
}

void console_putc(char c) {
  do_putc(c);
  update_cursor();
}

void console_puts(psl::string_view str) {
  for (char c : str) {
    do_putc(c);
  }
  update_cursor();
}

} // namespace arch
