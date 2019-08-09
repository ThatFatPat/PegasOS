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

disable_paging:
    mov eax, cr0                                   ; Set the A-register to control register 0.
    and eax, 01111111111111111111111111111111b     ; Clear the PG-bit, which is bit 31.
    mov cr0, eax                                   ; Set control register 0 to the A-register.

load_gdt64:
    lgdt [gdt64_descriptor] 

enable_pae:
    mov   eax, cr4
    or    eax, DWORD [pae_bit]
    mov   eax, cr4

load_page_table:
    xor eax, eax
    lea ebx, [pml4]
    shl ebx, 0x11
    mov eax, ebx
    mov cr3, eax
    lea eax, [pdpt] 
    shl eax, 0x4
    mov [pml4 + 0x1], eax
    xor eax, eax
    mov eax, MAIN_MEMORY64_PAGE  
    shl eax, 0x6
    mov [pdpt + 0x3], eax

RM_LEN equ end_lm - start_lm

copy_64_kernel_to_MAIN_MEMORY64_PAGE:
    mov esi, start_lm
    mov edi, MAIN_MEMORY64_PAGE
    mov edx, RM_LEN
    call memcpy

EFER_LME equ 8
MSR_EFER equ 0xc0000080
switch_mode_32_to_64:
    ; Enable Paging and Protection
    mov ebx, 0x80000001
    mov eax, cr0
    mov eax, ebx
    mov cr0, eax

    ; Enable stuff in MSR
    xor edx, edx ; Segmented Addressing by rdmsr (edx:eax)
    lea eax, [esp - 0x4]
    mov ecx, MSR_EFER
    rdmsr
    bts eax, EFER_LME
    wrmsr
    ; Transition to 64-bit code
    mov eax, MAIN_MEMORY64_PAGE
    push eax
    ret


jmp $

; Copy from starting address ESI to starting address EDI. Copy ECX bytes.
memcpy:
    push esi
    push edi
    push ecx
    push ebx
    test ecx, ecx
    jz .end_memcpy_loop
 .memcpy_loop:
    mov ebx, [esi]
    mov [edi], ebx
    inc esi
    inc edi
    loop .memcpy_loop
.end_memcpy_loop:
    pop ebx
    pop ecx
    pop edi
    pop esi
    ret

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
    dq 1000000000000000000000000000000000000000000000000000000000000011b ; First entry
    times 4032 db 0
pdpt:
    dq 1000000000000000000000000000000000000000000000000000000010000011b ; First entry
    times 4032 db 0 ; 4096 - 64
