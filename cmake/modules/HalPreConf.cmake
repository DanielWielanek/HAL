#for test only
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

################# COMPILER CHECK ##############################
if(NOT DEFINED CMAKE_CXX_STANDARD)
    Message(WARNING "CMAke standard not set, setting to 17")
    Set(CMAKE_CXX_STANDARD 17)
endif()

# be sure that we have support to std::filesystem
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
      link_libraries(stdc++fs)
endif()

include(CheckCXXCompilerFlag)

################# FLAG CHECK ##################################

Set(USE_EXAMPLES FALSE)
if(DEFINED EXAMPLES)
    if(EXAMPLES STREQUAL "ON")
        Set(USE_EXAMPLES TRUE)
    endif()
endif()

Set(WARNING_FLAGS FALSE)
if(DEFINED  FULL_FLAGS)
	if(FULL_FLAGS STREQUAL "ON")
		Set(WARNING_FLAGS TRUE)
	endif()
endif()

if(DEFINED CIA)
    set(CIA TRUE)
    add_compile_definitions(__CIA__)
else()
	set(CIA FALSE)
endif()

if(NOT DEFINED INCLUDE_HAL_SUBIDIR)
  Set(INCLUDE_HAL_SUBIDIR FALSE)
endif()

################# SIMD CHECK ##################################
if(NOT DEFINED SIMD)
    set(SIMD OFF)
endif()
	

#set(MY_WARNINGS
#    -Wall
#    -Wextra
#    -Wreorder
#)
