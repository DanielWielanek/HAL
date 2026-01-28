/*
 * CorrFit1DCFMultiDim.cxx
 *
 *  Created on: 29 lip 2025
 *      Author: daniel
 */

#include "CorrFit1DCFMultiDim.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH3.h>
#include <TString.h>
#include <iostream>
#include <string>

#include "Array.h"
#include "CorrFit.h"
#include "CorrFitFunctor.h"
#include "CorrFitInfo.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "FemtoConst.h"
#include "Minimizer.h"
#include "Std.h"
#include "StdHist.h"

namespace Hal {

  CorrFit1DCFMultiDim::CorrFit1DCFMultiDim(Int_t pars, Bool_t autoswap) : CorrFit1DCF(pars) {
    if (autoswap) {
      fNormParIndex   = pars - 2;
      fLambdaParIndex = pars - 1;
      fRinvParIndex   = pars - 3;
      SetParameterName(NormID(), "N");
      SetParameterName(LambdaID(), "#lambda");
      SetParameterName(RadiusID(), "R");
    }
  }

  void CorrFit1DCFMultiDim::Check() {
    CorrFit1DCF::Check();
    CorrFitInfo* info = fFunctor->GetInfo();
    Femto1DCF* cf     = dynamic_cast<Femto1DCF*>(info->GetCf());
    if (cf == nullptr) { Cout::PrintInfo("Cannot find 1DCF in CorrFitInfo !", EInfo::kCriticalError); }
    Femto1DCF* thisCf = (Femto1DCF*) fCF;
    if (thisCf->GetFrame() != cf->GetFrame()) { Cout::PrintInfo("CorrFitInfo incompatible frames !", EInfo::kCriticalError); }
    TH3* testThis = (TH3*) thisCf->GetNum();
    TH3* testMap  = (TH3*) cf->GetNum();
    if (!Hal::Std::AreSimilar(testThis, testMap, kFALSE)) {
      Cout::PrintInfo("CorrFitInfo incompatible histograms in CorrFitInfo and Femto1DCF !", EInfo::kError);
    }
  }

  void CorrFit1DCFMultiDim::RecalculateFunction() const {
    fData = fFunctor->GetData(fTempParamsEval);
    for (int i = 0; i < GetParametersNo(); i++) {
      std::cout << i << " " << fTempParamsEval[i] << std::endl;
    }
    std::cout << "RECAL" << std::endl;
  }

  void CorrFit1DCFMultiDim::PrepareHalMinimizer() const {
    std::cout << "PREPARING MINIMIZER" << GetParametersNo() << std::endl;
    Minimizer* min = Minimizer::Instance();
    for (int i = 0; i < GetParametersNo(); i++) {
      std::string par_name = GetParameterName(i).Data();
      if (!IsParFixed(i)) {  // fixed parameters will be configured later
        TString name = par_name;
        Double_t Min, Max;
        Int_t points;
        if (fFunctor->GetParameterConfig(name, Min, Max, points)) {
          Double_t step_size = (Max - Min) / (Double_t(points - 1));
          if (points == 0) step_size = 0;
          std::string Name = name.Data();
          min->SetLimitedVariable(i, Name, 0.5 * (Min + Max), step_size, Min, Max);
        } else {
          min->SetLimitedVariable(i, par_name, 0.5 * (GetParMin(i) + GetParMax(i)), 0.01, GetParMin(i), GetParMax(i));
        }
        min->SetVariableLimits(i, GetParMin(i), GetParMax(i));
      } else {
        min->SetLimitedVariable(i, par_name, GetParMin(i), 1, GetParMin(i), GetParMin(i));
        min->SetFixedVariable(i, par_name, GetParMin(i));
      }
    }
    std::cout << "PREPARING MINIMIZER DONE !" << std::endl;
  }

  Double_t CorrFit1DCFMultiDim::CalculateCF(const Double_t* x, const Double_t* params) const {
    /**calculate bin in flat array **/
    Double_t bin = (fBinX - 1);
    Double_t num = fData->Get(bin) - 1.0;
    if (TMath::IsNaN(num)) num = 0;
    return params[NormID()] * (1.0 + num * params[LambdaID()]);
  }

  void CorrFit1DCFMultiDim::SetFunctorFromMap(TString filename) {
    fFunctor = new CorrFitFunctor(filename);
    SetAutoLimits();
  }

  void CorrFit1DCFMultiDim::SetFunctor(CorrFitFunctor* functor) {
    fFunctor = functor;
    SetAutoLimits();
  }

  void CorrFit1DCFMultiDim::SetMinimizer(EMinAlgo alg) {
    if (alg == kHalScan || alg == kHalAnt) {
      if (alg == kHalAnt) { std::cout << "Smart scan" << std::endl; }
      CorrFit1DCF::SetMinimizer(alg);
    } else {
      Cout::Text("This class supports correctly only Minimizer", "L", kYellow);
    }
  }

  void CorrFit1DCFMultiDim::SetAutoLimits() {
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
      fParameters[i].SetParName(name);
      SetParLimits(i, min, max);
    }
#ifdef _MULTIDIM_DEBUG_
    std::cout << "AUTO DONE" << std::endl;
#endif
  }

  CorrFit1DCFMultiDim::~CorrFit1DCFMultiDim() {
    if (fFunctor) delete fFunctor;
  }


} /* namespace Hal */
