target remote | qemu-system-i386 -gdb stdio boot.bin
set tdesc filename target.xml
set disassembly-flavor intel