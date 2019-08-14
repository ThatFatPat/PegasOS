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

.code32

.section .boottext, "ax"
.global _start32
.type _start32, @function
_start32:
    cmp eax, MULTIBOOT2_BOOTLOADER_MAGIC
    jnz multiboot_error
    lea ebx, [Welcome32_string]
    mov ecx, 0x12
    mov edx, 0xb8000
    call print32_string
    jmp  fallback_32

multiboot_error:
    lea ebx, [multiboot_error_string]
    mov ecx, 0x25
    mov edx, 0xb8000
    call print32_string
    jmp  fallback_32
    
fallback_32:
    jmp fallback_32

# 32-bit Printing Routines Below!
.set VIDEO_MEMORY, 0xb8000
.set STDCOLOR, 0x0f
# EBX - Address of string, ECX - Size of string, EDX - Current location in VIDEO_MEMORY.(If 0x0 assumes start of VIDEO_MEMORY)
print32_string:
    push ebp
    mov ebp, esp
    push eax
    push ebx
    push ecx
    push edx
    test edx, edx
    jne .print32_string_loop
    mov edx, VIDEO_MEMORY

.print32_string_loop:
        mov al, BYTE PTR [ebx] # The current character
        mov ah, STDCOLOR
        test ecx, ecx
        jz .end_print32_string
        mov WORD PTR [edx], ax
        add edx, 0x2
        inc ebx
        loop .print32_string_loop
        
.end_print32_string:
    pop edx
    pop ecx
    pop ebx
    pop eax
    leave
    ret

.section .bootrodata, "a"

Welcome32_string: .ascii "Welcome to 32-bit!"
multiboot_error_string: .ascii "Error! Please use Multiboot2 to boot!"
