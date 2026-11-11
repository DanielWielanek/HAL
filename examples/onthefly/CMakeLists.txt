set(SRCS
OTFComplexEvent.cxx
OTFData.cxx
OTFIOManager.cxx
OTFMcEvent.cxx
OTFMcEventInterface.cxx
OTFReader.cxx
OTFRecoEvent.cxx
OTFRecoEventInterface.cxx
OTFSource.cxx
OTFEventGenerator.cxx
OTFEventGeneratorDecay.cxx
OTFEventGeneratorDecayer.cxx
)

Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
)

Set(HEADERS )
Set(LINKDEF HalOTFLinkDef.h)
Set(LIBRARY_NAME HalOTF)
Set(DEPENDENCIES HalAna )

GENERATE_LIBRARY_HAL()

