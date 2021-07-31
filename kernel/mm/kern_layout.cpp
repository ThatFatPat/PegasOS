#include <mm/kern_layout.h>

extern "C" {
char _kern_phys_start;
char _kern_phys_end;
}

namespace mm {

phys_addr_t get_kernel_base() {
  return reinterpret_cast<phys_addr_t>(&_kern_phys_start);
}

phys_addr_t get_kernel_end() {
  return reinterpret_cast<phys_addr_t>(&_kern_phys_end);
}

size_t get_kernel_size() {
  return get_kernel_end() - get_kernel_base();
}

} // namespace mm
