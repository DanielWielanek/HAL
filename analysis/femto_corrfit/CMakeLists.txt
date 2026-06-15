# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
#add_subdirectory(Weights)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-uninitialized -Wno-strict-aliasing")

Set(INCLUDE_DIRECTORIES
${HAL_DATA}
${HAL_FEATURES}
${HAL_ANA}
${HAL_HBT}
${HAL_CUTS}
${HAL_HBT_FIT}
)

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${GSL_LIBRARY_DIRS} 
)

Include_Directories(
 ${INCLUDE_DIRECTORIES}
 ${GSL_INCLUDE_DIRS}
)
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
Include_Directories(${GSL_DIR}/include)


Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${GSL_LIBRARY_DIRS} 
  ${FAIRROOT_LIBRARY_DIR}
)
 
Link_Directories(${LINK_DIRECTORIES})

set(SRCS


base/CorrFit.cxx
base/CorrFitMask.cxx

base/CorrFitFunc.cxx
base/CorrFitFunc3D.cxx
base/CorrFitMath1DCF.cxx
base/CorrFitMath3DCF.cxx
base/CorrFit1DCF.cxx
base/CorrFit3DCF.cxx
base/CorrFitKisiel.cxx
base/CorrFitWielanek.cxx
base/CorrFitGUI.cxx
base/CorrFitHDFunc.cxx
base/CorrFitSmear1DCF.cxx
base/CorrFitConst.cxx

maps_1d/CorrFitMapKstarRstar.cxx
maps_1d/CorrFit1DSmearing.cxx
maps_1d/CorrFitSmearingMath.cxx
maps_1d/Femto1DCFAnaMapMCRoco.cxx

pairgenerators/CorrFitMapGroupConfig.cxx
pairgenerators/CorrFitPairGenerator.cxx
pairgenerators/CorrFitPairGeneratorSimple.cxx
pairgenerators/CorrFitPairGeneratorYPtKt.cxx
pairgenerators/CorrFitPairGeneratorYPtKtStubborn.cxx
pairgenerators/CorrFitPairGeneratorConvolutionYPt.cxx
pairgenerators/CorrFitPairFile.cxx
pairgenerators/ConvolutionPairGenerator.cxx
pairgenerators/FemtoDumpPairAna.cxx

mapgenerators/Femto1DCFAnaMap.cxx
mapgenerators/Femto1DMapGenerator.cxx
mapgenerators/Femto1DCFAnaMapMC.cxx
mapgenerators/FemtoWielanekMapGenerator.cxx
mapgenerators/Femto1DCFAnaMapPairsDumped.cxx
mapgenerator_multidim/CorrFitDumpedPairAna.cxx
mapgenerator_multidim/CorrFitDumpedPairAnaHorizontal.cxx
mapgenerator_multidim/CorrFitFunctor.cxx
mapgenerator_multidim/CorrFitInfo.cxx
mapgenerator_multidim/CorrFitMapRebin.cxx
mapgenerator_multidim/CorrFitParamsSetup.cxx
mapgenerator_multidim/CorrFit3DCFMultiDim.cxx

fittingfunctions/CorrFit1DCFSquare.cxx
fittingfunctions/CorrFit1DCFLorentz.cxx
fittingfunctions/CorrFit3DCFSquare.cxx
fittingfunctions/CorrFit3DCFLorentz.cxx
fittingfunctions/CorrFit1DCFExp.cxx
fittingfunctions/CorrFit1DCFGauss.cxx
fittingfunctions/CorrFit1DCFPoly.cxx
fittingfunctions/CorrFit3DCFGauss.cxx
fittingfunctions/CorrFit3DCFGauss2.cxx
fittingfunctions/CorrFit3DCFGaussCross.cxx
fittingfunctions/CorrFit1DCFCumac.cxx
fittingfunctions/CorrFit3DCFBowlerSinyukov.cxx
fittingfunctions/CorrFit3DCFBowlerSinyukovExtra.cxx

corrfitopt/CorrFitHDFunc1D.cxx
corrfitopt/CorrFitHDFunc3D.cxx

corrfitopt/CorrFitMask1D.cxx
corrfitopt/CorrFitMask3D.cxx

imaging/FemtoImaging.cxx
imaging/FemtoImaging1D.cxx
imaging/FemtoImaging3D.cxx

painters/CorrFitPainter.cxx
painters/CorrFit1DCFPainter.cxx
painters/CorrFit3DCFPainter.cxx

)


if(DEFINED CIA)
    Set(INCLUDE_DIRECTORIES
        ${INCLUDE_DIRECTORIES} top_secret
    )
    set(SRCS ${SRCS}
        top_secret/CorrFitSHCFOTF.cxx
        top_secret/CorrFitSHCF.cxx
        top_secret/CorrFitSHFrom3D.cxx
        top_secret/FemtoSHArray.cxx
        top_secret/CorrFitSHCFMultiDim.cxx
        top_secret/CorrFitMaskSH.cxx
        top_secret/CorrFitHDFuncSH.cxx
        top_secret/CorrFitDumpedPairAnaVertical.cxx
        top_secret/BattlestarGalactica.cxx
       # top_secret/BattlestarGalactica10.cxx
        top_secret/BattlestarPegasus.cxx
        top_secret/BattlestarValkyria.cxx
        top_secret/CorrFitSHCFPainter.cxx
        top_secret/BattlestarAtlantia.cxx
        top_secret/BattlestarSolaria.cxx
        top_secret/GausianSourceDenstityFastCalc.cxx
        top_secret/FemtoSHCFDecomposer.cxx
        top_secret/BattlestarAcropolis.cxx
        top_secret/BattlestarHelper.cxx
    )
endif()

Set(HEADERS)
if(DEFINED CIA)
    Set(LINKDEF HalFemtoCorrFitLinkDef2.h)
else()
    Set(LINKDEF HalFemtoCorrFitLinkDef.h)
endif()

Set(LIBRARY_NAME HalFemtoCorrFit)


Set(DEPENDENCIES HalFemto HalAna Minuit Minuit2 Fumili)
if(NOT DEFINED DISABLE_SQLITE)
   # Set(DEPENDENCIES ${DEPENDENCIES} sqlite3)
endif()

if (NOT DEFINED DISABLE_SQLITE)
	Set(DEPENDENCIES ${DEPENDENCIES} SQLIO)
endif()


if(INCLUDE_HAL_SUBIDIR)
install(FILES ${EXT_HEADERS}
        DESTINATION include/Hal
       )
else()
install(FILES ${EXT_HEADERS}
        DESTINATION include
       )
endif()


GENERATE_LIBRARY_HAL()


#making executables


Set(Exe_Names
    hal-corrfit)
Set(Exe_Source
    mapgenerator_multidim/hal_corrfit.cpp
    )
  List(LENGTH Exe_Names _length)
  Math(EXPR _length ${_length}-1)
  ForEach(_file RANGE 0 ${_length})
    List(GET Exe_Names ${_file} _name)
    List(GET Exe_Source ${_file} _src)
    Set(EXE_NAME ${_name})
    Set(SRCS ${_src})
    Set(DEPENDENCIES gsl gslcblas HalData HalFeatures Tree EG HalAna HalFemto HalFemtoCorrFit)
    GENERATE_EXECUTABLE()
  EndForEach(_file RANGE 0 ${_length})

