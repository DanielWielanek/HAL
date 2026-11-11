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

Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
)

Set(HEADERS )
Set(LINKDEF HalDbgLinkDef.h)
Set(LIBRARY_NAME HalDbg)
Set(DEPENDENCIES HalAna )

GENERATE_LIBRARY_HAL()

