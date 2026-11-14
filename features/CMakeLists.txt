# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES ${HAL_FEATURES})

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
Units.cxx

io/Link.cxx
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
io/schedulers/JobQueue.cxx
io/schedulers/JobSbatch.cxx
io/schedulers/JobTorque.cxx

hist/DividedHisto.cxx
hist/HistogramManager.cxx
hist/MultiGraph.cxx
hist/Splines.cxx
hist/ChiSqMap2D.cxx
hist/FastHist.cxx
hist/CorrelationHisto.cxx
hist/Painter.cxx
hist/HistoPainter.cxx
hist/styles/AxisStyle.cxx
hist/styles/FillStyle.cxx
hist/styles/HistoStyle.cxx
hist/styles/LineStyle.cxx
hist/styles/LegendStyle.cxx
hist/styles/MarkerStyle.cxx
hist/styles/PadStyle.cxx
hist/styles/Style.cxx
hist/styles/TextStyle.cxx

math/ManyPolynomialsX.cxx
math/PeakFitterX.cxx
math/ShMath.cxx
math/ShIndexes.cxx
math/ShGridDecomposer.cxx
math/ShGridDecomposerSimd.cxx
math/SimpleMathFunc.cxx

std/StdHist.cxx
std/StdString.cxx
std/StdMath.cxx
std/StdPhys.cxx
std/StdTypes.cxx
std/StdOpenMP.cxx
std/StdSimd.cxx
std/StdSimdComplex.cxx

phys/ErrorCalc.cxx
phys/Helix.cxx
phys/HelixBase.cxx
phys/FastAxisCalc.cxx
phys/ProfileAna2D.cxx
phys/SmearAlgo.cxx
phys/ParameterSolver.cxx
phys/LorentzSmearing.cxx

minimizer/FitParam.cxx
minimizer/Minimizer.cxx
minimizer/MinimizerResult.cxx
minimizer/MinimizerStepConf.cxx
minimizer/MultiDimDataManager.cxx
minimizer/MultiDimFile.cxx
minimizer/MultiDimInterpolator.cxx
minimizer/MultiDimMinimizer.cxx
minimizer/MultiDimMinuit.cxx
minimizer/MultiDimMinimizerSquare.cxx

)
install(FILES ${EXT_HEADERS} DESTINATION include )

Set(LINKDEF HalFeaturesLinkDef.h)
Set(LIBRARY_NAME HalFeatures)
Set(DEPENDENCIES ROOT::Gui ROOT::Core ROOT::RIO ROOT::Net ROOT::Hist ROOT::Graf ROOT::Graf3d ROOT::Gpad 
  ROOT::Tree ROOT::Rint ROOT::Postscript ROOT::Matrix ROOT::Physics ROOT::MathCore 
  ROOT::Thread ROOT::XMLParser ROOT::XMLIO ROOT::SQLIO ROOT::EG GSL::gsl GSL::gslcblas )


if(WARNING_FLAGS)
    add_compile_options( ${MY_WARNINGS})
endif()


string(JOIN " " ROOTCLING_FLAGS "-I" ${HAL_FEATURES})

GENERATE_LIBRARY_HAL()

#copy helpfull files
Install(DIRECTORY hal_plus 
        DESTINATION share/
        PATTERN ".svn" EXCLUDE
       )

#making executables

if(OpenMP_CXX_FOUND)
   list(APPEND DEPENDENCIES ${GSL_LIBRARIES} HalFeatures ROOT::Tree HalAna HalFemto HalPhysFlow HalSpectra OpenMP::OpenMP_CXX)
else()
   list(APPEND DEPENDENCIES ${GSL_LIBRARIES} HalFeatures ROOT::Tree HalAna HalFemto HalPhysFlow HalSpectra)
endif()

GENERATE_EXECUTABLE(hal-merger features_apps/hal_merger.cpp "${DEPENDENCIES}")
GENERATE_EXECUTABLE(hal-report features_apps/hal_report.cpp "${DEPENDENCIES}")
GENERATE_EXECUTABLE(hal-jobs features_apps/hal_jobs.cpp "${DEPENDENCIES}")
GENERATE_EXECUTABLE(hal-cmake features_apps/hal_cmake.cpp "${DEPENDENCIES}")
