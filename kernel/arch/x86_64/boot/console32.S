#include <asm.h>

.intel_syntax noprefix

.code32

.section .boottext, "ax"

.set VIDEO_MEMORY, 0xb8000
# Asumes the screen is 80x25
# TODO: parse correct values out of multiboot header
.set VIDEO_WIDTH, 80
.set VIDEO_HEIGHT, 25
.set STDCOLOR, 0x07

# Print null-terminated string.
# EBX - Address of string, EDX - Current offset in VIDEO_MEMORY.
FUNC(console32_print)
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
FUNC_END(console32_print)

FUNC(console32_clear)
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
FUNC_END(console32_clear)
