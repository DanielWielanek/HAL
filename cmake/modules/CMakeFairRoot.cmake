CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0 FATAL_ERROR)


if(NOT DEFINED HAL_SUBDIR_BUILD)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -Wshadow")

    
    FIND_PATH(FAIRBASE NAMES FairRun.h  PATHS
      ${CMAKE_SOURCE_DIR}/base/steer
      NO_DEFAULT_PATH
    )
    
    If (FAIRBASE)
      Message(STATUS "Found FAIRBASE")
      SET(FAIRBASE ${FAIRBASE})
    Else (FAIRBASE)
      Message(STATUS "NOT Found FAIRBASE")
      IF(NOT DEFINED ENV{FAIRROOTPATH})
         MESSAGE(FATAL_ERROR "You did not define the environment variable FAIRROOTPATH which is needed to find FairRoot. Please set this variable and execute cmake again.")
         SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/fair" ${CMAKE_MODULE_PATH})
         MESSAGE(WARNING "CMP ${CMAKE_MODULE_PATH}")
      ENDIF(NOT DEFINED ENV{FAIRROOTPATH})
      SET(FAIRROOTPATH $ENV{FAIRROOTPATH})
    EndIf (FAIRBASE)
    
    set(CMAKE_MODULE_PATH "$ENV{FAIRROOTPATH}/share/fairbase/cmake/modules_old" ${CMAKE_MODULE_PATH})
    set(CMAKE_MODULE_PATH "$ENV{FAIRROOTPATH}/share/fairbase/cmake/modules"  ${CMAKE_MODULE_PATH})
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules"  ${CMAKE_MODULE_PATH})
    IF(FAIRROOTPATH)
      Set(CheckSrcDir "${FAIRROOTPATH}/share/fairbase/cmake/checks")
    ELSE(FAIRROOTPATH)
      Set(CheckSrcDir "${CMAKE_SOURCE_DIR}/cmake/checks")
    ENDIF(FAIRROOTPATH)
    
    if(FAIRROOTPATH)
      find_package(FairRoot)
      SET(MY_ROOT_INCLUDE_PATH ${FAIRROOT_INCLUDE_DIR})
      If(CMAKE_INSTALL_PREFIX)
         SET(MY_ROOT_INCLUDE_PATH ${MY_ROOT_INCLUDE_PATH} "${CMAKE_INSTALL_PREFIX}/include")
      EndIf(CMAKE_INSTALL_PREFIX)
    endif(FAIRROOTPATH)
    
    include(CheckCXX11Features)
    
    
    include(FairMacros)
    include(Hal)
    include(CheckCompiler)
    include(WriteConfig)
    
    If(NOT _HAS_CXX11_FLAG)
      Message(FATAL_ERROR "The used C++ compiler (${CMAKE_CXX_COMPILER}) does not support C++11. NicaFemto can only be compiled with compilers supporting C++11. Please install such an compiler.")
    EndIf()
    
        Execute_process(COMMAND $ENV{SIMPATH}/bin/fairsoft-config --cxxflags OUTPUT_VARIABLE _res_fairsoft_config OUTPUT_STRIP_TRAILING_WHITESPACE)
        String(FIND ${_res_fairsoft_config} "-std=c++11" POS_C++11)
        If(${POS_C++11} EQUAL -1)
          String(FIND ${_res_fairsoft_config} "-std=c++14" POS_C++11)
          If(${POS_C++11} EQUAL -1)
            String(FIND ${_res_fairsoft_config} "-std=c++17" POS_C++11)
            If(${POS_C++11} EQUAL -1)
              Message(FATAL_ERROR "FairSoft wasn't compiled with support for c++11 or above. Please recompile FairSoft with a compiler which supports c++11, c++14 or c++17.")
            EndIf()
          EndIf()
        EndIf()


    Set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
    Set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
    Set(INCLUDE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/include")
    
    
    Option(USE_PATH_INFO "Information from PATH and LD_LIBRARY_PATH are used." OFF)
    If(USE_PATH_INFO)
      Set(PATH $ENV{PATH})
      If (APPLE)
        Set(LD_LIBRARY_PATH $ENV{DYLD_LIBRARY_PATH})
      Else (APPLE)
        Set(LD_LIBRARY_PATH $ENV{LD_LIBRARY_PATH})
      EndIf (APPLE)
    Else(USE_PATH_INFO)
      STRING(REGEX MATCHALL "[^:]+" PATH $ENV{PATH})
    EndIf(USE_PATH_INFO)
    
    IF(NOT UNIX)
      MESSAGE(FATAL_ERROR "You're not on an UNIX system. The project was up to now only tested on UNIX systems, so we break here. If you want to go on please edit the CMakeLists.txt in the source directory.")
    ENDIF(NOT UNIX)  
    
    
    IF(NOT DEFINED ENV{SIMPATH})
      MESSAGE(FATAL_ERROR "You did not define the environment variable SIMPATH which is nedded to find the external packages. Please set this variable and execute cmake again.")
    ENDIF(NOT DEFINED ENV{SIMPATH})
    
        find_package(ROOT 6.00.00 REQUIRED)
        if("${ROOT_VERSION_MAJOR}.${ROOT_VERSION_MINOR}" VERSION_GREATER 6.16)
          Execute_Process(COMMAND ${ROOT_CONFIG_EXECUTABLE} --has-vmc
                          OUTPUT_VARIABLE ROOT_vmc_FOUND
                         )
          String(STRIP ${ROOT_vmc_FOUND} ROOT_vmc_FOUND)
          If(NOT ROOT_vmc_FOUND)
            set(CMAKE_PREFIX_PATH $ENV{SIMPATH} ${CMAKE_PREFIX_PATH})
            find_package2(PUBLIC VMC REQUIRED)
            set(VMCLIB VMCLibrary)
          endif()
        endif()
    Set(SIMPATH $ENV{SIMPATH})
    Set(Boost_NO_SYSTEM_PATHS TRUE)
    Set(Boost_NO_BOOST_CMAKE TRUE)
    Set(BOOST_ROOT ${SIMPATH})
    Set(GSL_DIR ${SIMPATH})

    Message("-- Looking for Boost ...")
      # If an older version of boost is found both of the variables below are
      # cached and in a second cmake run, a good boost version is found even 
      # if the version is to old. 
      # To overcome this problem both variables are cleared before checking
      # for boost.
    Unset(Boost_INCLUDE_DIR CACHE)
    Unset(Boost_LIBRARY_DIRS CACHE)
    find_package(Boost 1.48 COMPONENTS program_options thread system timer serialization regex log log_setup filesystem)
    If (Boost_FOUND)
      Set(Boost_Avail 1)
    Else (Boost_FOUND)
      Set(Boost_Avail 0)
    EndIf (Boost_FOUND)

    Message("-- Looking for GSL ...")
    find_package(GSL)
        if(NOT GSL_FOUND)
          if(APPLE)
            set(GSL_DIR /usr/local)
          else()
            unset(GSL_DIR)
          endif()
          find_package(GSL REQUIRED)
        endif()
    Find_Package(FairLogger)
    Find_Package(FairMQ)  
  
    SetBasicVariables()
    
    Set(BASE_INCLUDE_DIRECTORIES
        ${BASE_INCLUDE_DIRECTORIES} 
                ${VMC_INCLUDE_DIRS}
        ${SYSTEM_INCLUDE_DIRECTORIES} 
    )
    SET(NICALIBDIR ${CMAKE_BINARY_DIR}/lib)  
    SET(LD_LIBRARY_PATH  ${NICALIBDIR} ${GSL_LIBRARY_DIR} ${LD_LIBRARY_PATH})

    If(CMAKE_SYSTEM_NAME MATCHES Darwin)
      Include(CheckIncludeFile)
      Check_Include_File("_types/_uint8_t.h" _UINT8_T_EXIST)
    EndIf()

    Check_Compiler()
    SET(FAIRROOT_LIBRARY_PROPERTIES ${FAIRROOT_LIBRARY_PROPERTIES}
          VERSION "${HAL_VERSION}"
          SOVERSION "${HAL_MAJOR_VERSION}"
    )
    SetHeaderVariables(${CMAKE_SOURCE_DIR})
    WriteConfig()
else()
    if(EXISTS "${CMAKE_SOURCE_DIR}/hal/cmake/modules" )
       set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/hal/cmake/modules"  ${CMAKE_MODULE_PATH})
    else()
       set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/hal/cmake/modules"  ${CMAKE_MODULE_PATH})
    endif()
    include(NicaFemto)
    if(EXISTS "${CMAKE_SOURCE_DIR}/hal")
       SetHeaderVariables("${CMAKE_SOURCE_DIR}/hal")
    else()
       SetHeaderVariables("${CMAKE_SOURCE_DIR}/external/hal")
    endif()    
endif()
if(DEFINED DISABLE_GSL)
    add_definitions(-DDISABLE_GSL)
endif()

if(DEFINED DISABLE_SQLITE)
    add_definitions(-DDISABLE_SQLITE)
endif()
#add_subdirectory (dataformat)
#add_subdirectory (cuts) 
add_subdirectory (features)
#add_subdirectory (halgenerators)   
#add_subdirectory (analysis)

