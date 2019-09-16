#include <arch/x86_64/mp.h>

#include <arch/x86_64/control_regs.h>
#include <arch/x86_64/mmu.h>

namespace arch::x86_64 {
namespace {

void init_cpu_common() {
  // Enable NX bit and syscall instruction
  uint64_t ia32_efer = read_msr(X86_64_MSR_EFER);
  ia32_efer |= X86_64_MSR_EFER_NXE | X86_64_MSR_EFER_SCE;
  write_msr(X86_64_MSR_EFER, ia32_efer);

  write_cr0(read_cr0() | X86_64_CR0_WP); // Enable kernel write protection

  // Enable SSE
  write_cr4(read_cr4() | X86_64_CR4_OSFXSR | X86_64_CR4_OXMMEXCPT);
}

} // namespace

void init_bsp() {
  mmu_init_phys_map();
  init_cpu_common();
}

} // namespace arch::x86_64
