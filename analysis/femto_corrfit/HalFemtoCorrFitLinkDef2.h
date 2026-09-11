#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace Hal::Femto;

#pragma link C++ class Hal::FemtoDumpPairAna + ;
#pragma link C++ class Hal::CorrFitMapGroupConfig + ;
#pragma link C++ class Hal::CorrFitDumpedPairAnaVertical + ;

#pragma link C++ class Hal::CorrFit + ;
#pragma link C++ class Hal::CorrFitFunc + ;
#pragma link C++ class Hal::CorrFitFunc3D + ;
#pragma link C++ class Hal::CorrFitMath1DCF + ;
#pragma link C++ class Hal::CorrFit1DCF_Sum + ;
#pragma link C++ class Hal::CorrFit1DCF_SumRsame + ;
#pragma link C++ class Hal::CorrFit1DCF_Multi + ;
#pragma link C++ class Hal::CorrFit1DCF_Poly + ;
#pragma link C++ class Hal::CorrFit1DCF_Poly2 + ;
#pragma link C++ class Hal::CorrFitMath3DCF + ;;
#pragma link C++ class Hal::CorrFit3DCF_Sum + ;
#pragma link C++ class Hal::CorrFit1DCF_Square + ;
#pragma link C++ class Hal::CorrFit3DCF_Square + ;
#pragma link C++ class Hal::CorrFit3DCF_Lorentz + ;
#pragma link C++ class Hal::CorrFit1DCF_Lorentz + ;
#pragma link C++ class Hal::CorrFit1DCFCumac + ;
#pragma link C++ class Hal::CorrFit1DCFCumacLamLam + ;
#pragma link C++ class Hal::CorrFit1DCFCumacPLam + ;
#pragma link C++ class Hal::CorrFit1DCFCumacK0K0 + ;
#pragma link C++ class Hal::CorrFit1DCFCumacK0Kch + ;
#pragma link C++ class Hal::CorrFit1DCFCumacDLam + ;
#pragma link C++ class Hal::CorrFit1DCFCumacStrong + ;

#pragma link C++ class Hal::CorrFitParamsSetup + ;
#pragma link C++ class Hal::CorrFit3DCFMultiDim + ;
#pragma link C++ class Hal::CorrFitFunctor + ;
#pragma link C++ class Hal::CorrFitMapRebin + ;
#pragma link C++ class Hal::CorrFitGUI + ;
#pragma link C++ class Hal::CorrFitParButton + ;
#pragma link C++ class Hal::CorrFitChiSelector + ;
#pragma link C++ class Hal::CorrFitHDFunc + ;
#pragma link C++ class Hal::CorrFitHDFunc1D + ;
#pragma link C++ class Hal::CorrFitHDFunc3D + ;

#pragma link C++ class Hal::CorrFitPairFile + ;
#pragma link C++ class Hal::CorrFitPairGenerator + ;
#pragma link C++ class Hal::CorrFitPairGeneratorSimple + ;
#pragma link C++ class Hal::CorrFitPairGeneratorYPtKt + ;
#pragma link C++ class Hal::CorrFitPairGeneratorYPtKtStubborn + ;
#pragma link C++ class Hal::CorrFitPairGeneratorConvolutionYPt + ;
#pragma link C++ class Hal::ConvolutionPairGenerator + ;

#pragma link C++ class Hal::CorrFit1DCF + ;
#pragma link C++ class Hal::CorrFit3DCF + ;
#pragma link C++ class Hal::CorrFitKisiel + ;
#pragma link C++ class Hal::CorrFitSmear1DCF + ;
#pragma link C++ class Hal::CorrFitMapKstarRstar + ;
#pragma link C++ class Hal::CorrFitMapKstarRstarDiv + ;
#pragma link C++ class Hal::FemtoWielanekMapGenerator + ;
#pragma link C++ class Hal::Femto1DMapGenerator + ;
#pragma link C++ class Hal::Femto1DCFAnaMapMC + ;
#pragma link C++ class Hal::Femto1DCFAnaMapMCRoco + ;
#pragma link C++ class Hal::Femto1DCFAnaMapPairsDumped + ;
#pragma link C++ class Hal::FemtoCorrFuncMap + ;
#pragma link C++ class Hal::CorrFitWielanek + ;
#pragma link C++ class Hal::CorrFit3DCFBowlerSinyukov + ;
#pragma link C++ class Hal::CorrFit3DCFBowlerSinyukovEllipse + ;
#pragma link C++ class Hal::CorrFit3DCFBowlerSinyukovClassic + ;

#pragma link C++ class Hal::Femto1DCFAnaMap + ;

#pragma link C++ class Hal::CorrFit1DCF_Exp + ;
#pragma link C++ class Hal::CorrFit1DCF_Gauss + ;

#pragma link C++ class Hal::CorrFit3DCF_Gauss + ;
#pragma link C++ class Hal::CorrFit3DCF_Gauss2 + ;
#pragma link C++ class Hal::CorrFit3DCF_GaussCross + ;

#pragma link C++ class Hal::CorrFitInfo + ;
#pragma link C++ class Hal::CorrFitDumpedPairAnaVertical + ;
#pragma link C++ class Hal::CorrFitDumpedPairAnaHorizontal + ;
#pragma link C++ class Hal::CorrFitDumpedPairAna + ;

#pragma link C++ class Hal::CorrFitPainter + ;
#pragma link C++ class Hal::CorrFitSHCFPainter + ;
#pragma link C++ class Hal::CorrFit3DCFPainter + ;
#pragma link C++ class Hal::CorrFit1DCFPainter + ;


#ifndef GSL_DISABLE
#pragma link C++ class Hal::FemtoImaging + ;
#pragma link C++ class Hal::FemtoImaging1D + ;
#pragma link C++ class Hal::FemtoImaging3D + ;

#pragma link C++ class Hal::CorrFitMask + ;
#pragma link C++ class Hal::CorrFitMask1D + ;
#pragma link C++ class Hal::CorrFitMask3D + ;
#pragma link C++ class Hal::CorrFitMaskSH + ;


#pragma link C++ class Hal::CorrFitSHCF + ;
#pragma link C++ class Hal::CorrFitSHFrom3D + ;
#pragma link C++ class Hal::FemtoSHArray + ;
#pragma link C++ class Hal::CorrFitMaskHisto + ;
#pragma link C++ class Hal::CorrFitSHCFMultiDim + ;
#pragma link C++ class Hal::CorrFitHDFuncSH + ;
#pragma link C++ class Hal::BattlestarGalactica + ;
// #pragma link C++ class Hal::BattlestarGalactica10 + ;
#pragma link C++ class Hal::BattlestarPegasus + ;
#pragma link C++ class Hal::BattlestarValkyria + ;
#pragma link C++ class Hal::BattlestarAtlantia + ;
#pragma link C++ class Hal::BattlestarSolaria + ;
#pragma link C++ class Hal::BattlestarAcropolis + ;
//

#pragma link C++ class Hal::CorrFitSmearingMath1DCF + ;
#pragma link C++ class Hal::CorrFitSmearingMath2DYield + ;
#pragma link C++ class Hal::CorrFitSmearingMath3DYield + ;
#pragma link C++ class Hal::CorrFit1DSmearing + ;

#pragma link C++ class Hal::CorrFitSHCFOTF + ;
#pragma link C++ class Hal::CorrFitSHGauss + ;
#pragma link C++ class Hal::CorrFitSHGaussGSL + ;

#pragma link C++ class Hal::FemtoSHDecomposerFunc + ;
#pragma link C++ class Hal::FemtoSHDecomposerFuncGausLCMS + ;
#pragma link C++ class Hal::FemtoSHCFDecomposer + ;
#pragma link C++ class Hal::FemtoSHDecomposerFuncGausPRF + ;
#pragma link C++ class Hal::FemtoSHCFDecomposerGSL + ;
#pragma link C++ class Hal::FemtoSHCFDecomposerGSLVc + ;

#pragma link C++ class Hal::SkurwolKernelGenerator + ;
#pragma link C++ class Hal::SkurwolCalculator + ;
#pragma link C++ class Hal::GausianSkurwolKalculator + ;
#pragma link C++ class Hal::SkurwolGaussianFitter + ;
#pragma link C++ class Hal::NumericalSHDecomposer + ;
#pragma link C++ class Hal::FemtoDecomposerGaus + ;
#endif

// #pragma link C++ class Led+;

#endif
