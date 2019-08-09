[bits 16]
org 0x7c00
MAIN_MEMORY64_PAGE equ 0x200

start_rm:

%include "rm.asm"
%include "gdt.asm"

times 510 - ($ - $$) db 0x0
dw 0xaa55

end_rm:

start_pm:

%include "pm.asm"

end_pm:

start_lm:

%include "lm.asm"

end_lm:

times 1024 db 'A', 'B'