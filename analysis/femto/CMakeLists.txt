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

base/funcs_serialization/FemtoSerializationInterface.cxx
base/funcs_serialization/FemtoSerializationInterface1D.cxx
base/funcs_serialization/FemtoSerializationInterface3D.cxx
base/funcs_serialization/FemtoSerializationInterfaceSH.cxx
base/funcs_serialization/CorrFitVerticalSlices.cxx

weights/FemtoFsiParsed.cxx
weights/FemtoWeightGeneratorLednicky2.cxx
weights/FemtoWeightGenerator.cxx
weights/FemtoWeightGeneratorBasic.cxx
weights/FemtoWeightGeneratorLednicky.cxx
weights/FemtoWeightGeneratorResidual.cxx
weights/FemtoWeightGeneratorSupressed.cxx
weights/FemtoWeightGeneratorKisiel.cxx
weights/FemtoWeightGeneratorDummy.cxx
weights/FemtoWeightGeneratorTF.cxx

base/funcs/FemtoSHCF.cxx
base/funcs/FemtoDPhiDEta.cxx
base/funcs/Femto1DCF.cxx
base/funcs/Femto3DCF.cxx
base/funcs/FemtoDebug2DCF.cxx
base/funcs/Femto3DCFQinv.cxx
base/funcs/CorrFitBaseFunc.cxx

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

)


set(NO_DICT_SRCS
weights/FemtoFsiTools.F
weights/FemtoFsiWeightLednicky.F
)

set(EXT_HEADERS
base/FemtoConst.h
)

Set(LINKDEF HalFemtoLinkDef.h)


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

