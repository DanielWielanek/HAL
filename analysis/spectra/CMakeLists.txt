# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

# List of source files
set(SRCS
   SpectraAna.cxx
)

Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
)

Set(HEADERS )
Set(LINKDEF HalSpectraLinkDef.h)
Set(LIBRARY_NAME HalSpectra)
Set(DEPENDENCIES ROOT::EG ROOT::MathCore ROOT::Physics ROOT::Core HalAna )

GENERATE_LIBRARY_HAL()


