set(
  ADDITIONAL_FLAGS
  -ffreestanding -fno-exceptions -fno-rtti
  -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2
)

add_compile_options(${ADDITIONAL_FLAGS})
add_link_options(${ADDITIONAL_FLAGS} -nostdlib -z max-page-size=0x1000)

add_executable(kernel dummy.cpp)

target_include_directories(kernel PRIVATE include)
target_link_libraries(kernel PRIVATE gcc)

add_subdirectory(arch/x86_64)
add_subdirectory(lib)


target_link_options(kernel PRIVATE -T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)
set_target_properties(kernel PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)
