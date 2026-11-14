# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${HAL_PHYS}
  ${HAL_CORE}/analysis/fluctuations
)

# List of source files
set(SRCS
   ChargedFluctuationsAna.cxx
   Fluctuation1D.cxx
   FluctuationsAna.cxx
   MultiplicityAna.cxx
)

Set(LINKDEF HalPhysFluctLinkDef.h)
Set(LIBRARY_NAME HalPhysFluct)
Set(DEPENDENCIES ROOT::EG ROOT::MathCore ROOT::Physics ROOT::Core HalAna )

GENERATE_LIBRARY_HAL()