org 0x7c00

xor bx, bx
mov ss, bx
mov bp, 0x8000
mov sp, bp

push dx ; dl = [bp]
mov ah, 0x0e ; AH - Video command
xor bx, bx ; BH = Page Number, BL = Color
lea bp, [str]
print_loop:
    mov al, [bp] ; The current character
    test al, al
    jz end_print_loop
    int 0x10
    inc bp
    jmp print_loop

end_print_loop:
    mov al, [bp - 1]
    add al, '0'
    int 0x10

; Reset the disk
xor ah, ah
int 0x13

; Read sector 2 of the disk we booted on into memory. Start writing to 0xa0000 and continue from there sequentially.
mov cx, 0x2 ; Cylinder 0, sector 2
mov dl, [bp - 1] ; Get DL = disk number
xor dh, dh ; Track 0
mov ah, 0x2
mov al, 0x1
xor bx, bx
mov es, bx
mov bx, 0x8001

; Print the number of sectors that have been read
int 0x13
xor bx, bx
mov ah, 0xe
add al, '0'
int 0x10

add sp, 2
duck:
    hlt
    jmp duck



str:
    db "Booted from drive ", 0

times 510 - ($ - $$) db 0x0
dw 0xaa55
times 1024 db 'A'