[bits 16]
org 0x7c00
KERNEL32_LOCATION equ 0x8001
MAIN_MEMORY64_PAGE equ 0x200

start_rm:

xor bx, bx
mov ss, bx
mov sp, 0x7bff ; Area between [0x500, 0x7c00) is unused by BIOS: https://0xax.gitbooks.io/linux-insides/content/Booting/linux-bootstrap-1.html

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

NUM_PM_SECTORS equ 0x1

read_pm_sectors:
    mov cx, 0x2 ; Cylinder 0, sector 2
    xor dx, dx  ; Track 0
    mov si, sp ; Cannot address using SP.
    mov dl, [si] ; Get DL = disk number
    mov ah, 0x2 ; Sub command INT13h AH=02h : Read Sectors from disk
    mov al, NUM_PM_SECTORS ; Number of sectors to read
    xor bx, bx
    mov es, bx ; Zero out ES
    mov bx, end_rm ; Starting address, read sectors for pm and lm right after rm code so it stays sequential.
    int 0x13

read_kernel_sectors:
    mov cx, 0x2+NUM_PM_SECTORS ; Cylinder 0, sector 2
    xor dx, dx  ; Track 0
    mov si, sp ; Cannot address using SP.
    mov dl, [si] ; Get DL = disk number
    mov ah, 0x2 ; Sub command INT13h AH=02h : Read Sectors from disk
    mov al, 0x1 ; Number of sectors to read
    xor bx, bx
    mov es, bx ; Zero out ES
    mov bx, KERNEL32_LOCATION ; Starting address, read sectors for pm and lm right after rm code so it stays sequential.
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
    cli ; Disable Interrupts
    lgdt [gdt_descriptor] ; Load GDT
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax ; Enable Protected Mode
    jmp CODE_SEG:end_rm ; Far jump to clear icache

fallback:
    hlt
    jmp fallback

; Prints string. BX - Address to string, DX - Number of chars. set to 0x0 for null-terminated string.
print_string:
    push bp
    mov bp, sp
    push si
    push cx
    push ax
    mov si, bx ; Move address to SI
    mov ah, 0x0e ; AH - Video command
    xor bx, bx ; BH = Page Number, BL = Color
    xor cx, cx ; In case it's needed for print_loop_with_size
    test dx, dx ; Decide on print loop
    jne .print_loop_with_size
    .print_loop:
        mov al, BYTE [si] ; The current character
        cmp al, 0
        jz .end_print_loop
        int 0x10
        inc si
        jmp .print_loop
    .end_print_loop:
        pop ax
        pop cx
        pop si
        leave
        ret
    .print_loop_with_size:
        mov al, BYTE [si] ; The current character
        cmp cx, dx
        jz .end_print_loop
        int 0x10
        inc si
        inc cx
        jmp .print_loop_with_size

print_newline:
    push bx
    push dx
    lea bx, [newline]
    mov dx, 0x0
    call print_string
    pop bx
    pop dx
    ret

; Print number in al
print_number:
    push ax
    mov ah, 0xe
    add al, '0'
    int 0x10
    pop ax
    ret

; Prints the value of DX as hex.
print_hex:
    push cx             ; save the register values to the stack for later
    push bx
    push ax
    mov cx,4          ; Start the counter: we want to print 4 characters
                        ; 4 bits per char, so we're printing a total of 16 bits

    .char_loop:
        dec cx            ; Decrement the counter

        mov ax,dx         ; copy bx into ax so we can mask it for the last chars
        shr dx,4          ; shift bx 4 bits to the right
        and ax,0xf        ; mask ah to get the last 4 bits

        mov bx, HEX_OUT   ; set bx to the memory address of our string
        add bx, 2         ; skip the '0x'
        add bx, cx        ; add the current counter to the address

        cmp ax,0xa        ; Check to see if it's a letter or number
        jl .set_letter     ; If it's a number, go straight to setting the value
        add byte [bx],7   ; If it's a letter, add 7
                            ; Why this magic number? ASCII letters start 17
                            ; characters after decimal numbers. We need to cover that
                            ; distance. If our value is a 'letter' it's already
                            ; over 10, so we need to add 7 more.
        jl .set_letter

    .set_letter:
        add byte [bx],al  ; Add the value of the byte to the char at bx

        cmp cx,0          ; check the counter, compare with 0
        je .print_hex_done ; if the counter is 0, finish
        jmp .char_loop     ; otherwise, loop again

    .print_hex_done:
        mov bx, HEX_OUT   ; print the string pointed to by bx
        call print_string

        pop ax              ; pop the initial register values back from the stack
        pop bx
        pop cx
        ret               ; return the function

; global variables
HEX_OUT: db '0x0000',0
newline: db 0xa,0xd, 0

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

end_rm:

start_pm:
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
    call KERNEL32_LOCATION

jmp $

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


Welcome32_string: db "Welcome to 32-bit!"

end_pm:
times 1024 - ($ - $$) db 0x0
