# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

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
hist/styles/LegendStyle.cxx
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
phys/SmearAlgo.cxx
phys/ParameterSolver.cxx

minimizer/FitParam.cxx
minimizer/Minimizer.cxx
minimizer/MinimizerStepConf.cxx
minimizer/MultiDimDataManager.cxx
minimizer/MultiDimInterpolator.cxx
minimizer/MultiDimFile.cxx
minimizer/MultiDimMinuit.cxx
)
install(FILES ${EXT_HEADERS}
        DESTINATION include
       )

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/hist
  ${CMAKE_CURRENT_SOURCE_DIR}/hist/styles
  ${CMAKE_CURRENT_SOURCE_DIR}/io
  ${CMAKE_CURRENT_SOURCE_DIR}/io/html
  ${CMAKE_CURRENT_SOURCE_DIR}/std
  ${CMAKE_CURRENT_SOURCE_DIR}/minimizer
  ${CMAKE_CURRENT_SOURCE_DIR}/phys
  ${CMAKE_CURRENT_SOURCE_DIR}
)


Set(HEADERS )
Set(LINKDEF HalFeaturesLinkDef.h)
Set(LIBRARY_NAME HalFeatures)
Set(DEPENDENCIES ROOT::Gui ROOT::Core ROOT::RIO ROOT::Net ROOT::Hist
                 ROOT::Graf ROOT::Graf3d ROOT::Gpad ROOT::Tree
                 ROOT::Rint ROOT::Postscript ROOT::Matrix ROOT::Physics
                 ROOT::MathCore ROOT::Thread ROOT::XMLParser ROOT::XMLIO
                 ROOT::SQLIO ROOT::EG GSL::gsl GSL::gslcblas )


string(JOIN " " ROOTCLING_FLAGS "-I" ${HAL_FEATURES})

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
    Set(DEPENDENCIES GSL::gsl GSL::gslcblas HalFeatures ROOT::Tree HalAna HalFemto HalFlow HalSpectra )
    GENERATE_EXECUTABLE()
  EndForEach(_file RANGE 0 ${_length})


