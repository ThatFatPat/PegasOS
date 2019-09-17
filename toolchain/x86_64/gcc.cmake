include(${CMAKE_CURRENT_LIST_DIR}/common.cmake)

set(CMAKE_CXX_COMPILER ${GCC_TOOLCHAIN_BIN}/x86_64-elf-g++ CACHE PATH "CXX compiler")
set(CMAKE_ASM_COMPILER ${GCC_TOOLCHAIN_BIN}/x86_64-elf-gcc CACHE PATH "ASM compiler")

# Prevent gas from treating '/' as comment
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,--divide")
