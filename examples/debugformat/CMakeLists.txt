Set(INCLUDE_DIRECTORIES
${HAL_FEATURES}
${HAL_DATA}
${HAL_CORE}/analysis/base
${HAL_CORE}/data/dataformat
${HAL_CORE}/data/io
${HAL_CORE}/examples/debugformat
)

Set(SYSTEM_INCLUDE_DIRECTORIES
 ${ROOT_INCLUDE_DIR} 
 ${BASE_INCLUDE_DIRECTORIES}
)

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

 
Link_Directories( ${LINK_DIRECTORIES})

set(SRCS
DbgComplexEvent.cxx
DbgData.cxx
DbgIOManager.cxx
DbgMcEvent.cxx
DbgMcEventInterface.cxx
DbgReader.cxx
DbgRecoEvent.cxx
DbgRecoEventInterface.cxx
DbgSource.cxx
DbgTask.cxx
)

Set(HEADERS )
Set(LINKDEF HalDbgLinkDef.h)
Set(LIBRARY_NAME HalDbg)
Set(DEPENDENCIES HalAna )

GENERATE_LIBRARY_HAL()

