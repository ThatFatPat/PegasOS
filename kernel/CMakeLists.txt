set(ADDITIONAL_FLAGS -ffreestanding -fno-exceptions -fno-rtti -mcmodel=kernel -mno-red-zone)

add_compile_options(${ADDITIONAL_FLAGS})
add_link_options(${ADDITIONAL_FLAGS} -nostdlib -z max-page-size=0x1000)

include_directories(include)

add_subdirectory(arch/x86_64)

add_subdirectory(lib)

add_executable(kernel dummy.cpp)
target_link_libraries(kernel PRIVATE arch support gcc)

target_link_options(kernel PRIVATE -T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)
set_target_properties(kernel PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)
