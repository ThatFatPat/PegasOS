[bits 16]
org 0x7c00

start_rm:

%include "rm.asm"
%include "gdt.asm"

times 510 - ($ - $$) db 0x0
dw 0xaa55

end_rm:

%include "pm.asm"

times 1024 db 'A', 'B'