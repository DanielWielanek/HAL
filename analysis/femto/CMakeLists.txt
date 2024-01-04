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

Set(SYSTEM_INCLUDE_DIRECTORIES
 ${ROOT_INCLUDE_DIR} 
 ${BASE_INCLUDE_DIRECTORIES}
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
ana/FemtoFreezoutsAna.cxx
ana/FemtoDumpPairAna.cxx
ana/FemtoEventBinsAna.cxx

base/FemtoDPhiDEta.cxx
base/Femto1DCF.cxx
base/Femto3DCF.cxx
base/FemtoDebug2DCF.cxx
base/Femto3DCFQinv.cxx
base/FemtoFastCut.cxx
base/FemtoFreezoutGenerator.cxx
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


corrfit/CorrFitDrawOptions.cxx
corrfit/CorrFit.cxx
corrfit/CorrFitMask.cxx

corrfit/CorrFitFunc.cxx
corrfit/CorrFitMath1DCF.cxx
corrfit/CorrFitMath3DCF.cxx
corrfit/CorrFit1DCF.cxx
corrfit/CorrFit3DCF.cxx
corrfit/CorrFitKisiel.cxx
corrfit/CorrFitWielanek.cxx
corrfit/CorrFitGUI.cxx
corrfit/CorrFitHDFunc.cxx
corrfit/CorrFitSmear1DCF.cxx
corrfit/CorrFitMapKstarRstar.cxx
corrfit/mapgenerators/Femto1DCFAnaMap.cxx
corrfit/roco/Femto1DCFAnaMapMCRoco.cxx
corrfit/mapgenerators/Femto1DMapGenerator.cxx
corrfit/mapgenerators/Femto1DCFAnaMapMC.cxx
corrfit/mapgenerators/FemtoWielanekMapGenerator.cxx
corrfit/mapgenerators/Femto1DCFAnaMapPairsDumped.cxx
corrfit/mapgenerator_multidim/CorrFitDumpedPairAna.cxx
corrfit/mapgenerator_multidim/CorrFitDumpedPairAnaHorizontal.cxx
corrfit/mapgenerator_multidim/CorrFitFunctor.cxx
corrfit/mapgenerator_multidim/CorrFitInfo.cxx
corrfit/mapgenerator_multidim/CorrFitMapRebin.cxx
corrfit/mapgenerator_multidim/CorrFitPairGeneratorYPt.cxx
corrfit/mapgenerator_multidim/CorrFitParamsSetup.cxx
corrfit/mapgenerator_multidim/CorrFitVerticalSlices.cxx
corrfit/mapgenerator_multidim/CorrFit3DCFMultiDim.cxx
corrfit/mapgenerator_multidim/FemtoDumpPairAnaMC.cxx
corrfit/CorrFitConst.cxx

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




weights/FemtoFsiParsed.cxx
weights/FemtoWeightGeneratorLednicky2.cxx
weights/FemtoWeightGenerator.cxx
weights/FemtoWeightGeneratorBasic.cxx
weights/FemtoWeightGeneratorLednicky.cxx
weights/FemtoWeightGeneratorResidual.cxx
weights/FemtoWeightGeneratorSupressed.cxx
weights/FemtoWeightGeneratorKisiel.cxx

base/FemtoCorrFunc.cxx
base/corrfunc/FemtoCorrFuncPt.cxx
base/corrfunc/FemtoCorrFuncKt.cxx
base/corrfunc/FemtoCorrFuncAzimuthalKt.cxx
base/corrfunc/FemtoCorrFuncSimple.cxx
base/corrfunc/FemtoCorrFunc0D.cxx
base/corrfunc/FemtoCorrFunc1D.cxx
base/corrfunc/FemtoCorrFunc2D.cxx

imaging/FemtoImaging.cxx
imaging/FemtoImaging1D.cxx
imaging/FemtoImaging3D.cxx
base/FemtoSHCF.cxx


)


if(DEFINED CIA)
    Set(INCLUDE_DIRECTORIES
        ${INCLUDE_DIRECTORIES} top_secret
    )
    set(SRCS ${SRCS}
        top_secret/CorrFitSHCF.cxx
        top_secret/CorrFitSHFrom3D.cxx
        top_secret/FemtoSHArray.cxx
        top_secret/CorrFitSHCFMultiDim.cxx
        top_secret/CorrFitMaskSH.cxx
        top_secret/CorrFitHDFuncSH.cxx
        top_secret/CorrFitDumpedPairAnaVertical.cxx
        top_secret/BattlestarGalactica.cxx
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

if(NOT DEFINED DISABLE_GSL)
	Set(DEPENDENCIES ${DEPENDENCIES} gsl gslcblas)
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

