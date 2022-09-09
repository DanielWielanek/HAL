# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
set(INCLUDE_DIRECTORIES
${HAL_ANA}
${HAL_FEATURES}
${HAL_CUTS}
${HAL_DATA}
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

link_directories( ${LINK_DIRECTORIES})

Set(SRCS
EventAna.cxx
MemoryMapManager.cxx
MultiTrackAna.cxx
TrackAna.cxx
TwoTrackAna.cxx
Reader.cxx
dump/TrackColDumAna.cxx
chains/EventAnaChain.cxx
chains/TrackAnaChain.cxx
chains/TwoTrackAnaChain.cxx
on_the_fly/SplitedTrackToStatusTask.cxx
on_the_fly/BoostTask.cxx
on_the_fly/MiniEventTask.cxx
on_the_fly/CopyDataTask.cxx
on_the_fly/SmearTask.cxx
on_the_fly/SplittingToStatusTask.cxx
on_the_fly/smear_codes/SmearAlgorithm.cxx
on_the_fly/smear_codes/TrackSmearMomentumPercent.cxx
)

Set(HEADERS )
Set(LINKDEF HalAnaLinkDef.h)
Set(LIBRARY_NAME HalAna)
Set(DEPENDENCIES HalCut)

GENERATE_LIBRARY_HAL()


