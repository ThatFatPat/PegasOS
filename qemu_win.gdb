file ./build/kernel/kernel
target remote | qemu-system-x86_64.exe -gdb stdio -cdrom ./build/iso/PegasOS.iso
set disassembly-flavor intel
