#include <arch/x86_64/cpuid.h>
#include <arch/x86_64/gdt.h>

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
gdt_start:
.quad 0 # null entry
# native long mode code
# note: base and limit here are ignored, only flags matter
.quad GDT_ENTRY(0, 0, GDT_FLAG_64, GDT_ACCESS_PRESENT |
                                   GDT_ACCESS_RING0 |
                                   GDT_ACCESS_NON_SYSTEM |
                                   GDT_ACCESS_EXEC)
gdt_end:
.set GDT_SIZE, gdt_end - gdt_start
gdt_desc:
.word GDT_SIZE - 1
.long gdt_start

.set CODE_SEG, GDT_ENTRY_SIZE

.section .bootbss, "aw", @nobits

.set EARLY_STACK_SIZE, 0x4000 # 16KiB
stack_bottom:
.skip EARLY_STACK_SIZE
stack_top:

.code32

.section .boottext, "ax"

.global _start32
.type _start32, @function
_start32:
  lea ebp, [stack_top]
  mov esp, ebp
  call clear_screen32
  cmp eax, MULTIBOOT2_BOOTLOADER_MAGIC
  jnz .Lno_mutliboot
  push ebx # Preserve pointer to structure with multiboot information.
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

die: # RIP
  hlt
  jmp die

.section .bootrodata, "a"

multiboot_error_string: .asciz "Error! Please use Multiboot2 to boot!"
no_long_mode_error_string: .asciz "Error: a 64-bit processor is required for PegasOS"
