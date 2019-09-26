#include <arch/kernel_vspace.h>
#include <arch/x86_64/control_regs.h>
#include <arch/x86_64/cpuid.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/mmu.h>
#include <arch/x86_64/multiboot2.h>
#include <asm.h>

#define PHYS_ADDR(addr) ((addr) - ARCH_KERNEL_VMA_OFFSET)

.intel_syntax noprefix

.section .multiboot, "a"
.set MB_HEADER_LENGTH, mb_header_end - mb_header_start
.set MB_HEADER_CHECKSUM, -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + MB_HEADER_LENGTH)
mb_header_start:
.long MULTIBOOT2_HEADER_MAGIC
.long MULTIBOOT_ARCHITECTURE_I386
.long MB_HEADER_LENGTH
.long MB_HEADER_CHECKSUM
# multiboot2 final tag
.align 8
.word MULTIBOOT_TAG_TYPE_END # type
.word 0 # flags
.long 8 # size
mb_header_end:

.section .bootrodata, "a"

LOCAL_DATA(early_gdt)
  .quad 0 # null entry
  # native long mode code
  # note: base and limit here are ignored, only flags matter
  .quad X86_64_GDT_ENTRY(0, 0, X86_64_GDT_FLAG_64, X86_64_GDT_ACCESS_PRESENT |
                                                   X86_64_GDT_ACCESS_RING0 |
                                                   X86_64_GDT_ACCESS_NON_SYSTEM |
                                                   X86_64_GDT_ACCESS_EXEC)
DATA_END(early_gdt)
early_gdt_end:

.set EARLY_GDT_SIZE, early_gdt_end - early_gdt

LOCAL_DATA(early_gdt_desc)
  .word EARLY_GDT_SIZE - 1
  .long early_gdt
DATA_END(early_gdt_desc)

.set EARLY_CODE_SEG, X86_64_GDT_ENTRY_SIZE

LOCAL_DATA(multiboot_error_string)
  .asciz "Error: PegasOS requires a multiboot2-compliant bootloader"
DATA_END(multiboot_error_string)

LOCAL_DATA(long_mode_error_string)
  .asciz "Error: PegasOS requires a 64-bit processor"
DATA_END(long_mode_error_string)

.section .bootbss, "aw", @nobits

# Space for temporary low 1GiB mapping

.align X86_64_PT_ALIGN
LOCAL_DATA(early_low_pdpt)
  .skip X86_64_PT_SIZE
DATA_END(early_low_pdpt)

.bss

.set EARLY_STACK_SIZE, 0x4000 # 16KiB
.set EARLY_STACK_ALIGN, 0x10

.align EARLY_STACK_ALIGN
LOCAL_DATA(early_stack)
  .skip EARLY_STACK_SIZE
DATA_END(early_stack)

.align X86_64_PT_ALIGN
LOCAL_DATA(early_low_pd)
  .skip X86_64_PT_SIZE
DATA_END(early_low_pd)

.code32

.section .boottext, "ax"

FUNC(_start32)
  # Stash pointer to multiboot info
  mov [PHYS_ADDR(multiboot_info_phys_addr)], ebx
  
  lea esp, [PHYS_ADDR(early_stack) + EARLY_STACK_SIZE]
  call console32_clear
  cmp eax, MULTIBOOT2_BOOTLOADER_MAGIC
  jnz .Lno_mutliboot
  call check_long_mode
  test al, al
  jz .Lno_long_mode
  jmp  enter_long_mode
.Lno_mutliboot: # Error: not loaded via multiboot2 protocol
  lea ebx, [multiboot_error_string]
  xor edx, edx
  call console32_print
  jmp .Ldie
.Lno_long_mode: # Error: 32-bit processor
  lea ebx, [long_mode_error_string]
  xor edx, edx
  call console32_print
.Ldie:
  hlt
  jmp .Ldie
FUNC_END(_start32)

LOCAL_FUNC(check_long_mode)
  mov eax, X86_64_CPUID_FUNC_EXT_CPUID # Check for extended cpuid functions
  cpuid
  cmp eax, X86_64_CPUID_FUNC_EXT_CPUID
  jbe 1f
  mov eax, X86_64_CPUID_FUNC_EXT_PROC_INFO
  cpuid
  test edx, X86_64_CPU_FEATURE_LONG_MODE
  setnz al
  ret
1: # No long mode support
  xor eax, eax
  ret
FUNC_END(check_long_mode)

LOCAL_FUNC(enter_long_mode)
  # Set up early 1GiB identity mapping
  lea eax, [early_low_pdpt + X86_64_MMU_FLAG_PRESENT | X86_64_MMU_FLAG_WRITE]
  mov [PHYS_ADDR(pml4)], eax

  lea eax, [PHYS_ADDR(early_low_pd) + X86_64_MMU_FLAG_PRESENT | X86_64_MMU_FLAG_WRITE]
  mov [early_low_pdpt], eax

  mov eax, X86_64_MMU_FLAG_PRESENT | X86_64_MMU_FLAG_WRITE | X86_64_MMU_FLAG_LARGE
  xor ecx, ecx

.Lfill_low_pd:
  mov [PHYS_ADDR(early_low_pd) + X86_64_PT_ENTRY_SIZE * ecx], eax
  add eax, X86_64_PDT_PAGE_SIZE
  inc ecx
  cmp ecx, X86_64_PT_ENTRIES
  jne .Lfill_low_pd

  # Set up mapping of kernel at -2GiB: use existing low-1GiB table
  lea eax, [PHYS_ADDR(high_pdpt) + X86_64_MMU_FLAG_PRESENT | X86_64_MMU_FLAG_WRITE]
  mov [PHYS_ADDR(pml4) + X86_64_PML4_IDX_FROM_ADDR(ARCH_KERNEL_VSPACE_BASE) \
       * X86_64_PT_ENTRY_SIZE], eax

  lea eax, [PHYS_ADDR(early_low_pd) + X86_64_MMU_FLAG_PRESENT | X86_64_MMU_FLAG_WRITE]
  mov [PHYS_ADDR(high_pdpt) + X86_64_PDPT_IDX_FROM_ADDR(ARCH_KERNEL_VMA_OFFSET) \
       * X86_64_PT_ENTRY_SIZE], eax

  lea eax, [PHYS_ADDR(pml4)]
  mov cr3, eax
  mov eax, X86_64_CR4_PAE
  mov cr4, eax
  mov ecx, X86_64_MSR_EFER
  rdmsr
  or eax, X86_64_MSR_EFER_LME
  wrmsr
  mov eax, X86_64_CR0_PE | X86_64_CR0_PG
  mov cr0, eax

  lgdt early_gdt_desc
  jmp EARLY_CODE_SEG:_start64
FUNC_END(enter_long_mode)

.code64

LOCAL_FUNC(_start64)
  xor eax, eax
  mov ss, eax
  mov ds, eax
  mov es, eax
  mov fs, eax
  mov gs, eax
  lea rsp, [early_stack + EARLY_STACK_SIZE]
  call kernel_main
FUNC_END(_start64)
