# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

# List of source files
set(SRCS
   ChargedFluctuationsAna.cxx
   Fluctuation1D.cxx
   FluctuationsAna.cxx
   MultiplicityAna.cxx
)

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
)

Set(HEADERS )
Set(LINKDEF HalPhysFluctLinkDef.h)
Set(LIBRARY_NAME HalFluct)
Set(DEPENDENCIES ROOT::EG ROOT::MathCore ROOT::Physics ROOT::Core HalAna )

GENERATE_LIBRARY_HAL()


