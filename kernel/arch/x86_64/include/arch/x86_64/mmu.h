/**
 * @addtogroup x86_64
 * @{
 * @file mmu.h Processor MMU and paging helpers.
 */

#pragma once

/**
 * Mask defining how many bits of an address are used to index into a paging
 * structure.
 */
#define X86_64_PTE_MASK 0x1ff

/**
 * Shift amount defining which part of an address indexes into PML4 tables.
 */
#define X86_64_PML4_SHIFT 39

/**
 * Shift amount defining which part of an address indexes into page directory
 * pointer tables.
 */
#define X86_64_PDPT_SHIFT 30

/**
 * Shift amount defining which part of an address indexes into page directories.
 */
#define X86_64_PDT_SHIFT 21

/**
 * Shift amount defining which part of an address indexes into page tables.
 */
#define X86_64_PT_SHIFT 12


/**
 * Compute the PML4 index for the specified address.
 */
#define X86_64_PML4_IDX_FROM_ADDR(addr) \
  (((addr) >> X86_64_PML4_SHIFT) & X86_64_PTE_MASK)

/**
 * Compute the page directory pointer table index for the specified address.
 */
#define X86_64_PDPT_IDX_FROM_ADDR(addr) \
  (((addr) >> X86_64_PDPT_SHIFT) & X86_64_PTE_MASK)

/**
 * Compute the page directory index for the specified address.
 */
#define X86_64_PDT_IDX_FROM_ADDR(addr) \
  (((addr) >> X86_64_PDT_SHIFT) & X86_64_PTE_MASK)

/**
 * Compute the page table index for the specified address.
 */
#define X86_64_PT_IDX_FROM_ADDR(addr) \
  (((addr) >> X86_64_PT_SHIFT) & X86_64_PTE_MASK)


/**
 * The size of a page referenced by a page table.
 */
#define X86_64_PAGE_SIZE (1 << X86_64_PT_SHIFT) // 4KiB

/**
 * The size of a page referenced by a page directory.
 */
#define X86_64_PDT_PAGE_SIZE (1 << X86_64_PDT_SHIFT) // 2MiB


/**
 * The alignment of all paging structures.
 */
#define X86_64_PT_ALIGN 0x1000

/**
 * The number of entries in all paging structures.
 */
#define X86_64_PT_ENTRIES 0x200

/**
 * The entry size of all paging structures.
 */
#define X86_64_PT_ENTRY_SIZE 8

/**
 * The size (in bytes) of all paging structures.
 */
#define X86_64_PT_SIZE (X86_64_PT_ENTRIES * X86_64_PT_ENTRY_SIZE)


/**
 * Indicate that the page(s) referenced by this entry are present in memory. If
 * this flag is not set, accessing memory referenced by this entry will result
 * in a page fault.
 */
#define X86_64_MMU_FLAG_PRESENT (1 << 0)

/**
 * Indicate that the page(s) referenced by this entry are writeable.
 */
#define X86_64_MMU_FLAG_WRITE (1 << 1)

/**
 * Indicate that the page(s) referenced by this entry are accessible from user
 * mode.
 */
#define X86_64_MMU_FLAG_USER (1 << 2)

/**
 * Indicate that the page(s) referenced by this entry may not be executed.
 * @note This flag may not be used unless @ref X86_64_MSR_EFER_NXE is set in
 * @ref X86_64_MSR_EFER.
 */
#define X86_64_MMU_FLAG_NX (1ull << 63)

/**
 * Indicate that that this entry defines a large page. In a page directory
 * pointer table entry, this allows the entry to control a 1GiB huge page. In a
 * page directory entry, this allows the entry to control a 2MiB large page.
 */
#define X86_64_MMU_FLAG_LARGE (1 << 7)

#ifndef __ASSEMBLER__

#include <stdint.h>

namespace arch::x86_64 {

/**
 * Represents a paging structure entry.
 */
using pt_entry_t = volatile uint64_t;

/**
 * Initialize the map of physical memory at virtual address @ref
 * ARCH_PHYS_MAP_BASE.
 */
void mmu_init_phys_map();

} // namespace arch::x86_64

#endif

/** @} */
