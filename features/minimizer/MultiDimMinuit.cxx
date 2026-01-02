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
#include "MinimizerResult.h"
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


  MinimizerResult MultiDimMinuit::Minimize() {
    MinimizerResult result;
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
    int count = 0;
    for (auto par : fParameters) {
      FittedParam parFited(par);
      parFited.SetVal(fMinimizer->X()[count]);
      parFited.SetLowError(fMinimizer->Errors()[count]);
      parFited.SetHighError(fMinimizer->Errors()[count]);
      count++;
      result.AddParam(parFited);
    }
    return result;
  }

  MultiDimMinuit::~MultiDimMinuit() {}

  double MultiDimMinuit::GetEval(const double* params) {
    int i = -1;
    for (auto& x : fValues) {
      x = params[++i];
    }
    return fFunc->Extrapolate(fValues);
  }

} /* namespace Hal */
