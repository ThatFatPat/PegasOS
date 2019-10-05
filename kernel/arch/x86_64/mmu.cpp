/**
 * @addtogroup x86_64
 * @{
 * @file mmu.cpp
 */

#include <arch/x86_64/mmu.h>

#include <arch/kernel_vspace.h>
#include <arch/x86_64/control_regs.h>

#include <psl/iterator.h>
#include <stddef.h>

alignas(X86_64_PT_ALIGN) arch::x86_64::pt_entry_t pml4[X86_64_PT_ENTRIES];
alignas(X86_64_PT_ALIGN) arch::x86_64::pt_entry_t high_pdpt[X86_64_PT_ENTRIES];
alignas(X86_64_PT_ALIGN) arch::x86_64::pt_entry_t
    phys_map_pd[64 * X86_64_PT_ENTRIES];

namespace arch::x86_64 {
namespace {

void flush_tlb() {
  write_cr3(read_cr3());
}

} // namespace

void mmu_init_phys_map() {
  constexpr uint64_t flags =
      X86_64_MMU_FLAG_PRESENT | X86_64_MMU_FLAG_WRITE | X86_64_MMU_FLAG_NX;

  high_pdpt[X86_64_PDPT_IDX_FROM_ADDR(ARCH_PHYS_MAP_BASE)] =
      (reinterpret_cast<uintptr_t>(phys_map_pd) - ARCH_KERNEL_VMA_OFFSET) |
      flags;

  for (size_t i = 0; i < psl::size(phys_map_pd); i++) {
    uintptr_t paddr = i * X86_64_PDT_PAGE_SIZE;
    phys_map_pd[i] = paddr | flags | X86_64_MMU_FLAG_LARGE;
  }

  pml4[0] = 0; // Remove temporary 1GiB identity map

  flush_tlb();
}

} // namespace arch::x86_64

/** @} */