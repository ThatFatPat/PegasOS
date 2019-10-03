add_executable(kernel main.cpp)

target_include_directories(kernel PRIVATE include)
target_link_libraries(kernel PRIVATE gcc psl)

set(ADDITIONAL_OPTIONS -ffreestanding -fno-exceptions -fno-rtti)

target_compile_options(kernel PRIVATE ${ADDITIONAL_OPTIONS})
target_link_options(
  kernel PRIVATE
  ${ADDITIONAL_OPTIONS}
  -nostdlib
  -z max-page-size=0x1000
)

if (CMAKE_BUILD_TYPE MATCHES Rel AND NOT PEGASOS_NO_KERNEL_LTO)
  set_target_properties(kernel PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_subdirectory(arch/x86_64)
add_subdirectory(lib)
add_subdirectory(mm)
