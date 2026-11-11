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

  target_include_directories(${Int_LIB} PUBLIC ${INCLUDE_DIRECTORIES})
  target_link_libraries(${Int_LIB} ${Int_DEPENDENCIES})
  set_target_properties(${Int_LIB} PROPERTIES ${PROJECT_LIBRARY_PROPERTIES})

  if(LINKDEF)
    root_generate_dictionary(G__${Int_LIB} ${HDRS} MODULE ${Int_LIB} LINKDEF ${LINKDEF})
    if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
      set_target_properties(G__${Int_LIB} PROPERTIES COMPILE_FLAGS "-Wno-overloaded-virtual -Wno-shadow -Wno-deprecated-declarations -Wno-unused-parameter")
    else()
      set_target_properties(G__${Int_LIB} PROPERTIES COMPILE_FLAGS "-Wno-ctor-dtor-privacy -Wno-overloaded-virtual -Wno-null-pointer-subtraction -Wno-shadow -Wno-deprecated-declarations -Wno-unused-parameter")
    endif()
  endif(LINKDEF)


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

Macro(GENERATE_EXECUTABLE)

#  If(IWYU_FOUND)
#    Set(HeaderRuleName "${EXE_NAME}_HEADER_RULES")
#    CHECK_HEADERS("${SRCS}" "${INCLUDE_DIRECTORIES}" ${HeaderRuleName})
#  EndIf(IWYU_FOUND)

  ############### build the library #####################
  Add_Executable(${EXE_NAME} ${SRCS})
  target_link_libraries(${EXE_NAME} ${DEPENDENCIES})

  ############### install the library ###################
  if(DEFINED BIN_DESTINATION)
    install(TARGETS ${EXE_NAME} DESTINATION ${BIN_DESTINATION})
  else(DEFINED BIN_DESTINATION)
    install(TARGETS ${EXE_NAME} DESTINATION bin)
  endif(DEFINED BIN_DESTINATION)

  Set(EXE_NAME)
  Set(SRCS)
  Set(DEPENDENCIES)

EndMacro(GENERATE_EXECUTABLE)
