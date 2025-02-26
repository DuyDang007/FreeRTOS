#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "freertos_bsp::libmetal" for configuration ""
set_property(TARGET freertos_bsp::libmetal APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(freertos_bsp::libmetal PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "ASM;C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/usr/local/lib/libmetal.a"
  )

list(APPEND _cmake_import_check_targets freertos_bsp::libmetal )
list(APPEND _cmake_import_check_files_for_freertos_bsp::libmetal "${_IMPORT_PREFIX}/usr/local/lib/libmetal.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
