target remote | qemu-system-x86_64 -gdb stdio boot.bin
# Next line for debugging in 16-bit
# set tdesc filename target.xml
set disassembly-flavor intel