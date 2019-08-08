target remote | qemu-system-i386 -gdb stdio boot.bin
# Next line for debugging in 16-bit
# set tdesc filename target.xml
set disassembly-flavor intel