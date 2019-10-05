find_package(Doxygen)
if (DOXYGEN_FOUND)
  doxygen_add_docs(doxygen ${PROJECT_SOURCE_DIR})
endif()
