add_library(arch boot/boot.S boot/startup32.cpp)
target_include_directories(arch PUBLIC include)
