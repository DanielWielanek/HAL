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

ana/FemtoBasicAna.cxx
ana/FemtoFreezeoutsAna.cxx

ana/FemtoEventBinsAna.cxx

base/FemtoFastCut.cxx
base/FemtoFreezeoutGenerator.cxx
base/FemtoPairKinematics.cxx
base/FemtoPair.cxx
base/FemtoMiniPair.cxx
base/FemtoSourceModel.cxx
base/FemtoConst.cxx
base/source_models/FemtoSourceModelExp.cxx
base/source_models/FemtoSourceModelGauss.cxx
base/source_models/FemtoSourceModelGausMt.cxx
base/source_models/FemtoSourceModelNumerical1D.cxx
base/source_models/FemtoSourceDensity.cxx
base/source_models/FemtoSourceModelNumericalPseudo3d.cxx
base/shmath/FemtoSHSlice.cxx
base/shmath/FemtoYlmIndexes.cxx
base/shmath/FemtoYlmMath.cxx
base/shmath/FemtoYlmSolver.cxx

corrfit/base/CorrFit.cxx
corrfit/base/CorrFitMask.cxx

corrfit/base/CorrFitFunc.cxx
corrfit/base/CorrFitFunc3D.cxx
corrfit/base/CorrFitMath1DCF.cxx
corrfit/base/CorrFitMath3DCF.cxx
corrfit/base/CorrFit1DCF.cxx
corrfit/base/CorrFit3DCF.cxx
corrfit/base/CorrFitKisiel.cxx
corrfit/base/CorrFitWielanek.cxx
corrfit/base/CorrFitGUI.cxx
corrfit/base/CorrFitHDFunc.cxx
corrfit/base/CorrFitSmear1DCF.cxx
corrfit/maps_1d/CorrFitMapKstarRstar.cxx
corrfit/maps_1d/CorrFitMapKstarRstarSmearingCorrection.cxx
corrfit/maps_1d/CorrFitSmearingMatrixCalculator.cxx
corrfit/base/CorrFitConst.cxx
corrfit/mapgenerators/Femto1DCFAnaMap.cxx
corrfit/roco/Femto1DCFAnaMapMCRoco.cxx
corrfit/mapgenerators/Femto1DMapGenerator.cxx
corrfit/mapgenerators/Femto1DCFAnaMapMC.cxx
corrfit/mapgenerators/FemtoWielanekMapGenerator.cxx
corrfit/mapgenerators/Femto1DCFAnaMapPairsDumped.cxx
corrfit/pairgenerators/CorrFitMapGroupConfig.cxx
corrfit/pairgenerators/CorrFitPairGenerator.cxx
corrfit/pairgenerators/CorrFitPairGeneratorSimple.cxx
corrfit/pairgenerators/CorrFitPairGeneratorYPtKt.cxx
corrfit/pairgenerators/CorrFitPairGeneratorYPtKtStubborn.cxx
corrfit/pairgenerators/CorrFitPairGeneratorConvolutionYPt.cxx
corrfit/pairgenerators/CorrFitPairFile.cxx
corrfit/pairgenerators/ConvolutionPairGenerator.cxx
corrfit/pairgenerators/FemtoDumpPairAna.cxx
corrfit/mapgenerator_multidim/CorrFitDumpedPairAna.cxx
corrfit/mapgenerator_multidim/CorrFitDumpedPairAnaHorizontal.cxx
corrfit/mapgenerator_multidim/CorrFitFunctor.cxx
corrfit/mapgenerator_multidim/CorrFitInfo.cxx
corrfit/mapgenerator_multidim/CorrFitMapRebin.cxx
corrfit/mapgenerator_multidim/CorrFitParamsSetup.cxx
corrfit/mapgenerator_multidim/CorrFitVerticalSlices.cxx
corrfit/mapgenerator_multidim/CorrFit3DCFMultiDim.cxx

corrfit/fittingfunctions/CorrFit1DCFSquare.cxx
corrfit/fittingfunctions/CorrFit1DCFLorentz.cxx
corrfit/fittingfunctions/CorrFit3DCFSquare.cxx
corrfit/fittingfunctions/CorrFit3DCFLorentz.cxx
corrfit/fittingfunctions/CorrFit1DCFExp.cxx
corrfit/fittingfunctions/CorrFit1DCFGauss.cxx
corrfit/fittingfunctions/CorrFit1DCFPoly.cxx
corrfit/fittingfunctions/CorrFit3DCFGauss.cxx
corrfit/fittingfunctions/CorrFit3DCFGauss2.cxx
corrfit/fittingfunctions/CorrFit3DCFGaussCross.cxx
corrfit/fittingfunctions/CorrFit1DCFCumac.cxx
corrfit/fittingfunctions/CorrFit3DCFBowlerSinyukov.cxx
corrfit/fittingfunctions/CorrFit3DCFBowlerSinyukovExtra.cxx

corrfit/corrfitopt/CorrFitHDFunc1D.cxx
corrfit/corrfitopt/CorrFitHDFunc3D.cxx

corrfit/corrfitopt/CorrFitMask1D.cxx
corrfit/corrfitopt/CorrFitMask3D.cxx

corrfit/serialization/FemtoSerializationInterface.cxx
corrfit/serialization/FemtoSerializationInterface1D.cxx
corrfit/serialization/FemtoSerializationInterface3D.cxx
corrfit/serialization/FemtoSerializationInterfaceSH.cxx


weights/FemtoFsiParsed.cxx
weights/FemtoWeightGeneratorLednicky2.cxx
weights/FemtoWeightGenerator.cxx
weights/FemtoWeightGeneratorBasic.cxx
weights/FemtoWeightGeneratorLednicky.cxx
weights/FemtoWeightGeneratorResidual.cxx
weights/FemtoWeightGeneratorSupressed.cxx
weights/FemtoWeightGeneratorKisiel.cxx
weights/FemtoWeightGeneratorDummy.cxx

base/funcs/FemtoSHCF.cxx
base/funcs/FemtoDPhiDEta.cxx
base/funcs/Femto1DCF.cxx
base/funcs/Femto3DCF.cxx
base/funcs/FemtoDebug2DCF.cxx
base/funcs/Femto3DCFQinv.cxx

base/FemtoCorrFunc.cxx
base/corrfunc/FemtoCorrFuncPt.cxx
base/corrfunc/FemtoCorrFuncKt.cxx
base/corrfunc/FemtoCorrFuncAzimuthalKt.cxx
base/corrfunc/FemtoCorrFuncSimple.cxx
base/corrfunc/FemtoCorrFunc0D.cxx
base/corrfunc/FemtoCorrFunc1D.cxx
base/corrfunc/FemtoCorrFunc2D.cxx

painters/FemtoCFPainter.cxx
painters/Femto1DCFPainter.cxx
painters/Femto3DCFPainter.cxx
painters/FemtoSHCFPainter.cxx


corrfit/imaging/FemtoImaging.cxx
corrfit/imaging/FemtoImaging1D.cxx
corrfit/imaging/FemtoImaging3D.cxx

corrfit/painters/CorrFitPainter.cxx
corrfit/painters/CorrFit1DCFPainter.cxx
corrfit/painters/CorrFit3DCFPainter.cxx

)


if(DEFINED CIA)
    Set(INCLUDE_DIRECTORIES
        ${INCLUDE_DIRECTORIES} top_secret
    )
    set(SRCS ${SRCS}
        corrfit/top_secret/CorrFitSHCF.cxx
        corrfit/top_secret/CorrFitSHFrom3D.cxx
        corrfit/top_secret/FemtoSHArray.cxx
        corrfit/top_secret/CorrFitSHCFMultiDim.cxx
        corrfit/top_secret/CorrFitMaskSH.cxx
        corrfit/top_secret/CorrFitHDFuncSH.cxx
        corrfit/top_secret/CorrFitDumpedPairAnaVertical.cxx
        corrfit/top_secret/BattlestarGalactica.cxx
        corrfit/top_secret/BattlestarGalactica10.cxx
        corrfit/top_secret/BattlestarPegasus.cxx
        corrfit/top_secret/BattlestarValkyria.cxx
        corrfit/top_secret/CorrFitSHCFPainter.cxx
        corrfit/top_secret/BattlestarAtlantia.cxx
        corrfit/top_secret/BattlestarSolaria.cxx
        corrfit/top_secret/GausianSourceDenstityFastCalc.cxx
    )
endif()

set(NO_DICT_SRCS
weights/FemtoFsiTools.F
weights/FemtoFsiWeightLednicky.F
)

set(EXT_HEADERS
base/FemtoConst.h
)

Set(HEADERS)
if(DEFINED CIA)
    Set(LINKDEF HalFemtoLinkDef2.h)
else()
    Set(LINKDEF HalFemtoLinkDef.h)
endif()

Set(LIBRARY_NAME HalFemto)


Set(DEPENDENCIES HalAna Minuit Minuit2 Fumili)
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
    corrfit/mapgenerator_multidim/hal_corrfit.cpp
    )
  List(LENGTH Exe_Names _length)
  Math(EXPR _length ${_length}-1)
  ForEach(_file RANGE 0 ${_length})
    List(GET Exe_Names ${_file} _name)
    List(GET Exe_Source ${_file} _src)
    Set(EXE_NAME ${_name})
    Set(SRCS ${_src})
    Set(DEPENDENCIES gsl gslcblas HalData HalFeatures Tree EG HalAna HalFemto)
    GENERATE_EXECUTABLE()
  EndForEach(_file RANGE 0 ${_length})

