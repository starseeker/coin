#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Coin::Coin" for configuration "Release"
set_property(TARGET Coin::Coin APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Coin::Coin PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libCoin.so.80.0.7"
  IMPORTED_SONAME_RELEASE "libCoin.so.80"
  )

list(APPEND _cmake_import_check_targets Coin::Coin )
list(APPEND _cmake_import_check_files_for_Coin::Coin "${_IMPORT_PREFIX}/lib/libCoin.so.80.0.7" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
