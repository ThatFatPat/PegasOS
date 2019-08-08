; 32-bit Routines Below!
VIDEO_MEMORY equ 0xb8000
STDCOLOR equ 0x0f
; EBX - Address of string, ECX - Size of string, EDX - Current location in VIDEO_MEMORY.(If 0x0 assumes start of VIDEO_MEMORY)
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
        mov al, BYTE [ebx] ; The current character
        mov ah, STDCOLOR
        test ecx, ecx
        jz .end_print32_string
        mov WORD [edx], ax
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
