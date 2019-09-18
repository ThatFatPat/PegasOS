#pragma once

#define X86_64_PTE_MASK 0x1ff
#define X86_64_PML4_SHIFT 39
#define X86_64_PDPT_SHIFT 30
#define X86_64_PDT_SHIFT 21
#define X86_64_PT_SHIFT 12

#define X86_64_PML4_IDX_FROM_ADDR(addr) \
  (((addr) >> X86_64_PML4_SHIFT) & X86_64_PTE_MASK)
#define X86_64_PDPT_IDX_FROM_ADDR(addr) \
  (((addr) >> X86_64_PDPT_SHIFT) & X86_64_PTE_MASK)
#define X86_64_PDT_IDX_FROM_ADDR(addr) \
  (((addr) >> X86_64_PDT_SHIFT) & X86_64_PTE_MASK)
#define X86_64_PT_IDX_FROM_ADDR(addr) \
  (((addr) >> X86_64_PT_SHIFT) & X86_64_PTE_MASK)

#define X86_64_PAGE_SIZE (1 << X86_64_PT_SHIFT)      // 4KiB
#define X86_64_PDT_PAGE_SIZE (1 << X86_64_PDT_SHIFT) // 2MiB

#define X86_64_PT_ALIGN 0x1000
#define X86_64_PT_ENTRIES 0x200
#define X86_64_PT_ENTRY_SIZE 8
#define X86_64_PT_SIZE (X86_64_PT_ENTRIES * X86_64_PT_ENTRY_SIZE)

#define X86_64_MMU_FLAG_PRESENT (1 << 0)
#define X86_64_MMU_FLAG_WRITE (1 << 1)
#define X86_64_MMU_FLAG_USER (1 << 2)
#define X86_64_MMU_FLAG_NX (1ull << 63)
#define X86_64_MMU_FLAG_LARGE (1 << 7)
