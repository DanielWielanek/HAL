# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES

${HAL_FEATURES}
${HAL_CORE}/features/minimizer
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

link_directories( ${LINK_DIRECTORIES})

set(SRCS
HalCout.cxx
HalArray.cxx
HalOptions.cxx
HalParameter.cxx
HalStd.cxx
HalObjectMatrix.cxx
HalHelix.cxx
HalHelixBase.cxx
HalConst.cxx
HalXMLNode.cxx

io/HalLink.cxx
io/HalJobs.cxx
io/HalObject.cxx
io/HalPackage.cxx
io/HalPackage2HTML.cxx
io/HalAnaFile.cxx
io/HalHtmlCore.cxx
io/HalPackageSql.cxx
io/HalDataManager.cxx
io/html/HalHtmlDiv.cxx
io/html/HalHtmlFile.cxx
io/html/HalHtmlObject.cxx
io/html/HalHtmlTable.cxx
hist/HalDividedHisto.cxx
hist/HalHistogramManager.cxx
hist/HalMultiGraph.cxx
hist/HalSplines.cxx
hist/HalChiSqMap2D.cxx
std/HalStdHist.cxx
std/HalStdString.cxx
std/HalStdMath.cxx
minimizer/HalFitParam.cxx
minimizer/HalMinimizer.cxx
)
install(FILES ${EXT_HEADERS}
        DESTINATION include
       )

Set(HEADERS )
Set(LINKDEF HalFeaturesLinkDef.h)
Set(LIBRARY_NAME HalFeatures)
Set(DEPENDENCIES Gui Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread XMLParser XMLIO SQLIO FairTools Base)
#Set(DEPENDENCIES XMLIO SQLIO FairTools)


GENERATE_LIBRARY()

#copy helpfull files
Install(DIRECTORY hal_plus 
        DESTINATION share/
        PATTERN ".svn" EXCLUDE
       )

#making executables

#[[
Set(Exe_Names
    hal-merger
    hal-report
    hal-jobs)
Set(Exe_Source
    features_apps/hal_merger.cpp
    features_apps/hal_report.cpp
    features_apps/hal_jobs.cpp
    )
  List(LENGTH Exe_Names _length)
  Math(EXPR _length ${_length}-1)
  ForEach(_file RANGE 0 ${_length})
    List(GET Exe_Names ${_file} _name)
    List(GET Exe_Source ${_file} _src)
    Set(EXE_NAME ${_name})
    Set(SRCS ${_src})
    if(NOT DEFINED DISABLE_GSL)
    	Set(DEPENDENCIES gsl gslcblas HalFeatures Tree HalAna HalFemto HalFlow HalSpectra )
    else()
    	Set(DEPENDENCIES HalFeatures Tree HalAna HalFemto HalFlow HalSpectra )
    endif()
    GENERATE_EXECUTABLE()
  EndForEach(_file RANGE 0 ${_length})
]]


