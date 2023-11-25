CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0 FATAL_ERROR)

Set(HAL_VERSION 0)
Set(HAL_MAJOR_VERSION 1)
project(Hal)
enable_language(Fortran)
#for test only
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

if(NOT DEFINED CMAKE_CXX_STANDARD)
    Message(WARNING "CMAke standard not set, setting to 17")
    Set(CMAKE_CXX_STANDARD 17)
endif()

Set(USE_FAIRROOT FALSE)
Set(USE_EXAMPLES FALSE)
#check if simpath detected
if(DEFINED ENV{SIMPATH})
    Set(USE_FAIRROOT TRUE)      
endif()
#check if fairroot disabled
if(DEFINED FAIRROOT)
    if(FAIRROOT STREQUAL "OFF")
        Set(USE_FAIRROOT FALSE)
    endif()
endif()
if(DEFINED EXAMPLES)
    if(EXAMPLES STREQUAL "ON")
        Set(USE_EXAMPLES TRUE)
    endif()
endif()

if (USE_FAIRROOT)
    include(cmake/modules/CMakeFairRoot.cmake)
  else()
    include(cmake/modules/CMakeListsExternal.cmake)
endif()


PrintConfig()
SetHeaderVariables(${CMAKE_SOURCE_DIR})
WriteConfig()

if (USE_FAIRROOT)
    add_subdirectory(fair)
endif()
add_subdirectory (data)
add_subdirectory (cuts) 
add_subdirectory (features)
add_subdirectory (analysis)
if(USE_EXAMPLES)
    add_subdirectory(examples)
endif()