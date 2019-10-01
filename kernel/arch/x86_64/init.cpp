#include <arch/console.h>
#include <arch/kernel_vspace.h>
#include <arch/x86_64/mmu.h>
#include <arch/x86_64/mp.h>
#include <arch/x86_64/multiboot2.h>

#include <psl/numeric.h>
#include <psl/util.h>
#include <stdint.h>

// Set by boot.S
uintptr_t multiboot_info_phys_addr;

namespace arch {
namespace {

void process_multiboot_tag(uint32_t type, const void* raw_tag) {
  switch (type) {
  case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
    const auto* tag = static_cast<const multiboot_tag_string*>(raw_tag);
    console_puts("Booted by: ");
    console_puts(tag->string);
    console_putc('\n');
    break;
  }
}

void process_multiboot_info() {
  const auto* info = reinterpret_cast<const psl::byte*>(
      multiboot_info_phys_addr + ARCH_PHYS_MAP_BASE);

  // Note: this is okay as info is properly aligned and psl::byte allows
  // aliasing.
  uint32_t info_size = *reinterpret_cast<const uint32_t*>(info);
  info += 2 * sizeof(uint32_t); // Skip size and reserved fields

  for (uint32_t processed = 0; processed < info_size;) {
    multiboot_tag tag;
    memcpy(&tag, info + processed, sizeof(tag));
    process_multiboot_tag(tag.type, info + processed);
    processed = psl::round_up(processed + tag.size, MULTIBOOT_TAG_ALIGN);
  }
}

} // namespace

void early_init() {
  x86_64::init_bsp();
  x86_64::mmu_init_phys_map();
  console_init();
  process_multiboot_info();
}

} // namespace arch
