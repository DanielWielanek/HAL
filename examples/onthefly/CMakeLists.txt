Set(INCLUDE_DIRECTORIES
${HAL_FEATURES}
${HAL_DATA}
${HAL_CUTS}
${HAL_CORE}/analysis/base
${HAL_CORE}/data/dataformat
${HAL_CORE}/data/io
${HAL_CORE}/examples/onthefly
${HAL_CORE}/examples/onthefly/cuts
${HAL_CORE}/examples/onthefly/experiment
${HAL_CORE}/examples/onthefly/generators
${HAL_SPECTRA}
)

Set(SYSTEM_INCLUDE_DIRECTORIES
 ${ROOT_INCLUDE_DIR} 
 ${BASE_INCLUDE_DIRECTORIES}
)

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${GSL_LIBRARY_DIRS} 
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
generators/OTFEventGenerator.cxx
generators/OTFEventGeneratorDecay.cxx
generators/OTFEventGeneratorDecayer.cxx
cuts/OTFDetectorHitsCut.cxx
cuts/OTFPidHypoCut.cxx
experiment/OTFEfficiency.cxx
experiment/OTFExperiment.cxx
experiment/OTFExperimentSubTask.cxx
experiment/OTFMisidentification.cxx
experiment/OTFMomentumSmearing.cxx
)

Set(HEADERS )
Set(LINKDEF HalOTFLinkDef.h)
Set(LIBRARY_NAME HalOTF)
Set(DEPENDENCIES HalAna )

GENERATE_LIBRARY_HAL()

