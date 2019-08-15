add_library(arch boot/boot.S boot/screen32.S)
target_include_directories(arch PUBLIC include)
