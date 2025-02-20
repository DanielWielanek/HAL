CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0 FATAL_ERROR)

Set(HAL_VERSION 0)
Set(HAL_MAJOR_VERSION 1)
project(Hal)
enable_language(Fortran)
#for test only
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

if(NOT DEFINED CMAKE_CXX_STANDARD)
    #Message(WARNING "CMAke standard not set, setting to 17")
    Set(CMAKE_CXX_STANDARD 17)
endif()


#check if simpath detected

Set(USE_EXAMPLES FALSE)
if(DEFINED EXAMPLES)
    if(EXAMPLES STREQUAL "ON")
        Set(USE_EXAMPLES TRUE)
    endif()
endif()

include(cmake/modules/CMakeListsExternal.cmake)


if(DEFINED CIA)
    add_compile_definitions(__CIA__ GLOBAL)
endif()
if(NOT DEFINED INCLUDE_HAL_SUBIDIR)
  Set(INCLUDE_HAL_SUBIDIR FALSE)
endif()


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