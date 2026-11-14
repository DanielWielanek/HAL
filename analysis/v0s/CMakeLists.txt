# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${HAL_PHYS}
  ${HAL_CORE}/analysis/v0s
)

# List of source files
set(SRCS
   V0BasicFinder.cxx
   V0CandBasicCut.cxx
   V0Candidate.cxx
   V0CandidateHelix.cxx
)

Set(LIBRARY_NAME HalV0Ana)
Set(DEPENDENCIES ROOT::EG ROOT::MathCore ROOT::Physics ROOT::Core HalAna )

GENERATE_LIBRARY_HAL()


