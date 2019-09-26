#pragma once

#include <arch/x86_64/mmu_constants.h>
#include <stdint.h>

namespace arch::x86_64 {

using pt_entry_t = volatile uint64_t;

void mmu_init_phys_map();

} // namespace arch::x86_64
