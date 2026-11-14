# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${HAL_PHYS}
  ${HAL_CORE}/analysis/flow
)

# List of source files
set(SRCS
   FlowPtVariable.cxx
   FlowVariable.cxx
   FlowYVariable.cxx
   SimpleVnAnalysis.cxx
)

Set(LIBRARY_NAME HalPhysFlow)
Set(DEPENDENCIES ROOT::EG ROOT::MathCore ROOT::Physics ROOT::Core HalAna )

GENERATE_LIBRARY_HAL()


