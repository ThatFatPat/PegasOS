#pragma once

#define X86_64_PAGE_SHIFT 12
#define X86_64_PAGE_SIZE (1 << X86_64_PAGE_SHIFT) // 4KiB

#define X86_64_PDT_PAGE_SHIFT 21
#define X86_64_PDT_PAGE_SIZE (1 << X86_64_PDT_PAGE_SHIFT) // 2MiB

#define X86_64_PML4_IDX_FROM_ADDR(addr) (((addr) >> 39) & 0x1ff)
#define X86_64_PDPT_IDX_FROM_ADDR(addr) (((addr) >> 30) & 0x1ff)
#define X86_64_PD_IDX_FROM_ADDR(addr) (((addr) >> 21) & 0x1ff)
#define X86_64_PT_IDX_FROM_ADDR(addr) (((addr) >> 12) & 0x1ff)

#define X86_64_PT_ALIGN 0x1000
#define X86_64_PT_ENTRIES 0x200
#define X86_64_PT_ENTRY_SIZE 8
#define X86_64_PT_SIZE (X86_64_PT_ENTRIES * X86_64_PT_ENTRY_SIZE)

#define X86_64_MMU_FLAG_PRESENT (1 << 0)
#define X86_64_MMU_FLAG_WRITE (1 << 1)
#define X86_64_MMU_FLAG_USER (1 << 2)
#define X86_64_MMU_FLAG_NX (1 << 63)
#define X86_64_MMU_FLAG_LARGE (1 << 7)

#ifndef __ASSEMBLER__

#include <stdint.h>

namespace arch::x86_64 {

using pt_entry_t = uint64_t;

} // namespace arch::x86_64

#endif
