# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
Set(SRCS
EventAna.cxx
MemoryMapManager.cxx
MultiTrackAna.cxx
TrackAna.cxx
TwoTrackAna.cxx
dump/TrackColDumAna.cxx
chains/EventAnaChain.cxx
chains/TrackAnaChain.cxx
chains/TwoTrackAnaChain.cxx
on_the_fly/SplitedTrackToStatusTask.cxx
on_the_fly/BoostTask.cxx
on_the_fly/EventRotateTask.cxx
on_the_fly/MiniEventTask.cxx
on_the_fly/CopyDataTask.cxx
on_the_fly/SmearTask.cxx
on_the_fly/SplittingToStatusTask.cxx
on_the_fly/smear_codes/SmearAlgorithm.cxx
on_the_fly/smear_codes/TrackSmearMomentumPercent.cxx
)

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/dump
  ${CMAKE_CURRENT_SOURCE_DIR}/chains
  ${CMAKE_CURRENT_SOURCE_DIR}/on_the_fly
  ${CMAKE_CURRENT_SOURCE_DIR}/on_the_fly/smear_codes
  ${CMAKE_CURRENT_SOURCE_DIR}
)

Set(HEADERS )
Set(LINKDEF HalAnaLinkDef.h)
Set(LIBRARY_NAME HalAna)
Set(DEPENDENCIES HalCut)

GENERATE_LIBRARY_HAL()


