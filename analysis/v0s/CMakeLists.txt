# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
 
${HAL_DATA}
${HAL_FEATURES}
${HAL_ANA}
${HAL_HBT}
${HAL_CUTS}
${HAL_CORE}/analysis/v0s
)

Set(SYSTEM_INCLUDE_DIRECTORIES
 ${ROOT_INCLUDE_DIR}
 ${BASE_INCLUDE_DIRECTORIES} 
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR} 
  ${FAIRROOT_LIBRARY_DIR}
)
 
Link_Directories(${LINK_DIRECTORIES})

# List of source files
set(SRCS
   V0BasicFinder.cxx
   V0CandBasicCut.cxx
   V0Candidate.cxx
   V0CandidateHelix.cxx
)

Set(HEADERS )
Set(LINKDEF HalV0AnaLinkDef.h)
Set(LIBRARY_NAME HalV0)
Set(DEPENDENCIES EG MathCore Physics Core HalAna )

GENERATE_LIBRARY_HAL()


