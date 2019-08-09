; 32-bit Code Below!
[bits 32]
init_protected_mode:
    ; First of all we need to point all segment registers by the new GDT since what is contained in them is meaningless right now.
    mov ax, DATA_SEG
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov ds, ax

    ; Create new stack
    mov ebp, 0x90000
    mov esp, ebp
    jmp protected_mode_main

protected_mode_main:
    lea ebx, [Welcome32_string]
    mov ecx, 0x12
    mov edx, 0xb8000
    call print32_string
    jmp $

%include "print32.asm"

Welcome32_string: db "Welcome to 32-bit!"
