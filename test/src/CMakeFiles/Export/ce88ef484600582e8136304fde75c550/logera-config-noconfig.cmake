#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "logera" for configuration ""
set_property(TARGET logera APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(logera PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/liblogera.so"
  IMPORTED_SONAME_NOCONFIG "liblogera.so"
  )

list(APPEND _cmake_import_check_targets logera )
list(APPEND _cmake_import_check_files_for_logera "${_IMPORT_PREFIX}/lib/liblogera.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
