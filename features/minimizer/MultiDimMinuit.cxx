/*
 * MultiDimMinuit.cxx
 *
 *  Created on: 23 sty 2025
 *      Author: daniel
 */

#include "MultiDimMinuit.h"

#include <Math/Factory.h>
#include <Math/Functor.h>
#include <Math/Minimizer.h>
#include <TMathBase.h>

#include "Cout.h"
#include "MultiDimDataManager.h"
#include "MultiDimFile.h"
#include "MultiDimInterpolator.h"
#include "Std.h"
#include "StdString.h"


namespace Hal {

  std::vector<TString> MultiDimMinuit::AlgoToOptions(EMinAlgo algo) {
    TString pattern;
    std::vector<TString> res;
    switch (algo) {
      case kMinuitMigrad: {
        pattern = "Minuit2:Migrad";
      } break;
      case kMinuitSimplex: {
        pattern = "Minuit2:Simplex";
      } break;
      case kMinuitCombined: {
        pattern = "Minuit2:Combined";
      } break;
      case kMinuitScan: {
        pattern = "Minuit2:Scan";
      } break;
      case kMinuitFumili: {
        pattern = "Minuit2:Scan";
      } break;
    }
    res = Hal::Std::ExplodeString(pattern, ':');
    return res;
  }

  MultiDimMinuit::MultiDimMinuit() {}

  void MultiDimMinuit::OpenFile(TString file) {
    if (fInit) {
      Hal::Cout::PrintInfo("Cannot open two files in MultiDimMinuit", EInfo::kError);
      return;
    }
    fFunc = new Hal::MultiDimInterpolator();
    fFunc->OpenFile(file);
    fParameters = fFunc->GetConfig()->GetParams();
    for (auto& x : fParameters) {
      x.SetStartVal(0.5 * (x.GetMapMax() + x.GetMapMin()));
      Double_t mapLow  = x.GetMapMin();
      Double_t mapHigh = x.GetMapMax();
      if (mapLow == mapHigh) {
        x.SetMin(mapLow);
        x.SetMax(mapHigh);
        x.SetIsFixed(true);
      } else {
        x.SetMin(mapLow);
        x.SetMax(mapHigh);
      }
    }
    fInit = true;
  }

  void MultiDimMinuit::SetParLimits(Int_t par, Double_t min, Double_t max) {
    if (par < 0 || par >= fParameters.size()) return;
    fParameters[par].SetMin(min);
    fParameters[par].SetMax(max);
    fParameters[par].SetStartVal(0.5 * (min + max));
    if (min != max)
      fParameters[par].SetIsFixed(kFALSE);
    else
      fParameters[par].SetIsFixed(kTRUE);
  }

  void MultiDimMinuit::FixParameter(Int_t par, Double_t val) {
    if (par < 0 || par >= fParameters.size()) return;
    fParameters[par].SetMin(val);
    fParameters[par].SetMax(val);
    fParameters[par].SetStartVal(val);
    fParameters[par].SetIsFixed(true);
  }

  void MultiDimMinuit::Minimize() {
    auto algos = AlgoToOptions(fMinAlgo);
    if (algos.size() < 2) algos.push_back("");
    TString pat1 = algos[0];
    TString pat2 = algos[1];
    fMinimizer   = ROOT::Math::Factory::CreateMinimizer(pat1.Data(), pat2.Data());
    fValues.resize(fParameters.size());
    ROOT::Math::Functor f = ROOT::Math::Functor(this, &MultiDimMinuit::GetEval, fParameters.size());
    fMinimizer->SetFunction(f);
    for (int i = 0; i < fParameters.size(); i++) {
      auto Param = fParameters[i];
      if (Param.GetMin() == Param.GetMax()) Param.SetIsFixed(kTRUE);
      Param.Init();
      if (Param.IsFixed()) {
        fMinimizer->SetFixedVariable(i, Param.GetParName().Data(), Param.GetStartVal());
      } else {
        Double_t step = TMath::Max(Param.GetStepSize(), (Param.GetMax() - Param.GetMin()) / 100.0);
        fMinimizer->SetLimitedVariable(i, Param.GetParName().Data(), Param.GetStartVal(), step, Param.GetMin(), Param.GetMax());
      }
    }
    fMinimizer->Minimize();
  }

  Double_t MultiDimMinuit::GetParameter(Int_t par) const {
    if (fMinimizer) return fMinimizer->X()[par];
    Hal::Cout::PrintInfo("Cannot get par MultiDimMinuit - no minimizer did you forget call Minimize?", EInfo::kError);
    return -1;
  }

  Double_t MultiDimMinuit::GetParError(Int_t par) const {
    if (fMinimizer) return fMinimizer->Errors()[par];
    Hal::Cout::PrintInfo("Cannot get par MultiDimMinuit - no minimizer did you forget call Minimize?", EInfo::kError);
    return -1;
  }

  double MultiDimMinuit::GetEval(const double* params) {
    int i = -1;
    for (auto& x : fValues) {
      x = params[++i];
    }
    return fFunc->Extrapolate(fValues);
  }

  MultiDimMinuit::~MultiDimMinuit() {
    if (fFunc) delete fFunc;
  }

  TString MultiDimMinuit::GetParameterName(Int_t par) const {
    if (par < 0 || par >= fParameters.size()) return "";
    return fParameters[par].GetParName();
  }

  void MultiDimMinuit::SetStarParam(Int_t par, Double_t start) {
    if (par < 0 || par >= fParameters.size()) return;
    fParameters[par].SetStartVal(start);
  }

} /* namespace Hal */
