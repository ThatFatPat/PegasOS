xor bx, bx
mov ss, bx
mov sp, 0x8000

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
    cli ; Disable Interrupts
    lgdt [gdt_descriptor] ; Load GDT
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax ; Enable Protected Mode
    jmp CODE_SEG:init_protected_mode ; Far jump to clear icache

fallback:
    hlt
    jmp fallback

%include "print.asm"

booted_from_drive_string:
    db "Booted from drive "
read_sectors_string:
    db "Read ", " sectors"