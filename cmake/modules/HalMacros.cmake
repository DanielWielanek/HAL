Macro(SetFairBase)
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
EndMacro(SetFairBase)


Macro(CheckFairCXX)
 If(NOT _HAS_CXX11_FLAG)
      Message(FATAL_ERROR "The used C++ compiler (${CMAKE_CXX_COMPILER}) does not support C++11. HAL can only be compiled with compilers supporting C++11. Please install such an compiler.")
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
EndMacro(CheckFairCXX)

Macro(CheckNoFairCXX)
SET(CXX_STANDARD_REQUIRED C++11)
 If(NOT _HAS_CXX11_FLAG)
   #   Message(FATAL_ERROR "The used C++ compiler (${CMAKE_CXX_COMPILER}) does not support C++11. HAL can only be compiled with compilers supporting C++11. Please install such an compiler.")
    EndIf()
    
EndMacro(CheckNoFairCXX)



Macro(PathInfo)
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
EndMacro(PathInfo)

Macro(FindDependencies)


EndMacro(FindDependencies)


