target remote | qemu-system-i386.exe -gdb stdio -cdrom ./build/PegasOS.iso
# Next line for debugging in 16-bit
# set tdesc filename target.xml
set disassembly-flavor intel
