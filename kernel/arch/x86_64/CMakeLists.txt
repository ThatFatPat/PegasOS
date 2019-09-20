target_include_directories(kernel PRIVATE include)

target_sources(
  kernel PRIVATE

  boot/early_entry.cpp
  boot/boot.S
  boot/console32.S
  mmu.cpp mp.cpp
)

set(
  ARCH_ADDITIONAL_OPTIONS 
  -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2
)

target_compile_options(kernel PRIVATE ${ARCH_ADDITIONAL_OPTIONS})
target_link_options(kernel PRIVATE ${ARCH_ADDITIONAL_OPTIONS})

target_link_options(kernel PRIVATE -T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)
set_target_properties(kernel PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld)
