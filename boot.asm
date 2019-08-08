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
    call print_hex
    call print_newline

reset_disk:
    xor ah, ah
    int 0x13

; read_sectors:
    mov cx, 0x2 ; Cylinder 0, sector 2
    xor dx, dx  ; Track 0
    mov si, sp
    mov dl, [si] ; Get DL = disk number
    mov ah, 0x2
    mov al, 0x1 ; Number of sectors to read
    xor bx, bx
    mov es, bx
    mov bx, 0x8001
    int 0x13

print_number_of_read_sectors:
    lea bx, [read_sectors_string]
    mov dx, 0x5
    call print_string
    xor bx, bx
    call print_number
    lea bx, [read_sectors_string + 0x5]
    mov dx, 0x8
    call print_string


cleanup:
    add sp, 2


fallback:
    hlt
    jmp fallback

%include "print.asm"

booted_from_drive_string:
    db "Booted from drive "
read_sectors_string:
    db "Read ", " sectors"

times 510 - ($ - $$) db 0x0
dw 0xaa55
times 1024 db 'A'