/*
 * CorrFitSHCFMultiDim.cxx
 *
 *  Created on: 22 mar 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitSHCFMultiDim.h"

#include "CorrFitFunctor.h"
#include "CorrFitInfo.h"
#include "Cout.h"
#include "FemtoSHCF.h"
#include "Minimizer.h"

namespace Hal {

  void CorrFitSHCFMultiDim::RecalculateFunction() const {
    Double_t pars[3];
    pars[0] = fTempParamsEval[Rout()];
    pars[1] = fTempParamsEval[Rside()];
    pars[2] = fTempParamsEval[Rlong()];
    fData   = fFunctor->GetData(pars);
    CorrFitSHCF::RecalculateFunction();
  }

  Double_t CorrFitSHCFMultiDim::CalculateCF(const Double_t* x, const Double_t* params) const {
    //fBins;  // liczba binow
    int BinX = fNumeratorHistogram->GetXaxis()->FindBin(x[0]);
    // int bins = fNumeratorHistogram->GetNbinsX();
    const Int_t L = fLmVals.GetMaxL();
    if(BinX>fNumeratorHistogram->GetNbinsX()){
        for (int l = 0; l <= L; l++) {
          for (int m = -l; m <= l; m++) {
            int index = fLmVals.GetIndex(l, m);
            fYlmBuffer[index].real(0);
            fYlmBuffer[index].imag(0);
          }
        }
        return 0;
    }

    int step = fLmVals.GetMaxJM() * 2 * (BinX - 1);
    for (int l = 0; l <= L; l++) {
      for (int m = -l; m <= l; m++) {
        int index = fLmVals.GetIndex(l, m);
        fYlmBuffer[index].real(fData->Get(step++));
        fYlmBuffer[index].imag(fData->Get(step++));
        // tu jest raczej ok
      }
    }

    for (int i = 0; i < fLmVals.GetMaxJM(); i++) {
      double real = params[Norm()] * (1.0 + fYlmBuffer[i].real() * params[Lambda()]);
      double imag = params[Norm()] * (1.0 + fYlmBuffer[i].imag() * params[Lambda()]);
    //  fYlmBuffer[i].real(real);
    //  fYlmBuffer[i].imag(imag);
    }
    return 0;
  }

  CorrFitSHCFMultiDim::CorrFitSHCFMultiDim(Int_t params) :
    CorrFitSHCF(params), fFunctor(nullptr), fData(nullptr), fFunctorXbins(0), fFunctorYbins(0) {
    fMinAlgo = EMinAlgo::kMinimizerScan;
  }

  void CorrFitSHCFMultiDim::SetFunctor(CorrFitFunctor* functor) {
    fFunctor = functor;
    SetAutoLimits();
  }

  void CorrFitSHCFMultiDim::SetFunctorFromMap(TString filename) {
      if(fFunctor) delete fFunctor;
    fFunctor = new CorrFitFunctor(filename);
    SetAutoLimits();
  }

  void CorrFitSHCFMultiDim::SetAutoLimits() {
    TString name;
    Double_t min, max;
    Int_t points;
    // get out
#ifdef _MULTIDIM_DEBUG_
    std::cout << "SET AUTO LIMITS" << std::endl;
#endif
    for (int i = 0; i < fFunctor->GetNParams(); i++) {
      name = fFunctor->GetParameterName(i);
      fFunctor->GetParameterConfig(name, min, max, points);
      fParameters[i].SetIsDiscrete(kTRUE);
      fParameters[i].SetMapRange(min, max, points);
      SetParLimits(i, min, max);
    }
#ifdef _MULTIDIM_DEBUG_
    std::cout << "AUTO DONE" << std::endl;
#endif
  }

  CorrFitSHCFMultiDim::~CorrFitSHCFMultiDim() {
    if (fFunctor) delete fFunctor;
  }

  void CorrFitSHCFMultiDim::SetMinimizer(EMinAlgo alg) {
    if (alg == kMinimizerScan || alg == kMinimizerSmartScan) {
      if (alg == kMinimizerSmartScan) { std::cout << "Smart scan" << std::endl; }
      CorrFitSHCF::SetMinimizer(alg);
    } else {
      Cout::Text("This class supports correctly only Minimizer", "L", kYellow);
    }
  }

  void CorrFitSHCFMultiDim::PrepareMinimizer() {
    std::cout << "PREPARING MINIMIZER" << GetParametersNo() << std::endl;
    Minimizer* min = Minimizer::Instance();
    for (int i = 0; i < GetParametersNo(); i++) {
      std::string par_name = GetParameterName(i).Data();
      if (!IsParFixed(i)) {  // fixed parameters will be configured later
        Double_t lower = GetParMin(i);
        Double_t upper = GetParMax(i);

        TString name = par_name;
        Double_t Min, Max;
        Int_t points;
        if (fFunctor->GetParameterConfig(name, Min, Max, points)) {
          Double_t step_size = (Max - Min) / (Double_t(points - 1));
          if (points == 0) step_size = 0;
#ifdef _MULTIDIM_DEBUG_
          std::cout << "SET MAP STEPS " << par_name << "\t" << Min << " " << Max << " " << points << " "
                    << " " << step_size << std::endl;
#endif
          std::string Name = name.Data();
          min->SetLimitedVariable(i, Name, 0.5 * (Min + Max), step_size, Min, Max);
        } else {
#ifdef _MULTIDIM_DEBUG_
          std::cout << "SET FREE STEPS " << par_name << "\t" << GetParMin(i) << " " << GetParMax(i) << " " << points << " "
                    << " " << 0.01 << std::endl;
#endif
          min->SetLimitedVariable(i, par_name, 0.5 * (GetParMin(i) + GetParMax(i)), 0.01, GetParMin(i), GetParMax(i));
        }
        min->SetVariableLimits(i, GetParMin(i), GetParMax(i));
      } else {
#ifdef _MULTIDIM_DEBUG_
        std::cout << "SET FIXED " << par_name << "\t" << GetParMin(i) << " " << GetParMax(i) << std::endl;
#endif
        min->SetLimitedVariable(i, par_name, GetParMin(i), 1, GetParMin(i), GetParMin(i));
        min->SetFixedVariable(i, par_name, GetParMin(i));
      }
    }
    std::cout << "PREPARING MINIMIZER DONE !" << std::endl;
  }

  void CorrFitSHCFMultiDim::Check() {
    CorrFitSHCF::Check();
    CorrFitInfo* info = fFunctor->GetInfo();
    FemtoSHCF* cf     = dynamic_cast<FemtoSHCF*>(info->GetCf());
    if (cf == nullptr) { Cout::PrintInfo("Cannot find SHCF in CorrFitInfo !", EInfo::kCriticalError); }
    FemtoSHCF* thisCf = (FemtoSHCF*) fCF;
    if (thisCf->GetFrame() != cf->GetFrame()) { Cout::PrintInfo("CorrFitInfo incompatible frames !", EInfo::kCriticalError); }
    TH1* testThis = (TH1*) thisCf->GetNum();
    TH1* testMap  = (TH1*) cf->GetNum();
    if (!Hal::Std::AreSimilar(testThis, testMap, kFALSE)) {
      Cout::PrintInfo("CorrFitInfo incompatible histograms in CorrFitInfo and FemtoSHCF !", EInfo::kError);
    }
    if (cf->GetL() != thisCf->GetL()) {
      Cout::PrintInfo("CorrFitInfo incompatible histograms in CorrFitInfo and FemtoSHCF !", EInfo::kError);
    }
    fFunctorXbins = testThis->GetXaxis()->GetNbins();
    fFunctorYbins = testThis->GetYaxis()->GetNbins();
  }
}  // namespace Hal
