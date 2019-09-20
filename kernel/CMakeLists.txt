add_executable(kernel dummy.cpp)

target_include_directories(kernel PRIVATE include)
target_link_libraries(kernel PRIVATE gcc)

set(
  ADDITIONAL_OPTIONS
  -ffreestanding -fno-exceptions -fno-rtti
)

target_compile_options(kernel PRIVATE ${ADDITIONAL_OPTIONS})
target_link_options(
  kernel PRIVATE
  ${ADDITIONAL_OPTIONS}
  -nostdlib -z
  max-page-size=0x1000
)

add_subdirectory(arch/x86_64)
add_subdirectory(lib)
