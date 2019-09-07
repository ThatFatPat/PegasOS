find_program(GRUB_MKRESCUE grub-mkrescue)
if (GRUB_MKRESCUE)
  set(ISO_FILES_DIR ${CMAKE_CURRENT_BINARY_DIR}/files)
  configure_file(grub.cfg ${ISO_FILES_DIR}/boot/grub/grub.cfg COPYONLY)
  add_custom_command(OUTPUT PegasOS.iso DEPENDS kernel
                     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:kernel> ${ISO_FILES_DIR}/boot/
                     COMMAND ${GRUB_MKRESCUE} -o ${CMAKE_CURRENT_BINARY_DIR}/PegasOS.iso ${ISO_FILES_DIR})
  add_custom_target(iso ALL DEPENDS PegasOS.iso)
else()
  message(WARNING "Can't find grub-mkrescue, ISO target will be unavailable")
endif()
