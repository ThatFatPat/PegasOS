#include <arch/x86_64/control_regs.h>
#include <arch/x86_64/cpuid.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/mmu.h>

#include <multiboot2.h>

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
early_gdt_start:
.quad 0 # null entry
# native long mode code
# note: base and limit here are ignored, only flags matter
.quad X86_64_GDT_ENTRY(0, 0, X86_64_GDT_FLAG_64, X86_64_GDT_ACCESS_PRESENT |
                                                 X86_64_GDT_ACCESS_RING0 |
                                                 X86_64_GDT_ACCESS_NON_SYSTEM |
                                                 X86_64_GDT_ACCESS_EXEC)
early_gdt_end:
.set EARLY_GDT_SIZE, early_gdt_end - early_gdt_start
early_gdt_desc:
.word EARLY_GDT_SIZE - 1
.long early_gdt_start

.set EARLY_CODE_SEG, X86_64_GDT_ENTRY_SIZE

.section .bootbss, "aw", @nobits

.set EARLY_STACK_SIZE, 0x4000 # 16KiB
early_stack_bottom:
.skip EARLY_STACK_SIZE
early_stack_top:

# Space for early 4GiB identity page tables
.align X86_64_PT_ALIGN
early_pml4:
.skip X86_64_PT_SIZE
early_pdpt:
.skip X86_64_PT_SIZE
early_page_directories:
.skip 4 * X86_64_PT_SIZE

.code32

.section .boottext, "ax"

.global _start32
.type _start32, @function
_start32:
  lea ebp, [early_stack_top]
  mov esp, ebp
  call clear_screen32
  mov esi, ebx # preserve pointer to multiboot info
  cmp eax, MULTIBOOT2_BOOTLOADER_MAGIC
  jnz .Lno_mutliboot
  call check_long_mode
  test al, al
  jz .Lno_long_mode
  jmp  enter_long_mode
.Lno_mutliboot: # Error: not loaded via multiboot2 protocol
  lea ebx, [multiboot_error_string]
  xor edx, edx
  call print_string32
  jmp die
.Lno_long_mode: # Error: 32-bit processor
  lea ebx, [no_long_mode_error_string]
  xor edx, edx
  call print_string32
  jmp die
.size _start32, . - _start32

.type check_long_mode, @function
check_long_mode:
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
.size check_long_mode, . - check_long_mode

.type enter_long_mode, @function
enter_long_mode:
  # Set up early 4GiB identity mapping
  lea eax, [early_pdpt + X86_64_MMU_FLAG_PRESENT | X86_64_MMU_FLAG_WRITE]
  mov [early_pml4], eax
  lea eax, [early_page_directories + X86_64_MMU_FLAG_PRESENT | X86_64_MMU_FLAG_WRITE]
  lea edi, [early_pdpt]
  mov ecx, 4

.Lfill_pdpt:
  mov [edi], eax
  add eax, X86_64_PT_SIZE
  add edi, X86_64_PT_ENTRY_SIZE
  loop .Lfill_pdpt

  mov eax, X86_64_MMU_FLAG_PRESENT | X86_64_MMU_FLAG_WRITE | X86_64_MMU_FLAG_LARGE
  xor ecx, ecx

.Lfill_page_directories:
  mov [early_page_directories + X86_64_PT_ENTRY_SIZE * ecx], eax
  add eax, X86_64_PDT_PAGE_SIZE
  inc ecx
  cmp ecx, 4 * (X86_64_PT_SIZE / 8)
  jne .Lfill_page_directories

  lea eax, [early_pml4]
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
.size enter_long_mode, . - enter_long_mode

.type die, @function
die: # RIP
  hlt
  jmp die
.size die, . - die

.code64

.type _start64, @function
_start64:
  xor ax, ax
  mov ss, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov dword ptr [0xb8000], 0x074c # Display 'L' in light gray
  1: hlt
  jmp 1b
.size _start64, . - _start64

.section .bootrodata, "a"

multiboot_error_string: .asciz "Error! Please use Multiboot2 to boot!"
no_long_mode_error_string: .asciz "Error: a 64-bit processor is required for PegasOS"
