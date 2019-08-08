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
    jne print_loop_with_size
    print_loop:
        mov al, BYTE [si] ; The current character
        cmp al, 0
        jz end_print_loop
        int 0x10
        inc si
        jmp print_loop
    end_print_loop:
        pop ax
        pop cx
        pop si
        leave
        ret
    print_loop_with_size:
        mov al, BYTE [si] ; The current character
        cmp cx, dx
        jz end_print_loop
        int 0x10
        inc si
        inc cx
        jmp print_loop_with_size

print_newline:
    push bx
    push dx
    push ax
    lea bx, [newline]
    mov dx, 0x0
    call print_string
    mov ah, 0x3 ; Get current cursor position
    int 0x10 
    mov ah, 0x2
    mov dl, 0x0 ; Set column to 0. (Line is automatic after \n)
    int 0x10
    pop ax
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

    char_loop:
        dec cx            ; Decrement the counter

        mov ax,dx         ; copy bx into ax so we can mask it for the last chars
        shr dx,4          ; shift bx 4 bits to the right
        and ax,0xf        ; mask ah to get the last 4 bits

        mov bx, HEX_OUT   ; set bx to the memory address of our string
        add bx, 2         ; skip the '0x'
        add bx, cx        ; add the current counter to the address

        cmp ax,0xa        ; Check to see if it's a letter or number
        jl set_letter     ; If it's a number, go straight to setting the value
        add byte [bx],7   ; If it's a letter, add 7
                            ; Why this magic number? ASCII letters start 17
                            ; characters after decimal numbers. We need to cover that
                            ; distance. If our value is a 'letter' it's already
                            ; over 10, so we need to add 7 more.
        jl set_letter

    set_letter:
        add byte [bx],al  ; Add the value of the byte to the char at bx

        cmp cx,0          ; check the counter, compare with 0
        je print_hex_done ; if the counter is 0, finish
        jmp char_loop     ; otherwise, loop again

    print_hex_done:
        mov bx, HEX_OUT   ; print the string pointed to by bx
        call print_string

        pop ax              ; pop the initial register values back from the stack
        pop bx
        pop cx
        ret               ; return the function

; global variables
HEX_OUT: db '0x0000',0
newline: db 0xa, 0