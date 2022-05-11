# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
 
${HAL_DATA}
${HAL_FEATURES}
${HAL_ANA}
${HAL_CUTS}
${HAL_CORE}/analysis/fluctuations
)

Set(SYSTEM_INCLUDE_DIRECTORIES
 ${ROOT_INCLUDE_DIR}
 ${BASE_INCLUDE_DIRECTORIES} 
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${GSL_LIBRARY_DIRS} 
  ${FAIRROOT_LIBRARY_DIR}
)
 
Link_Directories(${LINK_DIRECTORIES})

# List of source files
set(SRCS
   ChargedFluctuationsAna.cxx
   Fluctuation1D.cxx
   FluctuationsAna.cxx
)

Set(HEADERS )
Set(LINKDEF HalPhysFluctLinkDef.h)
Set(LIBRARY_NAME Fluct)
Set(DEPENDENCIES Base EG MathCore Physics Core HalAna )

GENERATE_LIBRARY()


