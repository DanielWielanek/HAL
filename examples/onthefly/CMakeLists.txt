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

Set(LIBRARY_NAME HalOTF)
Set(DEPENDENCIES HalAna )

GENERATE_LIBRARY_HAL()

