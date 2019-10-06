/**
 * @addtogroup x86_64
 * @{
 * @file console.cpp Console implementation.
 */

#include <arch/console.h>

#include <arch/x86_64/ioport.h>
#include <mm/phys.h>

#include <stdint.h>
#include <string.h>

namespace arch {
namespace {

/**
 * Represents a single cell in console video memory. Each cell contains both the
 * character to display as well as its attributes.
 */
using console_cell_t = volatile uint16_t;


/**
 * I/O port used to specify the register number when communicating with VGA
 * hardware.
 */
constexpr uint16_t port_vga_reg_num = 0x3d4;

/**
 * I/O port used to specify the register value when communicating with VGA
 * hardware.
 */
constexpr uint16_t port_vga_reg_val = 0x3d5;


/**
 * VGA register containing the cursor's initial scanline.
 */
constexpr uint8_t vga_reg_cursor_start = 0xa;

/**
 * VGA register containing the cursor's final scanline.
 */
constexpr uint8_t vga_reg_cursor_end = 0xb;

/**
 * VGA register containing the high byte of the cursor location.
 */
constexpr uint8_t vga_reg_cursor_loc_high = 0xe;

/**
 * VGA register containing the low byte of the cursor location.
 */
constexpr uint8_t vga_reg_cursor_loc_low = 0xf;


/**
 * Value used for the VGA cursor's initial scanline. This value can range
 * between 0 and 15, with 0 being the top of the character and 15 its bottom.
 */
constexpr uint8_t cursor_scanline_start = 0x0;

/**
 * Value used for the VGA cursor's final scanline. This value can range
 * between 0 and 15, with 0 being the top of the character and 15 its bottom.
 */
constexpr uint8_t cursor_scanline_end = 0xf;


/**
 * Default attributes used when displaying text. Currently light gray on black.
 */
constexpr uint16_t default_attr = 0x700;


/**
 * Physical address of console video memory.
 */
constexpr mm::phys_addr_t console_mem_paddr = 0xb8000;


/**
 * Number of rows in the VGA console.
 */
constexpr size_t console_rows = 25;

/**
 * Number of columns in the VGA console.
 */
constexpr size_t console_cols = 80;


/**
 * Current row in the console.
 */
size_t curr_row;

/**
 * Current column in the console
 */
size_t curr_col;


/**
 * Enable the VGA cursor
 */
void enable_cursor() {
  x86_64::outb(port_vga_reg_num, vga_reg_cursor_start);
  x86_64::outb(port_vga_reg_val, cursor_scanline_start);

  x86_64::outb(port_vga_reg_num, vga_reg_cursor_end);
  x86_64::outb(port_vga_reg_val, cursor_scanline_end);
}

/**
 * Update the VGA cursor's position.
 * @param row The row at which to place the cursor.
 * @param col The column at which to place the cursor.
 */
void set_cursor_pos(size_t row, size_t col) {
  auto pos = static_cast<uint16_t>(row * console_cols + col);

  x86_64::outb(port_vga_reg_num, vga_reg_cursor_loc_low);
  x86_64::outb(port_vga_reg_val, pos & 0xff);

  x86_64::outb(port_vga_reg_num, vga_reg_cursor_loc_high);
  x86_64::outb(port_vga_reg_val, (pos >> 8) & 0xff);
}

/**
 * Update the VGA cursor's position to reflect the current row and column.
 */
void update_cursor() {
  set_cursor_pos(curr_row, curr_col);
}

/**
 * Retrieve a pointer to the specified cell in console video memory.
 */
console_cell_t* get_console_cell(size_t row, size_t col) {
  return static_cast<console_cell_t*>(
             mm::paddr_to_phys_map(console_mem_paddr)) +
         console_cols * row + col;
}

/**
 * Clear `count` rows of the console starting at `row`.
 * @param row Row at which to start clearing.
 * @param count Number of rows to clear.
 */
void clear_rows(size_t row, size_t count) {
  for (size_t i = 0; i < console_cols * count; i++) {
    *get_console_cell(row, i) = default_attr;
  }
}

/**
 * Scroll the console by the specified number of lines, creating more space at
 * the bottom of the screen.
 * @param lines Number of lines to scroll.
 */
void scroll(size_t lines) {
  size_t move_region_rows = console_rows - lines;
  size_t region_size = move_region_rows * console_cols;

  // Note: can't memmove as console_cell_t is volatile
  for (size_t i = 0; i < region_size; i++) {
    *get_console_cell(0, i) = *get_console_cell(lines, i);
  }

  clear_rows(move_region_rows, lines);
}

/**
 * Advance the current position to the next line, scrolling if necessary.
 * @note This function does not update the VGA cursor.
 */
void advance_line() {
  curr_col = 0;
  if (curr_row < console_rows - 1) {
    curr_row++;
  } else {
    scroll(1);
  }
}

/**
 * Advance the current position to the next column, wrapping and scrolling if
 * necessary.
 * @note This function does not update the VGA cursor.
 */
void advance_col() {
  if (curr_col < console_cols - 1) {
    curr_col++;
  } else {
    advance_line();
  }
}

/**
 * Output the specified character to console video memory, wrapping and
 * scrolling if necessary.
 * @note This function does not update the VGA cursor.
 */
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
      *get_console_cell(curr_row, curr_col) = c | default_attr;
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

/** @} */
