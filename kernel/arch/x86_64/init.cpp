/**
 * @addtogroup x86_64
 * @{
 * @file init.cpp Architecture-specific initialization.
 */

#include <arch/console.h>
#include <arch/x86_64/mmu.h>
#include <arch/x86_64/mp.h>
#include <arch/x86_64/multiboot2.h>
#include <lib/log.h>
#include <mm/phys.h>

#include <psl/numeric.h>
#include <psl/util.h>
#include <stdint.h>

/**
 * Physical address of the multiboot data. Set in boot.S.
 */
mm::phys_addr_t multiboot_info_paddr;

namespace arch {
namespace {

/**
 * Process the specified multiboot tag.
 * @param type The multiboot tag type.
 * @param raw_tag The tag data.
 */
void process_multiboot_tag(uint32_t type, const void* raw_tag) {
  switch (type) {
  case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
    const auto* tag = static_cast<const multiboot_tag_string*>(raw_tag);
    log::write("Booted by: {}", tag->string);
    break;
  }
}

/**
 * Process the multiboot info provided by the bootloader.
 */
void process_multiboot_info() {
  const auto* info = static_cast<const psl::byte*>(
                         mm::paddr_to_phys_map(multiboot_info_paddr)) +
                     2 * sizeof(uint32_t); // Skip size and reserved fields

  uint32_t processed = 0;
  multiboot_tag tag;

  do {
    memcpy(&tag, info + processed, sizeof(tag));
    process_multiboot_tag(tag.type, info + processed);
    processed = psl::round_up(processed + tag.size, MULTIBOOT_TAG_ALIGN);
  } while (tag.type);
}

} // namespace

void early_init() {
  x86_64::init_bsp();
  x86_64::mmu_init_phys_map();
  console_init();
  console_install_log_handler();
  process_multiboot_info();
}

} // namespace arch

/** @} */
