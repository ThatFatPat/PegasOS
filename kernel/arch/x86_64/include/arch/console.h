#include <psl/string_view.h>

namespace arch {

void console_init();
void console_clear();

void console_putc(char c);
void console_puts(psl::string_view str);

} // namespace arch