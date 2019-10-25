/**
 * @addtogroup kernel
 * @{
 * @file log.h Kernel logging library
 */

#pragma once

#include <psl/format.h>

namespace log {
namespace impl {

using fmt_output_sink = psl::range_output_sink<char*>;

/**
 * Format and send the message to the current write handler.
 */
void do_write(psl::string_view fmt,
              psl::format_args_ref_t<fmt_output_sink> args);

} // namespace impl


/**
 * The maximum size a formatted log message is allowed to have. Messages longer
 * than this will be truncated.
 */
constexpr size_t max_msg_size = 256;


/**
 * Polymorphic base class for objects wishing to handle output of log messages.
 */
struct write_handler {
  /**
   * Called when a message is written to the log.
   */
  virtual void write(psl::string_view msg) = 0;
};

/**
 * Set the current write handler to `handler`. If `handler` is non-null, it will
 * be called every time a message is logged via @ref write().
 */
void set_write_handler(write_handler* handler);

/**
 * Write a @ref psl_fmt "formatted message" to the log. If the current write
 * handler is null, this is a no-op. If the formatted message is longer than
 * `max_msg_size`, it will be truncated.
 * @param fmt The @ref psl_fmt_str "format string" to use.
 * @param args Formatting arguments.
 */
template <typename... Args>
void write(psl::string_view fmt, const Args&... args) {
  return impl::do_write(fmt,
                        psl::make_format_args<impl::fmt_output_sink>(args...));
}

} // namespace log

/** @} */
