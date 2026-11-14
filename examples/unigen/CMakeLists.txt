Set(INCLUDE_DIRECTORIES
${HAL_FEATURES}
${HAL_DATA}
${HAL_CORE}/analysis/base
${HAL_CORE}/data/dataformat
${HAL_CORE}/data/io
${HAL_CORE}/examples/unigen
)

Include_Directories(${INCLUDE_DIRECTORIES})

set(SRCS
UEvent.cxx
UnigenEvent.cxx
UnigenEventInterface.cxx
UnigenTrack.cxx
UnigenTrackInterface.cxx
UParticle.cxx
UnigenReader.cxx
)

Set(HEADERS )
Set(LINKDEF HalUnigenLinkDef.h)
Set(LIBRARY_NAME HalUnigen)
Set(DEPENDENCIES HalAna )

GENERATE_LIBRARY_HAL()

