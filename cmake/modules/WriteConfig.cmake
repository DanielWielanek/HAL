MACRO(GenerateConfig HAL HAL_PATH )
	file(WRITE ${HAL} "#!/bin/bash\n")
	file(APPEND ${HAL} "export PATH=$ENV{PATH}\n")
	file(APPEND ${HAL} "export LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH}\n")
	file(APPEND ${HAL} "export HAL=${HAL_PATH}\n")
	file(APPEND ${HAL} "export PATH=\$PATH:\$HAL/bin\n")
	file(APPEND ${HAL} "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HAL_PATH}/lib\n")
	if(DEFINED JSROOT_DIR)
	       file(APPEND ${HAL} "export JSROOT=${JSROOT_DIR}")
	endif()
ENDMACRO(GenerateConfig)


MACRO(WriteConfig)
	GenerateConfig(${CMAKE_BINARY_DIR}/config.sh ${CMAKE_BINARY_DIR})
	GenerateConfig(${CMAKE_BINARY_DIR}/hal_config.sh ${CMAKE_INSTALL_PREFIX})
	install(FILES ${CMAKE_BINARY_DIR}/hal_config.sh DESTINATION bin)

ENDMACRO(WriteConfig)