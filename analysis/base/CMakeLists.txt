# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
set(INCLUDE_DIRECTORIES
${HAL_ANA}
${HAL_FEATURES}
${HAL_CUTS}
${HAL_DATA}
)

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

Set(LIBRARY_NAME HalAna)
Set(DEPENDENCIES HalCut)

GENERATE_LIBRARY_HAL()


