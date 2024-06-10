Set(INCLUDE_DIRECTORIES
${HAL_FEATURES}
${HAL_DATA}
${HAL_CORE}/analysis/base
${HAL_CORE}/data/dataformat
${HAL_CORE}/data/io
${HAL_CORE}/examples/unigen
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

