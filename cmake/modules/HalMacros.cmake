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


function(CHANGE_FILE_EXTENSION FILE_EXT1 FILE_EXT2 OUTVAR LIST)
  if("${FILE_EXT1}" MATCHES "^\\*+(\\..*)$")
    set(FILE_EXT1_NEW "${CMAKE_MATCH_1}")
  else()
    set(FILE_EXT1_NEW "${FILE_EXT1}")
  endif()

  if("${FILE_EXT2}" MATCHES "^\\*+(\\..*)$")
    set(FILE_EXT2_NEW "${CMAKE_MATCH_1}")
  else()
    set(FILE_EXT2_NEW "${FILE_EXT2}")
  endif()

  set(_result)

  foreach(_current_FILE IN LISTS LIST)
    string(REGEX REPLACE
      "^(.*)${FILE_EXT1_NEW}$"
      "\\1${FILE_EXT2_NEW}"
      _new_file
      "${_current_FILE}"
    )
    list(APPEND _result "${_new_file}")
  endforeach()

  set(${OUTVAR} "${_result}" PARENT_SCOPE)
endfunction(CHANGE_FILE_EXTENSION)

Function(GENERATE_EXECUTABLE EXE_NAME EXE_SRC DEPENDENCIES)

  Add_Executable(${EXE_NAME} ${EXE_SRC})
  target_link_libraries(${EXE_NAME} ${DEPENDENCIES})

  if(DEFINED BIN_DESTINATION)
    install(TARGETS ${EXE_NAME} DESTINATION ${BIN_DESTINATION})
  else(DEFINED BIN_DESTINATION)
    install(TARGETS ${EXE_NAME} DESTINATION bin)
  endif(DEFINED BIN_DESTINATION)

EndFunction(GENERATE_EXECUTABLE)


 ################################################################################
 #    Macro for building headers outside of include dir                         #
 ################################################################################
Function(GENERATE_LIBRARY_HAL)
  Include_Directories(${INCLUDE_DIRECTORIES})
  if(NOT LINKDEF)
  	set(LINKDEF "${LIBRARY_NAME}LinkDef.h")
  endif()
  
  if(NOT LIBRARY_NAME)
    message(FATAL_ERROR "LIBRARY_NAME not defined in ${CMAKE_CURRENT_SOURCE_DIR}")
  endif()
  
  set(Int_LIB ${LIBRARY_NAME})

  set(Int_SRCS ${SRCS})

  if(NOT DICTIONARY)
    set(DICTIONARY "${CMAKE_CURRENT_BINARY_DIR}/G__${Int_LIB}Dict")
  elseif(NOT IS_ABSOLUTE "${DICTIONARY}")
    set(DICTIONARY "${CMAKE_CURRENT_SOURCE_DIR}/${DICTIONARY}")
  endif()

  if(HEADERS)
    set(HDRS ${HEADERS})
  else()
    CHANGE_FILE_EXTENSION(*.cxx *.h HDRS "${SRCS}")
  endif(HEADERS)

  if(INCLUDE_HAL_SUBIDIR)
    install(FILES ${HDRS} DESTINATION include/Hal)
  else()
    install(FILES ${HDRS} DESTINATION include)
  endif()
  
  if(LINKDEF)
    if(IS_ABSOLUTE "${LINKDEF}")
      set(Int_LINKDEF "${LINKDEF}")
    else()
      set(Int_LINKDEF "${CMAKE_CURRENT_SOURCE_DIR}/${LINKDEF}")
    endif()

    ROOT_GENERATE_DICTIONARY(${DICTIONARY} ${HDRS} LINKDEF ${Int_LINKDEF})

    list(APPEND Int_SRCS ${DICTIONARY})

    set_source_files_properties(${DICTIONARY} PROPERTIES COMPILE_FLAGS "-Wno-old-style-cast")
  endif()


  If (ROOT_FOUND_VERSION LESS 59999)
    #ROOT_GENERATE_ROOTMAP()
  EndIf()

  Add_Library(${Int_LIB} SHARED ${Int_SRCS} ${NO_DICT_SRCS} ${LINKDEF})
  
  target_link_libraries(${Int_LIB} PUBLIC ${DEPENDENCIES})

  install(TARGETS ${Int_LIB} DESTINATION lib)

EndFunction(GENERATE_LIBRARY_HAL)

 ################################################################################
 #    Write config files                                                        #
 ################################################################################

Function(GenerateConfig HAL HAL_PATH )
	file(WRITE ${HAL} "#!/bin/bash\n")
	file(APPEND ${HAL} "export PATH=$ENV{PATH}\n")
	file(APPEND ${HAL} "export LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH}\n")
	file(APPEND ${HAL} "export HAL=${HAL_PATH}\n")
	file(APPEND ${HAL} "export PATH=\$PATH:\$HAL/bin\n")
	file(APPEND ${HAL} "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HAL_PATH}/lib\n")
	file(APPEND ${HAL} "export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH::${HAL_PATH}/lib\n")
	if(DEFINED JSROOT_DIR)
	       file(APPEND ${HAL} "export JSROOT=${JSROOT_DIR}")
	endif()
EndFunction(GenerateConfig)

Function(WriteConfig)
	GenerateConfig(${CMAKE_BINARY_DIR}/config.sh ${CMAKE_BINARY_DIR})
	GenerateConfig(${CMAKE_BINARY_DIR}/hal_config.sh ${CMAKE_INSTALL_PREFIX})
	install(FILES ${CMAKE_BINARY_DIR}/hal_config.sh DESTINATION bin)
EndFunction(WriteConfig)
