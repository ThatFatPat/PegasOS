/**
 * @addtogroup kernel
 * @{
 * @file log.cpp Kernel logging library implementation.
 */

#include <lib/log.h>

namespace log {
namespace {

/**
 * The current write handler.
 * @todo Protect with a spinlock?
 */
write_handler* curr_write_handler;

} // namespace

namespace impl {

void do_write(psl::string_view fmt,
              psl::format_args_ref_t<fmt_output_sink> args) {
  if (!curr_write_handler) {
    return;
  }

  char buf[max_msg_size];
  fmt_output_sink sink{psl::begin(buf), psl::end(buf)};
  psl::vformat(sink, fmt, args);
  size_t formatted_size = sink.cursor() - psl::begin(buf);

  curr_write_handler->write({buf, formatted_size});
}

} // namespace impl


void set_write_handler(write_handler* handler) {
  curr_write_handler = handler;
}

} // namespace log

/** } */
