/*
 * CorrFit.cxx
 *
 *  Created on: 24 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFit.h"

#include "Cout.h"

namespace Hal {
  CorrFit::CorrFit(Int_t parameters_no) :
    fLineColor(kRed),
    fLineStyle(1),
    fLineWidth(2),
    fParametersNo(parameters_no),
    fBinCalc(kSimple),
    fMinFunc(kChi),
    fMinAlgo(kDefaultAlgo),
    fNDF(0),
    fThreshold(0) {
    fParameters.resize(fParametersNo);
    for (int i = 0; i < fParametersNo; i++) {
      fParameters[i].SetMapMin(0);
      fParameters[i].SetMapMax(10);
    }
    fTempParamsEval = new Double_t[fParametersNo];
  }

  void CorrFit::AlgoToOptions(EMinAlgo algo, TString& opt1, TString& opt2) const {
    switch (algo) {
      case kMinuitMigrad: {
        opt1 = "Minuit2";
        opt2 = "Migrad";
      } break;
      case kMinuitSimplex: {
        opt1 = "Minuit2";
        opt2 = "Simplex";
      } break;
      case kMinuitCombined: {
        opt1 = "Minuit2";
        opt2 = "Combined";
      } break;
      case kMinuitScan: {
        opt1 = "Minuit2";
        opt2 = "Scan";
      } break;
      case kMinuitFumili: {
        opt1 = "Minuit2";
        opt2 = "Fumili";
      } break;
      case kGLSMultiMinConjungateFR: {
        opt1 = "GSLMultiMin";
        opt2 = "ConjugateFR";
      } break;
      case kGLSMultiMinConjugatePR: {
        opt1 = "GSLMultiMin";
        opt2 = "ConjugatePR";
      } break;
      case kGLSMultiMinBFGS: {
        opt1 = "GSLMultiMin";
        opt2 = "BFGS";
      } break;
      case kGLSMultiMinBFGS2: {
        opt1 = "GSLMultiMin";
        opt2 = "BFGS2";
      } break;
      case kGLSMultiMinSteppestDescent: {
        opt1 = "GSLMultiMin";
        opt2 = "SteepestDescent";
      } break;
      case kGLSMultiFit: {
        opt1 = "GSLMultiFit";
        opt2 = "";
      } break;
      case kGLSSimAn: {
        opt1 = "GSLSimAn";
        opt2 = "";
      } break;
      case kDefaultAlgo: {
        opt1 = "Minuit2";
        opt2 = "Migrad";
      } break;
      case kMinimizerScan: {
        opt1 = "MInimizer";
        opt2 = "Scan";
      } break;
      case kMinimizerSmartScan: {
        opt1 = "Minimizer";
        opt2 = "SmartScan";
      } break;
    }
  }

  void CorrFit::ChangeParametersNo() {
    fParameters.resize(fParametersNo);
    delete[] fTempParamsEval;
    fTempParamsEval = new Double_t[fParametersNo];
  }

  void CorrFit::SetParLimits(Int_t par, Double_t min, Double_t max) {
    if (par < fParametersNo && par >= 0) { fParameters[par].SetRange(min, max); }
  }

  void CorrFit::FixParameter(Int_t par, Double_t val) {
    if (par < fParametersNo && par >= 0) { fParameters[par].SetRange(val, val); }
  }

  void CorrFit::SetParameterName(Int_t par, TString name) {
    if (par >= 0 && par < fParametersNo) { fParameters[par].SetParName(name); }
  }

  Int_t CorrFit::GetParameterIndex(TString name) const {
    for (int i = 0; i < fParametersNo; i++) {
      if (fParameters[i].GetParName().EqualTo(name)) return i;
    }
    return -1;
  }

  Bool_t CorrFit::IsParFixed(Int_t par) const {
    if (par >= 0 && par < fParametersNo) { return fParameters[par].IsFixed(); }
    return kFALSE;
  }

  Bool_t CorrFit::IsParDiscrete(Int_t par) const {
    if (par >= 0 && par < fParametersNo) { return fParameters[par].IsDiscrete(); }
    return kFALSE;
  }

  Double_t CorrFit::GetParMin(Int_t par) const {
    if (par >= 0 && par < fParametersNo) return fParameters[par].GetMin();
    return 0;
  }

  Double_t CorrFit::GetParMax(Int_t par) const {
    if (par >= 0 && par < fParametersNo) return fParameters[par].GetMax();
    return 10;
  }

  Double_t CorrFit::GetParameter(Int_t par) const {
    if (par >= 0 && par < fParametersNo) {
      return fParameters[par].GetFittedValue();
    } else {
      return 0;
    }
  }

  Double_t CorrFit::GetParError(Int_t par) const {
    if (par >= 0 && par < fParametersNo) {
      return fParameters[par].GetError();
    } else {
      return 0;
    }
  }

  void CorrFit::PrintFitResults() const {
    Cout::Text(Form("%s fit results", ClassName()), "M", kWhite);
    Cout::Database(5, "ParName", "Val", "Err", "Min", "Max");
    for (int i = 0; i < GetParametersNo(); i++) {
      if (fParameters[i].IsFixed()) {
        Cout::Database(5, GetParameterName(i).Data(), Form("%4.2f", GetParameter(i)), "fixed", "-", "-");
        break;
      } else {
        Cout::Database(5,
                       GetParameterName(i).Data(),
                       Form("%4.2f", GetParameter(i)),
                       Form("%4.2f", GetParError(i)),
                       Form("%4.2f", GetParMin(i)),
                       Form("%4.2f", GetParMax(i)));
      }
    }
    Cout::Stars(kWhite);
  }

  CorrFit::~CorrFit() { /*delete[] fTempParamsEval;*/
  }
}  // namespace Hal
