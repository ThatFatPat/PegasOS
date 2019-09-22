CMAKE_MINIMUM_REQUIRED(VERSION 3.13)

project(PegasOS CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

add_subdirectory(psl)
add_subdirectory(kernel)
add_subdirectory(iso)
