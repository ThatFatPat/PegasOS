[bits 16]
org 0x7c00

%include "rm.asm"
%include "gdt.asm"
%include "pm.asm"

times 510 - ($ - $$) db 0x0
dw 0xaa55
times 1024 db 'A'