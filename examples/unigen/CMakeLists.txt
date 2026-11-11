set(SRCS
UEvent.cxx
UnigenEvent.cxx
UnigenEventInterface.cxx
UnigenTrack.cxx
UnigenTrackInterface.cxx
UParticle.cxx
UnigenReader.cxx
)

Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
)

Set(HEADERS )
Set(LINKDEF HalUnigenLinkDef.h)
Set(LIBRARY_NAME HalUnigen)
Set(DEPENDENCIES HalAna )

GENERATE_LIBRARY_HAL()

