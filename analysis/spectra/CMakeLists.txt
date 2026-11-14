# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${HAL_PHYS}
  ${HAL_CORE}/analysis/spectra
)

# List of source files
set(SRCS
   SpectraAna.cxx
)

Set(LIBRARY_NAME HalSpectra)
Set(DEPENDENCIES ROOT::EG ROOT::MathCore ROOT::Physics ROOT::Core HalAna )

GENERATE_LIBRARY_HAL()


