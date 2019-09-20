target_include_directories(kernel PRIVATE include)

target_sources(
  kernel PRIVATE

  boot/early_entry.cpp
  boot/boot.S
  boot/console32.S
  mmu.cpp mp.cpp
)
