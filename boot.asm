[bits 16]
org 0x7c00

xor bx, bx
mov ss, bx
mov sp, 0x8000 ; Moving directly to bp results 0xffff8000

push dx ; dl = [bp - 1]

print_drive:
    lea bx, [booted_from_drive_string]
    mov dx, 0x12
    call print_string
    mov si, sp
    mov dx, [si]
    call print_hex ; Print Drive Number, originates in DL and obtained through stack
    call print_newline

reset_disk:
    xor ah, ah ; INT13h, AH=00h : Reset Disk Services
    int 0x13

read_sectors:
    mov cx, 0x2 ; Cylinder 0, sector 2
    xor dx, dx  ; Track 0
    mov si, sp ; Cannot address using SP.
    mov dl, [si] ; Get DL = disk number
    mov ah, 0x2 ; Sub command INT13h AH=02h : Read Sectors from disk
    mov al, 0x1 ; Number of sectors to read
    xor bx, bx
    mov es, bx ; Zero out ES
    mov bx, 0x8001 ; Starting address
    int 0x13

print_number_of_read_sectors:
    lea bx, [read_sectors_string]
    mov dx, 0x5
    call print_string
    xor bx, bx
    call print_number ; Print number of sectors read, saved in AL.
    lea bx, [read_sectors_string + 0x5]
    mov dx, 0x8
    call print_string ; Print rest of read_sectors_string.


cleanup:
    add sp, 2

switch_mode_16_to_32:
    cli ; Disable interrupts
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:0x8001

fallback:
    hlt
    jmp fallback

%include "print.asm"

booted_from_drive_string:
    db "Booted from drive "
read_sectors_string:
    db "Read ", " sectors"

; --- THIS PART OF THE CODE IS COPIED FROM OS-DEV.pdf, I READ THROUGH THE EXPLANATION AND IT'S POINTLESS TO CRAFT THIS MYSELF AS OF NOW. ---
; GDT
gdt_start :
gdt_null : ; the mandatory null descriptor
    dd 0x0 ; ’dd ’ means define double word ( i.e. 4 bytes )
    dd 0x0
gdt_code : ; the code segment descriptor
    ; base =0x0 , limit =0 xfffff ,
    ; 1st flags : ( present )1 ( privilege )00 ( descriptor type )1 -> 1001 b
    ; type flags : ( code )1 ( conforming )0 ( readable )1 ( accessed )0 -> 1010 b
    ; 2nd flags : ( granularity )1 (32 - bit default )1 (64 - bit seg )0 ( AVL )0 -> 1100 b
    dw 0xffff ; Limit ( bits 0 -15)
    dw 0x0 ; Base ( bits 0 -15)
    db 0x0 ; Base ( bits 16 -23)
    db 10011010b ; 1st flags , type flags
    db 11001111b ; 2nd flags , Limit ( bits 16 -19)
    db 0x0 ; Base ( bits 24 -31)
gdt_data : ; the data segment descriptor
    ; Same as code segment except for the type flags :
    ; type flags : ( code )0 ( expand down )0 ( writable )1 ( accessed )0 -> 0010 b
    dw 0xffff ; Limit ( bits 0 -15)
    dw 0x0 ; Base ( bits 0 -15)
    db 0x0 ; Base ( bits 16 -23)
    db 10010010b ; 1st flags , type flags
    db 11001111b ; 2nd flags , Limit ( bits 16 -19)
    db 0x0 ; Base ( bits 24 -31)
gdt_end : ; The reason for putting a label at the end of the
; GDT is so we can have the assembler calculate
; the size of the GDT for the GDT decriptor ( below )
; GDT descriptior
gdt_descriptor :
    dw gdt_end - gdt_start - 1 ; Size of our GDT , always less one
    ; of the true size
    dd gdt_start ; Start address of our GDT
    ; Define some handy constants for the GDT segment descriptor offsets , which
    ; are what segment registers must contain when in protected mode. For example ,
    ; when we set DS = 0 x10 in PM , the CPU knows that we mean it to use the
    ; segment described at offset 0 x10 ( i.e. 16 bytes ) in our GDT , which in our
    ; case is the DATA segment (0 x0 -> NULL ; 0x08 -> CODE ; 0 x10 -> DATA )
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; --- THIS PART OF THE CODE IS COPIED FROM OS-DEV.pdf, I READ THROUGH THE EXPLANATION AND IT'S POINTLESS TO CRAFT THIS MYSELF AS OF NOW. ---

times 510 - ($ - $$) db 0x0
dw 0xaa55
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
    jmp $

times 1024 db 'A'