Set(INCLUDE_DIRECTORIES
${HAL_FEATURES}
${HAL_DATA}
${HAL_CORE}/analysis/base
${HAL_CORE}/data/dataformat
${HAL_CORE}/data/io
${HAL_CORE}/examples/debugformat
)

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

Set(LIBRARY_NAME HalDbg)
Set(DEPENDENCIES HalAna )

GENERATE_LIBRARY_HAL()

