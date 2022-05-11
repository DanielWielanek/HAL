CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0 FATAL_ERROR)

Set(HAL_VERSION 0)
Set(HAL_MAJOR_VERSION 1)
project(Hal)
enable_language(Fortran)


if (DEFINED ENV{SIMPATH})
    include(cmake/modules/CMakeFairRoot.cmake)
  else()
    set(DISABLE_JSROOT)
    
    include(cmake/modules/CMakeListsExternal.cmake)
endif()
