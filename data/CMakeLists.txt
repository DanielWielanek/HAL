Set(INCLUDE_DIRECTORIES

${HAL_FEATURES}
${HAL_DATA}
)

Set(SYSTEM_INCLUDE_DIRECTORIES
 ${ROOT_INCLUDE_DIR} 
 ${BASE_INCLUDE_DIRECTORIES}
)

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${GSL_LIBRARY_DIRS} 
  ${FAIRROOT_LIBRARY_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

 
Link_Directories( ${LINK_DIRECTORIES})

set(SRCS

dataformat/DataFormatManager.cxx
dataformat/Event.cxx
dataformat/Track.cxx
dataformat/TwoTrack.cxx
dataformat/McEvent.cxx
dataformat/McTrack.cxx
dataformat/EventInterface.cxx
dataformat/EventInterfaceAdvanced.cxx
dataformat/TrackInterface.cxx
dataformat/TrackInterfaceAdvanced.cxx
dataformat/McTrackInterface.cxx
dataformat/VirtualTrack.cxx
dataformat/VirtualEvent.cxx
dataformat/TrackClones.cxx
dataformat/DataFormat.cxx
dataformat/compound/ComplexEvent.cxx
dataformat/compound/ComplexTrack.cxx
dataformat/compound/SmearedEvent.cxx
dataformat/compound/SmearedTrack.cxx
dataformat/detector/ToFTrack.cxx
dataformat/detector/TpcTrack.cxx
dataformat/detector/ExpTrack.cxx
dataformat/detector/ExpEvent.cxx
dataformat/detector/ExpEventInterface.cxx
dataformat/detector/DetectorTrack.cxx
dataformat/detector/DetectorEvent.cxx
dataformat/hiddeninfo/V0Track.cxx

io/AnalysisManager.cxx
io/DataManager.cxx
io/MagField.cxx
io/RootIOManager.cxx
io/Source.cxx
io/Task.cxx
io/IOManager.cxx


)

Set(HEADERS )
Set(LINKDEF HalDataLinkDef.h)
Set(LIBRARY_NAME HalData)
Set(DEPENDENCIES HalFeatures)

GENERATE_LIBRARY_HAL()

