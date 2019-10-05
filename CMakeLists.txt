CMAKE_MINIMUM_REQUIRED(VERSION 3.13)

project(PegasOS CXX ASM)

set(PEGASOS_NO_KERNEL_LTO OFF CACHE BOOL "Disable kernel LTO in release builds")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

add_subdirectory(doxygen)

add_subdirectory(psl)
add_subdirectory(kernel)
add_subdirectory(iso)
