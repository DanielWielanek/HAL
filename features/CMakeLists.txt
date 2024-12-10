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
Cout.cxx
Array.cxx
Options.cxx
Parameter.cxx
Std.cxx
ObjectMatrix.cxx
Const.cxx
XMLNode.cxx
CompressionMap.cxx
UniqueOptions.cxx


io/Link.cxx
io/Jobs.cxx
io/Object.cxx
io/Package.cxx
io/AnaFile.cxx
io/Pointer.cxx

io/html/PackageSql.cxx
io/html/Package2HTML.cxx
io/html/HtmlCore.cxx
io/html/HtmlDiv.cxx
io/html/HtmlFile.cxx
io/html/HtmlObject.cxx
io/html/HtmlTable.cxx
io/html/PackageTable.cxx
io/html/PicTree.cxx

hist/DividedHisto.cxx
hist/HistogramManager.cxx
hist/MultiGraph.cxx
hist/Splines.cxx
hist/ChiSqMap2D.cxx
hist/FastHist.cxx
hist/CorrelationHisto.cxx
hist/Painter.cxx
hist/styles/AxisStyle.cxx
hist/styles/FillStyle.cxx
hist/styles/HistoStyle.cxx
hist/styles/LineStyle.cxx
hist/styles/MarkerStyle.cxx
hist/styles/PadStyle.cxx
hist/styles/Style.cxx

std/StdHist.cxx
std/StdString.cxx
std/StdMath.cxx

phys/ErrorCalc.cxx
phys/Helix.cxx
phys/HelixBase.cxx
phys/FastAxisCalc.cxx
phys/ProfileAna2D.cxx

minimizer/FitParam.cxx
minimizer/Minimizer.cxx
minimizer/MinimizerStepConf.cxx
)
install(FILES ${EXT_HEADERS}
        DESTINATION include
       )

Set(HEADERS )
Set(LINKDEF HalFeaturesLinkDef.h)
Set(LIBRARY_NAME HalFeatures)
Set(DEPENDENCIES Gui Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread XMLParser XMLIO SQLIO  EG )
#Set(DEPENDENCIES XMLIO SQLIO FairTools)


GENERATE_LIBRARY_HAL()

#copy helpfull files
Install(DIRECTORY hal_plus 
        DESTINATION share/
        PATTERN ".svn" EXCLUDE
       )

#making executables


Set(Exe_Names
    hal-merger
    hal-report
    hal-jobs
    hal-cmake)
Set(Exe_Source
    features_apps/hal_merger.cpp
    features_apps/hal_report.cpp
    features_apps/hal_jobs.cpp
    features_apps/hal_cmake.cpp
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



