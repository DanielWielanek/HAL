CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0 FATAL_ERROR)

Set(HAL_VERSION 0)
Set(HAL_MAJOR_VERSION 1)
project(Hal)
enable_language(Fortran)
#for test only
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
Set(CMAKE_CXX_STANDARD 17)

if (DEFINED ENV{SIMPATH})
    include(cmake/modules/CMakeFairRoot.cmake)
  else()
    include(cmake/modules/CMakeListsExternal.cmake)
endif()

SetHeaderVariables(${CMAKE_SOURCE_DIR})
WriteConfig()

if (DEFINED ENV{SIMPATH})
    add_subdirectory(fair)
endif()
add_subdirectory (data)
add_subdirectory (cuts) 
add_subdirectory (features)
add_subdirectory (analysis)
if(DEFINED EXAMPLES)
add_subdirectory(examples)
endif()