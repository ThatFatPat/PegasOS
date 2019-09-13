add_library(arch boot/early_entry.cpp boot/boot.S boot/screen32.S mmu.cpp)
target_compile_options(arch PUBLIC -Wa,--divide) # Prevent assembler from treating '/' as a comment
target_include_directories(arch PUBLIC include)
