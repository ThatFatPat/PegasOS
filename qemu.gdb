file ./build/kernel/kernel
target remote | qemu-system-x86_64 -gdb stdio -cdrom ./build/iso/PegasOS.iso
set disassembly-flavor intel
