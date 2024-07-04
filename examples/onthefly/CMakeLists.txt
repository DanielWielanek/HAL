Set(INCLUDE_DIRECTORIES
${HAL_FEATURES}
${HAL_DATA}
${HAL_CORE}/analysis/base
${HAL_CORE}/data/dataformat
${HAL_CORE}/data/io
${HAL_CORE}/examples/onthefly
${HAL_SPECTRA}
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
)

Set(HEADERS )
Set(LINKDEF HalOTFLinkDef.h)
Set(LIBRARY_NAME HalOTF)
Set(DEPENDENCIES HalAna )

GENERATE_LIBRARY_HAL()

