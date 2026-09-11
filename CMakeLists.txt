CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0 FATAL_ERROR)

Set(HAL_VERSION 0)
Set(HAL_MAJOR_VERSION 1)
project(Hal)
enable_language(Fortran)

include(cmake/modules/HalPreConf.cmake)
include(cmake/modules/CMakeListsExternal.cmake)

PrintConfig()
SetHeaderVariables(${CMAKE_SOURCE_DIR})
WriteConfig()

add_subdirectory (data)
add_subdirectory (cuts) 
add_subdirectory (features)
add_subdirectory (analysis)
if(USE_EXAMPLES)
    add_subdirectory(examples)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external)
    add_subdirectory(external)
endif()
