#include <arch/x86_64/mmu.h>

alignas(
    X86_64_PT_ALIGN) volatile arch::x86_64::pt_entry_t pml4[X86_64_PT_ENTRIES];

alignas(X86_64_PT_ALIGN) volatile arch::x86_64::pt_entry_t
    high_pdpt[X86_64_PT_ENTRIES];

alignas(X86_64_PT_ALIGN) volatile arch::x86_64::pt_entry_t
    kernel_pdt[2 * X86_64_PT_ENTRIES];

alignas(X86_64_PT_ALIGN) volatile arch::x86_64::pt_entry_t
    phys_map_pdt[64 * X86_64_PT_ENTRIES];
