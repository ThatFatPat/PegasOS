#include <arch/x86_64/control_regs.h>
#include <arch/x86_64/cpuid.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/paging.h>

#define ASM_FILE
#include <multiboot2.h>
#undef ASM_FILE

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
.quad GDT_ENTRY(0, 0, GDT_FLAG_64, GDT_ACCESS_PRESENT |
                                   GDT_ACCESS_RING0 |
                                   GDT_ACCESS_NON_SYSTEM |
                                   GDT_ACCESS_EXEC)
early_gdt_end:
.set EARLY_GDT_SIZE, early_gdt_end - early_gdt_start
early_gdt_desc:
.word EARLY_GDT_SIZE - 1
.long early_gdt_start

.set EARLY_CODE_SEG, GDT_ENTRY_SIZE

.section .bootbss, "aw", @nobits

.set EARLY_STACK_SIZE, 0x4000 # 16KiB
early_stack_bottom:
.skip EARLY_STACK_SIZE
early_stack_top:

# Space for early 4GiB identity page tables
.align PML4_ALIGN
early_pml4:
.skip PML4_SIZE
early_pdpt:
.skip PDPT_SIZE
early_page_directories:
.skip 4 * PAGE_DIRECTORY_SIZE

.code32

.section .boottext, "ax"

.global _start32
.type _start32, @function
_start32:
  lea ebp, [early_stack_top]
  mov esp, ebp
  call clear_screen32
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

check_long_mode:
  mov eax, CPUID_FUNC_EXT_CPUID # Check for extended cpuid functions
  cpuid
  cmp eax, CPUID_FUNC_EXT_CPUID
  jbe 1f
  mov eax, CPUID_FUNC_EXT_PROC_INFO
  cpuid
  test edx, CPU_FEATURE_LONG_MODE
  setnz al
  ret
1: # No long mode support
  xor eax, eax
  ret

enter_long_mode:
  # Set up early 4GiB identity mapping
  lea eax, [early_pdpt + PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE]
  mov [early_pml4], eax
  lea eax, [early_page_directories + PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE]
  lea edi, [early_pdpt]
  mov ecx, 4

.Lfill_pdpt:
  mov [edi], eax
  add eax, PAGE_DIRECTORY_SIZE
  add edi, PDPT_ENTRY_SIZE
  loop .Lfill_pdpt

  mov eax, PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE | PDT_FLAG_2MB
  lea edi, [early_page_directories]
  xor ecx, ecx

.Lfill_page_directories:
  mov [early_page_directories + PAGE_DIRECTORY_ENTRY_SIZE * ecx], eax
  add eax, PDT_PAGE_SIZE
  inc ecx
  cmp ecx, 4 * (PAGE_DIRECTORY_SIZE / 8)
  jne .Lfill_page_directories

  lea eax, [early_pml4]
  mov cr3, eax
  mov eax, CR4_PAE
  mov cr4, eax
  mov ecx, MSR_EFER
  rdmsr
  or eax, MSR_EFER_LME
  wrmsr
  mov eax, CR0_PE | CR0_PG
  mov cr0, eax

  lgdt early_gdt_desc
  jmp EARLY_CODE_SEG:_start64

die: # RIP
  hlt
  jmp die

.code64

_start64:
1: hlt
jmp 1b

.section .bootrodata, "a"

multiboot_error_string: .asciz "Error! Please use Multiboot2 to boot!"
no_long_mode_error_string: .asciz "Error: a 64-bit processor is required for PegasOS"
