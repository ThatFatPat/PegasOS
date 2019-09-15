#include <arch/kernel_addrspace.h>
#include <arch/x86_64/control_regs.h>
#include <arch/x86_64/mmu.h>
#include <stddef.h>

alignas(
    X86_64_PT_ALIGN) volatile arch::x86_64::pt_entry_t pml4[X86_64_PT_ENTRIES];

alignas(X86_64_PT_ALIGN) volatile arch::x86_64::pt_entry_t
    high_pdpt[X86_64_PT_ENTRIES];

alignas(X86_64_PT_ALIGN) volatile arch::x86_64::pt_entry_t
    kernel_pdt[2 * X86_64_PT_ENTRIES];

alignas(X86_64_PT_ALIGN) volatile arch::x86_64::pt_entry_t
    phys_map_pdt[64 * X86_64_PT_ENTRIES];

namespace arch::x86_64 {
namespace {

void flush_tlb() {
  write_cr3(read_cr3());
}

} // namespace

void mmu_init_phys_map() {
  constexpr uint64_t flags = X86_64_MMU_FLAG_PRESENT | X86_64_MMU_FLAG_WRITE;

  high_pdpt[X86_64_PDPT_IDX_FROM_ADDR(ARCH_PHYS_MAP_BASE)] =
      (reinterpret_cast<uintptr_t>(phys_map_pdt) - ARCH_KERNEL_VMA_OFFSET) |
      flags;

  for (size_t i = 0; i < 64 * X86_64_PT_ENTRIES; i++) {
    uintptr_t paddr = i * X86_64_PDT_PAGE_SIZE;
    phys_map_pdt[i] = paddr | flags | X86_64_MMU_FLAG_LARGE;
  }

  pml4[0] = 0; // Remove temporary 1GiB identity map

  flush_tlb();
}

} // namespace arch::x86_64
