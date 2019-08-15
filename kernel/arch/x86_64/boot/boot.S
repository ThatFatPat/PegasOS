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
.word 0 # type
.word 0 # flags
.long 8 # size
mb_header_end:

.section .bootrodata, "a"
gdt_start:
.quad 0 # null entry
.quad GDT_ENTRY(0, 0xffffffff, GDT_FLAG_64, GDT_ACCESS_PRESENT |
                                            GDT_ACCESS_RING0 |
                                            GDT_ACCESS_NON_SYSTEM |
                                            GDT_ACCESS_EXEC) # early 64-bit code
gdt_end:
.set GDT_SIZE, gdt_end - gdt_start
gdt_desc:
.word GDT_SIZE - 1
.long gdt_start

.set CODE_SEG, GDT_ENTRY_SIZE

.section .bootbss, "aw", @nobits
stack_bottom:
.skip 0x4000
stack_top:

.code32

.section .boottext, "ax"
.global _start32
.type _start32, @function

_start32:
  lea ebp, [stack_top]
  mov esp, ebp
  call clear_screen
  cmp eax, MULTIBOOT2_BOOTLOADER_MAGIC
  jnz 1f
  lea ebx, [welcome32_string]
  xor edx, edx
  call print_string
  jmp  die

1:
  lea ebx, [multiboot_error_string]
  xor edx, edx
  call print_string
  jmp die

.size _start32, . - _start32

die:
  hlt
  jmp die

.set VIDEO_MEMORY, 0xb8000
# Asumes the screen is 80x25
# TODO: parse correct values out of multiboot header
.set VIDEO_WIDTH, 80
.set VIDEO_HEIGHT, 25
.set STDCOLOR, 0x0f

# Print null-terminated string.
# EBX - Address of string, EDX - Current offset in VIDEO_MEMORY.
print_string:
  push ebp
  mov ebp, esp
  push eax
  push ebx
  push edx
  add edx, VIDEO_MEMORY
  mov al, [ebx] # The current character
  test al, al
  jz 2f
  
1:
  mov ah, STDCOLOR
  mov [edx], ax
  add edx, 0x2
  inc ebx
  mov al, [ebx]
  test al, al
  jnz 1b

2:
  pop edx
  pop ebx
  pop eax
  leave
  ret

# Clear the screen.
clear_screen:
  push ebp
  mov ebp, esp
  push eax
  push ecx
  push edi
  mov ecx, VIDEO_WIDTH * VIDEO_HEIGHT
  xor ax, ax # Fill with null character
  cld
  mov edi, VIDEO_MEMORY
  rep stosw
  pop edi
  pop ecx
  pop eax
  leave
  ret

.section .bootrodata, "a"

welcome32_string: .asciz "Welcome to 32-bit!"
multiboot_error_string: .asciz "Error! Please use Multiboot2 to boot!"
