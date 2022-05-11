/*
 * CorrFit3DCFMultiDim.cxx
 *
 *  Created on: 18 maj 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFit3DCFMultiDim.h"

#include "CorrFit.h"
#include "CorrFitFunctor.h"
#include "CorrFitInfo.h"
#include "Cout.h"
#include "Femto3DCF.h"
#include "Minimizer.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH1.h>
#include <TString.h>
#include <iostream>
#include <string>


namespace Hal {
  void CorrFit3DCFMultiDim::RecalculateFunction() const {
    Double_t pars[3];
    pars[0] = fTempParamsEval[Rout()];
    pars[1] = fTempParamsEval[Rside()];
    pars[2] = fTempParamsEval[Rlong()];
    fData   = fFunctor->GetData(pars);
  }

  Double_t CorrFit3DCFMultiDim::CalculateCF(const Double_t* /*x*/, const Double_t* params) const {
    Int_t sizeX = fNumeratorHistogram->GetNbinsX();
    Int_t sizeY = fNumeratorHistogram->GetNbinsY();
    /**calcuclate bin in flat array **/
    Double_t bin = (fBinX - 1) * fFunctorXbins * fFunctorYbins + (fBinY - 1) * fFunctorYbins + fBinZ - 1;
    Double_t num = fData->Get(bin) - 1.0;
    return params[Norm()] * (1.0 + num * params[Lambda()]);
  }

  CorrFit3DCFMultiDim::CorrFit3DCFMultiDim(Int_t params) :
    CorrFit3DCF(params), fFunctor(nullptr), fData(nullptr), fFunctorXbins(0), fFunctorYbins(0) {
    fMinAlgo = EMinAlgo::kMinimizerScan;
  }

  void CorrFit3DCFMultiDim::SetFunctor(CorrFitFunctor* functor) {
    fFunctor = functor;
    SetAutoLimits();
  }

  void CorrFit3DCFMultiDim::SetFunctorFromMap(TString filename) {
    fFunctor = new CorrFitFunctor(filename);
    SetAutoLimits();
  }

  void CorrFit3DCFMultiDim::SetAutoLimits() {
    TString name;
    Double_t min, max;
    Int_t points;
    // get out
    Int_t pars[3] = {Rout(), Rside(), Rlong()};
    for (int i = 0; i < 3; i++) {
      fFunctor->GetParameterConfig(i, name, min, max, points);
      fParameters[pars[i]].SetIsDiscrete(kTRUE);
      fParameters[pars[i]].SetMapRange(min, max, points);
      SetParLimits(pars[i], min, max);
    }
  }

  CorrFit3DCFMultiDim::~CorrFit3DCFMultiDim() {
    if (fFunctor) delete fFunctor;
  }

  void CorrFit3DCFMultiDim::SetMinimizer(EMinAlgo alg) {
    if (alg == kMinimizerScan || alg == kMinimizerSmartScan) {
      if (alg == kMinimizerSmartScan) { std::cout << "Smart scan" << std::endl; }
      CorrFit3DCF::SetMinimizer(alg);
    } else {
      Cout::Text("This class supports correctly only Minimizer", "L", kYellow);
    }
  }

  void CorrFit3DCFMultiDim::PrepareMinimizer() {
    std::cout << "PREPARING MINIMIZER" << GetParametersNo() << std::endl;
    Minimizer* min = Minimizer::Instance();
    for (int i = 0; i < GetParametersNo(); i++) {
      std::string par_name = GetParameterName(i).Data();
      if (!IsParFixed(i)) {  // fixed parameters will be configured later
        Double_t lower = GetParMin(i);
        Double_t upper = GetParMax(i);
        if (i <= Rlong()) {
          TString name;
          Double_t Min, Max;
          Int_t steps;
          fFunctor->GetParameterConfig(i, name, Min, Max, steps);
          Double_t step_size = (Max - Min) / (Double_t(steps));
          if (steps == 0) step_size = 0;
          std::cout << "SET MAP STEPS " << par_name << "\t" << Min << " " << Max << " " << steps << " "
                    << " " << step_size << std::endl;
          std::string Name = name.Data();
          min->SetLimitedVariable(i, Name, 0.5 * (Min + Max), step_size, Min, Max);
        } else {
          min->SetLimitedVariable(
            i, GetParameterName(i).Data(), 0.5 * (GetParMin(i), GetParMax(i)), 0.01, GetParMin(i), GetParMax(i));
        }
      } else {
        std::cout << "SET FIXED " << par_name << "\t" << GetParMin(i) << " " << GetParMax(i) << std::endl;
        min->SetFixedVariable(i, par_name, GetParMin(i));
      }
    }
    std::cout << "PREPARING MINIMIZER DONE !" << std::endl;
  }

  void CorrFit3DCFMultiDim::Check() {
    CorrFit3DCF::Check();
    CorrFitInfo* info = fFunctor->GetInfo();
    Femto3DCF* cf     = dynamic_cast<Femto3DCF*>(info->GetCf());
    if (cf == nullptr) { Cout::PrintInfo("Cannot find 3DCF in CorrFitInfo !", EInfo::kImportantError); }
    Femto3DCF* thisCf = (Femto3DCF*) fCF;
    if (thisCf->GetFrame() != cf->GetFrame()) { Cout::PrintInfo("CorrFitInfo incompatible frames !", EInfo::kImportantError); }
    TH3* testThis = (TH3*) thisCf->GetNum();
    TH3* testMap  = (TH3*) cf->GetNum();
    if (!HalStd::AreSimilar(testThis, testMap, kFALSE)) {
      Cout::PrintInfo("CorrFitInfo incompatible histograms in CorrFitInfo and Femto3DCF !", EInfo::kLessError);
    }
    fFunctorXbins = testThis->GetXaxis()->GetNbins();
    fFunctorYbins = testThis->GetYaxis()->GetNbins();
  }
}  // namespace Hal
