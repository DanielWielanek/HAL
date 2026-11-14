#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -Wshadow")

PathInfo()
   
################# CHECK OPENMP STUFF #########################
Message("-- Looking for OpenMP ...")
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    add_compile_definitions(HAVE_OPENMP)
    add_compile_options(${OpenMP_CXX_FLAGS})
	add_link_options(${OpenMP_CXX_FLAGS})
endif()

################# SETUP ROOT STUFF ############################
Message("-- Looking for ROOT ...")
find_package(ROOT 6.20.00 REQUIRED)

If(NOT DEFINED SKIP_ROOT_FEATURES_CHECKING)
  find_library(ROOT_XML_LIBRARY XMLIO PATHS ${ROOT_LIBRARY_DIR})
  if(NOT ROOT_XML_LIBRARY)
  	message(WARNING "ROOT XML library not found")
  else()
  	message(STATUS "ROOT XML found at ${ROOT_XML_LIBRARY}")
  endif()
  find_library(ROOT_SQLITE_LIBRARY RSQLite PATHS ${ROOT_LIBRARY_DIR})
  if(NOT ROOT_SQLITE_LIBRARY)
  	message(WARNING "ROOT SQlite library not found")
  else()
	message(STATUS "ROOT SQLITE found at  ${ROOT_SQLITE_LIBRARY}")
  endif()
endif()

################# SETUP GSL ###################################


Message("-- Looking for GSL ...")
find_package(GSL)
if(NOT GSL_FOUND)
  if(APPLE)
   set(GSL_DIR /usr/local)
  else()
    #unset(GSL_DIR)
    execute_process(COMMAND gsl-config --prefix OUTPUT_VARIABLE GSL_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
  endif()
  find_package(GSL REQUIRED)
endif()

################# SETUP SIMD ##################################
Message("-- SIMD Configuring ...")
if(SIMD STREQUAL "OFF")
	#scalar computing
	Message(STATUS "Scalar computing")
	add_compile_definitions(HAL_SIMD_FORCE_SCALAR)
elseif(SIMD STREQUAL "ON")
	message(STATUS "Simple simd")
elseif(SIMD STREQUAL "AUTO")
	message(STATUS "Simd in auto mode")
	SETUP_HAL_SIMD()
endif()

################# SETUP SOME VARIABLES ########################

include(GNUInstallDirs)
set(CMAKE_INSTALL_LIBDIR "lib")

set(SYSTEM_INCLUDE_DIRECTORIES
  ${ROOT_INCLUDE_DIRS}
  ${GSL_INCLUDE_DIRS}
  ${GSL_DIR}/include
)

set(BASE_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${ROOT_INCLUDE_DIR}
)

include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
link_directories(${ROOT_LIBRARY_DIR} ${GSL_LIBRARY_DIRS})

set(HALLIBDIR ${CMAKE_BINARY_DIR}/lib)  
set(LD_LIBRARY_PATH  ${HALLIBDIR} ${GSL_LIBRARY_DIR} ${LD_LIBRARY_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

PrintConfig()
SetHeaderVariables(${CMAKE_SOURCE_DIR})
WriteConfig()

