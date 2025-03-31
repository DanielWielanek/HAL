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
      if(NOT DEFINED ENV{FAIRROOTPATH} AND NOT DEFINED FAIRROOTPATH)
         MESSAGE(FATAL_ERROR "You did not define the environment variable FAIRROOTPATH which is needed to find FairRoot. Please set this variable and execute cmake again.")
         SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/fair" ${CMAKE_MODULE_PATH})
         MESSAGE(WARNING "CMP ${CMAKE_MODULE_PATH}")
      endif()
      if(NOT FAIRROOTPATH)
        SET(FAIRROOTPATH $ENV{FAIRROOTPATH})
      endif()
    EndIf (FAIRBASE)
    set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules_old" ${CMAKE_MODULE_PATH})
    set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules"  ${CMAKE_MODULE_PATH})
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

 ################################################################################
 #    Macro for building headers outside of include dir                         #
 ################################################################################
Macro(GENERATE_LIBRARY_HAL)

  # TODO: remove this backwards-compatibility check when no longer needed
  if(DEFINED FAIRROOT_LIBRARY_PROPERTIES AND NOT DEFINED PROJECT_LIBRARY_PROPERTIES)
    set(PROJECT_LIBRARY_PROPERTIES ${FAIRROOT_LIBRARY_PROPERTIES})
  endif()



  set(Int_LIB ${LIBRARY_NAME})

  Set(HeaderRuleName "${Int_LIB}_HEADER_RULES")
  Set(DictName "G__${Int_LIB}Dict.cxx")

  If(NOT DICTIONARY)
    Set(DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/${DictName})
  EndIf(NOT DICTIONARY)

  If( IS_ABSOLUTE ${DICTIONARY})
    Set(DICTIONARY ${DICTIONARY})
  Else( IS_ABSOLUTE ${DICTIONARY})
    Set(Int_DICTIONARY ${CMAKE_CURRENT_SOURCE_DIR}/${DICTIONARY})
  EndIf( IS_ABSOLUTE ${DICTIONARY})

  Set(Int_SRCS ${SRCS})

  If(HEADERS)
    Set(HDRS ${HEADERS})
  Else(HEADERS)
    CHANGE_FILE_EXTENSION(*.cxx *.h HDRS "${SRCS}")
  EndIf(HEADERS)

  If(IWYU_FOUND)
    Set(_INCLUDE_DIRS ${INCLUDE_DIRECTORIES} ${SYSTEM_INCLUDE_DIRECTORIES})
    CHECK_HEADERS("${Int_SRCS}" "${_INCLUDE_DIRS}" ${HeaderRuleName})
  EndIf(IWYU_FOUND)

  If(INCLUDE_HAL_SUBIDIR)
    install(FILES ${HDRS} DESTINATION include/Hal)
  Else()
    install(FILES ${HDRS} DESTINATION include)
  Endif()
  
  
  If(LINKDEF)
    If( IS_ABSOLUTE ${LINKDEF})
      Set(Int_LINKDEF ${LINKDEF})
    Else( IS_ABSOLUTE ${LINKDEF})
      Set(Int_LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/${LINKDEF})
    EndIf( IS_ABSOLUTE ${LINKDEF})
    ROOT_GENERATE_DICTIONARY()
    SET(Int_SRCS ${Int_SRCS} ${DICTIONARY})
    SET_SOURCE_FILES_PROPERTIES(${DICTIONARY}
       PROPERTIES COMPILE_FLAGS "-Wno-old-style-cast"
    )
  EndIf(LINKDEF)


  If (ROOT_FOUND_VERSION LESS 59999)
    ROOT_GENERATE_ROOTMAP()
  EndIf()

  set(Int_DEPENDENCIES)
  foreach(d ${DEPENDENCIES})
    get_filename_component(_ext ${d} EXT)
    If(NOT _ext MATCHES a$)
      set(Int_DEPENDENCIES ${Int_DEPENDENCIES} ${d})
    Else()
      Message("Found Static library with extension ${_ext}")
      get_filename_component(_lib ${d} NAME_WE)
      set(Int_DEPENDENCIES ${Int_DEPENDENCIES} ${_lib})
    EndIf()
  endforeach()

  ############### build the library #####################
  If(${CMAKE_GENERATOR} MATCHES Xcode)
    Add_Library(${Int_LIB} SHARED ${Int_SRCS} ${NO_DICT_SRCS} ${HDRS} ${LINKDEF})
  Else()
    Add_Library(${Int_LIB} SHARED ${Int_SRCS} ${NO_DICT_SRCS} ${LINKDEF})
  EndIf()
  

  target_link_libraries(${Int_LIB} ${Int_DEPENDENCIES})
  set_target_properties(${Int_LIB} PROPERTIES ${PROJECT_LIBRARY_PROPERTIES})

  ############### install the library ###################
  install(TARGETS ${Int_LIB} DESTINATION lib)

  Set(LIBRARY_NAME)
  Set(DICTIONARY)
  Set(LINKDEF)
  Set(SRCS)
  Set(HEADERS)
  Set(NO_DICT_SRCS)
  Set(DEPENDENCIES)

EndMacro(GENERATE_LIBRARY_HAL)

