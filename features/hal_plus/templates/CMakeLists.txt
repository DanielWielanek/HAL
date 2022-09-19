# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
    ${Hal_INCLUDE_DIR}
    #put here other included libs
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
)

link_directories( ${LINK_DIRECTORIES})

set(SRCS
__HAL_NAME_OF_CXX_FILES__
)

Set(HEADERS )
Set(LINKDEF __HAL_NAME_OF_LINKDEF__)
Set(LIBRARY_NAME __HAL_NAME_OF_LIBRARY__)
Set(DEPENDENCIES Gui Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread XMLParser XMLIO SQLIO  EG
#add other dependencies
 )
GENERATE_LIBRARY()
