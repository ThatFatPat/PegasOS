org 0x7c00

mov ah, 0x0e ; AH - Video command
xor bx, bx ; BH = Page Number, BL = Color
lea bp, [str]
print_loop:
    mov al, [bp] ; The current character
    test al, al
    je duck
    int 0x10
    inc bp
    jmp print_loop


duck:
    hlt
    jmp duck

str:
    db "Hi!", 0

times 510 - ($ - $$) db 0x0
dw 0xaa55
