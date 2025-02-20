/*
 * CorrFit.cxx
 *
 *  Created on: 24 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFit.h"

#include "CorrFitPainter.h"
#include "Cout.h"
#include "Std.h"

#include <TF1.h>

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
      fParameters[i].SetMapMin(-9999);
      fParameters[i].SetMapMax(9999);
    }
    fTempParamsEval = new Double_t[fParametersNo];
  }

  std::vector<TString> CorrFit::AlgoToOptions(EMinAlgo algo) const {
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
      case kGLSMultiMinConjungateFR: {
        pattern = "GSLMultiMin:ConjugateFR";
      } break;
      case kGLSMultiMinConjugatePR: {
        pattern = "GSLMultiMin:ConjugatePR";
      } break;
      case kGLSMultiMinBFGS: {
        pattern = "GSLMultiMin:BFGS";
      } break;
      case kGLSMultiMinBFGS2: {
        pattern = "GSLMultiMin:BFGS2";
      } break;
      case kGLSMultiMinSteppestDescent: {
        pattern = "GSLMultiMin:SteepestDescent";
      } break;
      case kGLSMultiFit: {
        pattern = "GSLMultiFit:";
      } break;
      case kGLSSimAn: {
        pattern = "GSLSimAn:";
      } break;
      case kDefaultAlgo: {
        pattern = "Minuit2:Migrad";
      } break;
      case kHalScan: {
        pattern = "HalMinimizer:Scan";
      } break;
      case kHalAnt: {
        pattern = "HalMinimizer:Ant";
      } break;
      case kHalScanMigrad: {
        pattern = "HalMinimizer:Scan:Minuit2:Migrad";
      } break;
      case kHalScanScan: {
        pattern = "HalMinimizer:Scan:Minuit2:Migrad";
      } break;
    }
    res = Hal::Std::ExplodeString(pattern, ':');
    return res;
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
    Cout::Database({"ParName", "Val", "Err", "Min", "Max"});
    for (int i = 0; i < GetParametersNo(); i++) {
      if (fParameters[i].IsFixed()) {
        Cout::Database({GetParameterName(i), Form("%4.2f", GetParameter(i)), "fixed", "-", "-"});
        break;
      } else {
        Cout::Database({GetParameterName(i),
                        Form("%4.2f", GetParameter(i)),
                        Form("%4.2f", GetParError(i)),
                        Form("%4.2f", GetParMin(i)),
                        Form("%4.2f", GetParMax(i))});
      }
    }
    Cout::Stars(kWhite);
  }

  void CorrFit::CopyParamsToTF1(TF1* f, Bool_t numPar, Bool_t graphPar) const {
    if (numPar)
      for (int i = 0; i < GetParametersNo(); i++) {
        f->FixParameter(i, GetParameter(i));
        f->SetParName(i, GetParameterName(i));
      }
    if (graphPar) {
      f->SetLineColor(GetLineColor());
      f->SetLineStyle(GetLineStyle());
      f->SetLineWidth(GetLineWidth());
    }
  }

  CorrFit::~CorrFit() {
    for (auto f : fDrawFunc) {
      if (f.first != nullptr) { delete f.first; }
    }
    if (fPainter) delete fPainter;
    /*delete[] fTempParamsEval;*/
  }

  void CorrFit::SetErrorsNegative() {
    for (auto& i : fParameters)
      i.SetError(-1);
  }

  void CorrFit::SetFitOrder(std::initializer_list<Int_t> order) {
    auto vec = Hal::Std::GetVector(order);
    if (int(vec.size()) != fParametersNo) {
      Hal::Cout::PrintInfo("Cannot change fit order, wrong size of list", EInfo::kWarning);
      return;
    }
    std::vector<bool> vals(fParametersNo, false);
    for (unsigned int it = 0; it < vec.size(); it++) {
      vals[vec[it]] = true;
    }

    for (auto i : vals) {
      if (i == false) {
        Hal::Cout::PrintInfo("Cannot change fit order, wrong values", EInfo::kWarning);
        return;
      }
    }
    fFitOrder = vec;
  }

  void CorrFit::CheckOrder() {
    if (fFitOrder.size() == 0) {
      fFitOrder.resize(fParametersNo);
      for (int i = 0; i < fParametersNo; i++) {
        fFitOrder[i] = i;
      }
    }
  }

}  // namespace Hal
