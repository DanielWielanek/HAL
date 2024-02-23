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


Option(USE_FAIRROOT "Build FairRoot connected code" OFF)
Option(USE_EXAMPLES "Build examples" OFF)


if(USE_FAIRROOT) 
    if(NOT DEFINED ENV{SIMPATH} AND NOT DEFINED SIMPATH)
    message (FATAL_ERROR "\
    You did not define the environment variable SIMPATH or define SIMPATH when calling cmake. \
    Either of the two is needed to properly find the external packages. \
    Please either set this environment variable or pass -DSIMPATH=<path> and execute cmake again. \
    ")
    endif()
    if (NOT DEFINED SIMPATH)
        set(SIMPATH $ENV{SIMPATH})
    endif()
endif()

#check if fairroot disabled



if (USE_FAIRROOT)
    include(cmake/modules/CMakeFairRoot.cmake)
  else()
    include(cmake/modules/CMakeListsExternal.cmake)
endif()

if(DEFINED CIA)
    add_compile_definitions(__CIA__ GLOBAL)
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