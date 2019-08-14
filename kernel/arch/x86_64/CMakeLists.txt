add_library(arch boot/boot.S boot/startup32.cpp include/arch/x86_64/gdt.h)
target_include_directories(arch PUBLIC include)