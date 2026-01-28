#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace Hal::Femto;
#pragma link C++ enum Hal::EFemtoPairType;
#pragma link C++ enum Hal::EFitExtraMask;
#pragma link C++ enum Hal::Femto::EKinematics;
#pragma link C++ class Hal::Femto::CorrFitGammaCalc + ;

#pragma link C++ class Hal::FemtoPair + ;
#pragma link C++ class Hal::FemtoMiniPair + ;
#pragma link C++ class Hal::FemtoMicroPair + ;
#pragma link C++ class Hal::FemtoPicoPair + ;
#pragma link C++ class Hal::FemtoPairLCMS + ;
#pragma link C++ class Hal::FemtoPairPRF + ;
#pragma link C++ class Hal::FemtoPairPRFL + ;
#pragma link C++ class Hal::FemtoPair_SH + ;
#pragma link C++ class Hal::FemtoPairPRF_SH + ;
#pragma link C++ class Hal::FemtoPairLCMS_SH + ;
#pragma link C++ class Hal::FemtoPairDPhiDEta + ;
#pragma link C++ class Hal::FemtoBasicAna + ;
#pragma link C++ class Hal::FemtoFastCut + ;
#pragma link C++ class Hal::FemtoFastCutVirtual + ;
#pragma link C++ class Hal::FemtoFastCutPhi + ;
#pragma link C++ class Hal::FemtoFastCutKt + ;
#pragma link C++ class Hal::FemtoFastCutPtSum + ;
#pragma link C++ class Hal::FemtoSourceModel + ;
#pragma link C++ class Hal::FemtoSourceModel1D + ;
#pragma link C++ class Hal::FemtoSourceModel3D + ;
#pragma link C++ class Hal::FemtoSourceModelGauss1D + ;
#pragma link C++ class Hal::FemtoSourceModelExp1D + ;
#pragma link C++ class Hal::FemtoSourceModelGauss3D + ;
#pragma link C++ class Hal::FemtoSourceModelGausMt + ;
#pragma link C++ class Hal::FemtoSourceModelNumerical1D + ;
#pragma link C++ class Hal::FemtoSourceModelNumericalPseudo3d + ;
#pragma link C++ class Hal::FemtoSourceDensityNumericalPseudo3d + ;
#pragma link C++ class Hal::FemtoSourceModelGauss3DCross + ;
#pragma link C++ class Hal::FemtoFreezeoutGenerator + ;
#pragma link C++ class Hal::FemtoFreezeoutGeneratorLCMS + ;
#pragma link C++ class Hal::FemtoFreezeoutGeneratorPRF + ;
#pragma link C++ class Hal::FemtoFreezeoutsAna + ;
#pragma link C++ enum Hal::FemtoFreezeoutsAna::EMode;
#pragma link C++ class Hal::FemtoEventBinsAna + ;
#pragma link C++ class Hal::FemtoFastCutEta + ;
#pragma link C++ class Hal::FemtoDumpPairAna + ;
#pragma link C++ class Hal::FemtoSourceDensity + ;
#pragma link C++ class Hal::FemtoSourceDensityGaus1d + ;
#pragma link C++ class Hal::FemtoSourceDensityGaus3d + ;
#pragma link C++ class Hal::FemtoSourceDensityNumerical1D + ;
#pragma link C++ class Hal::CorrFitBaseFunc + ;


#pragma link C++ class Hal::Femto1DCF + ;
#pragma link C++ class Hal::Femto3DCF + ;
#pragma link C++ class Hal::Femto3DCFQinv + ;
#pragma link C++ class Hal::FemtoDPhiDEta + ;
#pragma link C++ class Hal::FemtoDebug2DCF + ;

#pragma link C++ class Hal::FemtoCorrFunc + ;
#pragma link C++ class Hal::FemtoCorrFunc0D + ;
#pragma link C++ class Hal::FemtoCorrFunc1D + ;
#pragma link C++ class Hal::FemtoCorrFunc2D + ;
#pragma link C++ class Hal::FemtoCorrFuncPt + ;
#pragma link C++ class Hal::FemtoCorrFuncKt + ;
#pragma link C++ class Hal::FemtoCorrFuncAzimuthalKt + ;
#pragma link C++ class Hal::FemtoCorrFuncSimple + ;


#pragma link C++ class Hal::FemtoWeightGenerator + ;
#pragma link C++ class Hal::FemtoWeightGeneratorBasic + ;
#pragma link C++ class Hal::FemtoFsiParsed + ;
#pragma link C++ class Hal::FemtoWeightGeneratorLednicky + ;
#pragma link C++ class Hal::FemtoWeightGeneratorLednicky2 + ;
#pragma link C++ class Hal::FemtoWeightGeneratorResidual + ;
#pragma link C++ class Hal::FemtoWeightGeneratorSupressed + ;
#pragma link C++ class Hal::FemtoWeightGeneratorKisiel + ;
#pragma link C++ class Hal::FemtoWeightGeneratorK0K0 + ;

#pragma link C++ class Hal::CorrFitVerticalSlices + ;
#pragma link C++ class Hal::CorrFitVerticalSlices1D + ;
#pragma link C++ class Hal::CorrFitVerticalSlices3D + ;
#pragma link C++ class Hal::CorrFitVerticalSlicesSH + ;

#pragma link C++ class Hal::FemtoCFPainter + ;
#pragma link C++ class Hal::Femto1DCFPainter + ;
#pragma link C++ class Hal::Femto3DCFPainter + ;
#pragma link C++ class Hal::FemtoSHCFPainter + ;


#ifndef GSL_DISABLE


#pragma link C++ class Hal::FemtoYlmIndexes + ;
#pragma link C++ class Hal::FemtoSHCF + ;


#pragma link C++ class Hal::FemtoSHSlice + ;
#pragma link C++ class Hal::FemtoYlmMath + ;
#pragma link C++ class Hal::FemtoYlmSolver + ;

#pragma link C++ class Hal::FemtoSerializationInterface + ;
#pragma link C++ class Hal::FemtoSerializationInterface1D + ;
#pragma link C++ class Hal::FemtoSerializationInterface3D + ;
#pragma link C++ class Hal::FemtoSerializationInterfaceSH + ;
#pragma link C++ class Hal::FemtoSerializationInterfaceTemplate < FemtoSHCF, CorrFitVerticalSlicesSH> + ;
#pragma link C++ class Hal::FemtoSerializationInterfaceTemplate < Femto1DCF, CorrFitVerticalSlices1D> + ;
#pragma link C++ class Hal::FemtoSerializationInterfaceTemplate < Femto3DCF, CorrFitVerticalSlices3D> + ;
//

#endif

// #pragma link C++ class Led+;

#endif
