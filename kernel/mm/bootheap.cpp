/**
 * @addtogroup mm
 * @{
 * @file bootheap.cpp Early heap implementation.
 */

#include <mm/bootheap.h>

#include <mm/phys.h>
#include <mm/types.h>

namespace mm {
namespace {

constexpr size_t bootheap_max_size = 0x1000000; // 16MiB

phys_addr_t bootheap_start;
phys_addr_t bootheap_cursor;

} // namespace

void bootheap_init() {
  extern char _kern_phys_end[]; // Defined in the linker script
  bootheap_start = reinterpret_cast<phys_addr_t>(_kern_phys_end);
  bootheap_cursor = reinterpret_cast<phys_addr_t>(_kern_phys_end);
}

void* bootheap_malloc(size_t size, size_t align) {
  phys_addr_t ptr = (bootheap_cursor + align - 1) & -align;

  if (size > bootheap_max_size ||
      ptr - bootheap_start > bootheap_max_size - size) {
    return nullptr;
  }

  bootheap_cursor = ptr + size;
  return paddr_to_phys_map(ptr);
}

} // namespace mm

/** @} */