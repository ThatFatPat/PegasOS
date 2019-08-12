add_compile_options(-ffreestanding -nostdlib)
add_link_options(-ffreestanding -nostdlib)

add_subdirectory(arch/x86)

add_executable(kernel.bin dummy.cpp)

target_link_libraries(kernel.bin PRIVATE arch)

target_link_options(kernel.bin PRIVATE -T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.lds.S)
set_target_properties(kernel.bin PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kernel.lds.S)