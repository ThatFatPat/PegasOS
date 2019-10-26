/**
 * @addtogroup x86_64
 * @{
 * @file console.h Console API
 */

#pragma once

#include <lib/log.h>
#include <psl/string_view.h>

namespace arch {

/**
 * Initialize the console, including clearing the screen and setting up the
 * cursor position.
 */
void console_init();

/**
 * Clear the console and reset the cursor.
 */
void console_clear();

/**
 * Write a character to the console.
 * @param c The character to be written.
 */
void console_putc(char c);

/**
 * Write a string to the console.
 * @param str The string to be written.
 */
void console_puts(psl::string_view str);


/**
 * Initialize logging for the console. Sets write handler for logging library
 * implementation.
 */
void console_log_init();

} // namespace arch
/** @} */
