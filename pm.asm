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

load_gdt64:
    lgdt [gdt64_descriptor] 

enable_pae:
    mov   eax, cr4
    or    eax, DWORD [pae_bit]
    mov   eax, cr4

load_page_table:
    xor eax, eax
    lea eax, [pdpt] 
    shl eax, 0x4
    mov [pml4+0x1], eax

jmp $

%include "print32.asm"

Welcome32_string: db "Welcome to 32-bit!"

; --- THIS PART OF THE CODE IS COPIED FROM OS-DEV.pdf, I READ THROUGH THE EXPLANATION AND IT'S POINTLESS TO CRAFT THIS MYSELF AS OF NOW. ---
; GDT
gdt64_start :
gdt64_null : ; the mandatory null descriptor
    dd 0x0 ; ’dd ’ means define double word ( i.e. 4 bytes )
    dd 0x0
gdt64_code32 : ; the code segment descriptor
    ; base = 0x0 , limit = 0xfffff ,
    ; 1st flags : ( present )1 ( privilege )00 ( descriptor type )1 -> 1001 b
    ; type flags : ( code )1 ( conforming )0 ( readable )1 ( accessed )0 -> 1010 b
    ; 2nd flags : ( granularity )1 (32 - bit default )1 (64 - bit seg )0 ( AVL )0 -> 1100 b
    dw 0xffff ; Limit ( bits 0 -15)
    dw 0x0 ; Base ( bits 0 -15)
    db 0x0 ; Base ( bits 16 -23)
    db 10011010b ; 1st flags , type flags
    db 11001111b ; 2nd flags , Limit ( bits 16 -19)
    db 0x0 ; Base ( bits 24 -31)
gdt64_code64 : ; the code segment descriptor
    ; base = 0x0 , limit = 0xfffff ,
    ; 1st flags : ( present )1 ( privilege )00 ( descriptor type )1 -> 1001 b
    ; type flags : ( code )1 ( conforming )0 ( readable )1 ( accessed )0 -> 1010 b
    ; 2nd flags : ( granularity )1 (32 - bit default )1 (64 - bit seg )0 ( AVL )0 -> 1100 b
    dw 0xffff ; Limit ( bits 0 -15)
    dw 0x0 ; Base ( bits 0 -15)
    db 0x0 ; Base ( bits 16 -23)
    db 10011010b ; 1st flags , type flags
    db 10101111b ; 2nd flags , Limit ( bits 16 -19)
    db 0x0 ; Base ( bits 24 -31)
gdt64_data : ; the data segment descriptor
    ; Same as code segment except for the type flags :
    ; type flags : ( code )0 ( expand down )0 ( writable )1 ( accessed )0 -> 0010 b
    dw 0xffff ; Limit ( bits 0 -15)
    dw 0x0 ; Base ( bits 0 -15)
    db 0x0 ; Base ( bits 16 -23)
    db 10010010b ; 1st flags , type flags
    db 11001111b ; 2nd flags , Limit ( bits 16 -19)
    db 0x0 ; Base ( bits 24 -31)
gdt64_end : ; The reason for putting a label at the end of the
; GDT is so we can have the assembler calculate
; the size of the GDT for the GDT decriptor ( below )
; GDT descriptior
gdt64_descriptor :
    dw gdt64_end - gdt64_start - 1 ; Size of our GDT , always less one
    ; of the true size
    dd gdt64_start ; Start address of our GDT
    ; Define some handy constants for the GDT segment descriptor offsets , which
    ; are what segment registers must contain when in protected mode. For example ,
    ; when we set DS = 0 x10 in PM , the CPU knows that we mean it to use the
    ; segment described at offset 0 x10 ( i.e. 16 bytes ) in our GDT , which in our
    ; case is the DATA segment (0 x0 -> NULL ; 0x08 -> CODE ; 0 x10 -> DATA )
CODE32_SEG equ gdt64_code32 - gdt64_start
CODE64_SEG equ gdt64_code64 - gdt64_start
DATA64_SEG equ gdt64_data - gdt64_start

; --- THIS PART OF THE CODE IS COPIED FROM OS-DEV.pdf, I READ THROUGH THE EXPLANATION AND IT'S POINTLESS TO CRAFT THIS MYSELF AS OF NOW. ---

pae_bit:
    db 00100000b
    times 3 db 0

pml4:
    dq 1000000000000000000000000000000000000000000000000000000000000011b
    times 4032 db 0
pdpt:
    ; Manually insert first entry (1GB page);
    times 4032 db 0 ; 4096 - 64