/*
 * FemtoFastCut.cxx
 *
 *  Created on: 25-07-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoFastCut.h"

#include "Cout.h"
#include "FemtoPair.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"

#include <TCollection.h>
#include <TList.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TString.h>


namespace Hal {
  FemtoFastCut::FemtoFastCut() : fBins(0), fMin(NULL), fMax(NULL), fMinTrue(NULL), fMaxTrue(NULL), fPair(NULL) {}

  void FemtoFastCut::Allocate() {
    if (fBins == 0) {
      Cout::PrintInfo("No default value in FemtoFastCuts, put some default values", EInfo::kLowWarning);
      fBins   = 1;
      fMin    = new Double_t[1];
      fMax    = new Double_t[1];
      fMin[0] = -1E+9;
      fMax[0] = 1E+9;
    }
    for (int i = 1; i < fBins; i++) {
      if (fMax[i - 1] > fMin[i]) {
        Cout::PrintInfo("Overlaping bins in FemtoFastCuts, this lead to loosing pairs", EInfo::kLowWarning);
      }
      if (fMin[i - 1] != fMax[i]) {
        Cout::PrintInfo("Hole in FemtoFastCut, this might lead to loosing pairs", EInfo::kLowWarning);
      }
    }
  }

  void FemtoFastCut::Init(FemtoPair* pair) {
    if (fBins == 0) {
      Cout::PrintInfo("Empty femto fast cut, adding  large values", EInfo::kLowWarning);
      AddCutBin(-1E+9, 1E+9);
    }
    fMinTrue = new Double_t[fBins];
    fMaxTrue = new Double_t[fBins];
    for (int i = 0; i < fBins; i++) {
      fMinTrue[i] = fMin[i];
      fMaxTrue[i] = fMax[i];
    }
    fPair = pair;
    if (fPair == NULL) { Cout::PrintInfo(Form("No FemtoPair in %s", this->ClassName()), EInfo::kCriticalError); }
  }

  void FemtoFastCut::AddCutBin(Double_t min, Double_t max, Option_t* /*opt*/) {
    Double_t* old_min = fMin;
    Double_t* old_max = fMax;
    fMin              = new Double_t[fBins + 1];
    fMax              = new Double_t[fBins + 1];
    for (int i = 0; i < fBins; i++) {
      fMin[i] = old_min[i];
      fMax[i] = old_max[i];
    }
    fMin[fBins] = min;
    fMax[fBins] = max;
    fBins++;
    if (old_min) {
      delete[] old_min;
      delete[] old_max;
    }
  }

  FemtoFastCut::FemtoFastCut(const FemtoFastCut& copy) : TObject(copy) {
    if (copy.fPair) Cout::PrintInfo("FemtoFastCut already initialized", EInfo::kCriticalError);
    fPair = NULL;
    fBins = copy.fBins;
    fMin  = new Double_t[fBins];
    fMax  = new Double_t[fBins];
    if (fMinTrue) {
      fMinTrue = new Double_t[fBins];
      fMaxTrue = new Double_t[fBins];
    }
    for (int i = 0; i < fBins; i++) {
      fMin[i] = copy.fMin[i];
      fMax[i] = copy.fMax[i];
    }
    if (copy.fMinTrue) {
      for (int i = 0; i < fBins; i++) {
        fMinTrue[i] = copy.fMinTrue[i];
        fMaxTrue[i] = copy.fMaxTrue[i];
      }
    }
  }

  FemtoFastCut::~FemtoFastCut() {
    delete[] fMin;
    delete[] fMax;
    delete[] fMinTrue;
    delete[] fMaxTrue;
  }
  ////-----------------------------------------------------------------kt cut
  FemtoFastCutKt::FemtoFastCutKt() : FemtoFastCut() {}

  Int_t FemtoFastCutKt::GetBin() const {
    Double_t px  = fPair->GetPx1() + fPair->GetPx2();
    Double_t py  = fPair->GetPy1() + fPair->GetPy2();
    Double_t val = TMath::Sqrt(px * px + py * py) * 0.5;
    for (int i = 0; i < fBins; i++) {
      if (val >= fMinTrue[i] && val < fMaxTrue[i]) { return i; }
    }
    return -1;
  }

  Int_t FemtoFastCutKt::GetBinRotated() const {
    Double_t px  = fPair->GetPx1() - fPair->GetPx2();
    Double_t py  = fPair->GetPy1() - fPair->GetPy2();
    Double_t val = TMath::Sqrt(px * px + py * py) * 0.5;
    for (int i = 0; i < fBins; i++) {
      if (val >= fMinTrue[i] && val < fMaxTrue[i]) { return i; }
    }
    return -1;
  }

  Int_t FemtoFastCutKt::GetBinHemisphere() const { return GetBinRotated(); }

  FemtoFastCutKt::~FemtoFastCutKt() {}

  FemtoFastCutPtSum::FemtoFastCutPtSum() : FemtoFastCut() {}

  Int_t FemtoFastCutPtSum::GetBin() const {
    Double_t pt1 = TMath::Power(fPair->GetPx1(), 2) + TMath::Power(fPair->GetPy1(), 2);
    Double_t pt2 = TMath::Power(fPair->GetPx2(), 2) + TMath::Power(fPair->GetPy2(), 2);
    Double_t val = TMath::Sqrt(pt1) + TMath::Sqrt(pt2);
    for (int i = 0; i < fBins; i++) {
      if (val >= fMinTrue[i] && val < fMaxTrue[i]) { return i; }
    }
    return -1;
  }

  Int_t FemtoFastCutPtSum::GetBinRotated() const {
    Double_t pt1 = TMath::Power(fPair->GetPx1(), 2) + TMath::Power(fPair->GetPy1(), 2);
    Double_t pt2 = TMath::Power(fPair->GetPx2(), 2) + TMath::Power(fPair->GetPy2(), 2);
    Double_t val = TMath::Sqrt(pt1) + TMath::Sqrt(pt2);
    for (int i = 0; i < fBins; i++) {
      if (val >= fMinTrue[i] && val < fMaxTrue[i]) { return i; }
    }
    return -1;
  }

  Int_t FemtoFastCutPtSum::GetBinHemisphere() const { return GetBinRotated(); }

  FemtoFastCutPtSum::~FemtoFastCutPtSum() {}

  Package* FemtoFastCut::Report() const {
    Package* pack = new Package(this);
    TList* list   = new TList();
    TList* listF  = new TList();
    list->SetName("NumericValues");
    listF->SetName("FormatedNames");
    list->Add(new ParameterString("Mode", this->ClassName()));
    list->Add(new ParameterInt("Bins", fBins));
    for (int i = 0; i < fBins; i++) {
      list->Add(new ParameterFloat("Min", fMin[i]));
      list->Add(new ParameterFloat("Max", fMax[i]));
      listF->Add(new ParameterString(Form("%s_%i", this->ClassName(), i), Form("%.2f %.2f", fMin[i], fMax[i])));
    }
    pack->AddObject(list);
    pack->AddObject(listF);
    return pack;
  }

  FemtoFastCutPhi::FemtoFastCutPhi() {
    fEventPhi      = 0;
    fGlobalMin     = 0;
    fPhiCorrection = 0;
  }

  Int_t FemtoFastCutPhi::GetBin() const {
    Double_t px_sum = fPair->GetPx1() + fPair->GetPx2();
    Double_t py_sum = fPair->GetPy1() + fPair->GetPy2();
    Double_t val    = RoundPhi(TMath::ATan2(py_sum, px_sum) - fPhiCorrection);
    for (int i = 0; i < fBins; i++) {
      if (val >= fMinTrue[i] && val < fMaxTrue[i]) { return i; }
    }
    return -1;
  }

  Int_t FemtoFastCutPhi::GetBinRotated() const {
    Double_t px_sum = fPair->GetPx1() - fPair->GetPx2();
    Double_t py_sum = fPair->GetPy1() - fPair->GetPy2();
    Double_t val    = TMath::ATan2(py_sum, px_sum) - fPhiCorrection;
    val             = RoundPhi(val - TMath::Pi());
    for (int i = 0; i < fBins; i++) {
      if (val >= fMinTrue[i] && val < fMaxTrue[i]) { return i; }
    }
    return -1;
  }

  Int_t FemtoFastCutPhi::GetBinHemisphere() const { return GetBinRotated(); }

  void FemtoFastCutPhi::Init(FemtoPair* pair) {
    FemtoFastCut::Init(pair);
    for (int i = 0; i < fBins; i++) {
      fMin[i] = RoundPhi(fMin[i]);
      fMax[i] = RoundPhi(fMax[i]);
    }
    fGlobalMin = TMath::Pi();
    for (int i = 0; i < fBins; i++) {
      if (fMin[i] < fGlobalMin) { fGlobalMin = fMin[i]; }
    }
    for (int i = 0; i < fBins; i++) {
      fMinTrue[i] -= fGlobalMin;
      fMaxTrue[i] -= fGlobalMin;
      fMinTrue[i] = RoundPhi(fMinTrue[i]);
      fMaxTrue[i] = RoundPhi(fMaxTrue[i]);
      TryRoundRange(i);
    }
    for (int i = 0; i < fBins; i++) {
      fMin[i] = fMin[i] * TMath::RadToDeg();  // round to degrees to be
                                              // human-readable in HTML-report
      fMax[i] = fMax[i] * TMath::RadToDeg();
    }
  }

  Double_t FemtoFastCutPhi::RoundPhi(Double_t val) const {
    while (val < -TMath::Pi()) {
      val += TMath::Pi() * 2.0;
    }
    while (val >= TMath::Pi()) {
      val -= TMath::Pi() * 2.0;
    }
    return val;
  }

  void FemtoFastCutPhi::SetEventPhi(const Double_t phi) {
    fEventPhi      = phi;
    fPhiCorrection = fEventPhi + fGlobalMin;
    fPhiCorrection = RoundPhi(fPhiCorrection);
  }

  Package* FemtoFastCutPhi::Report() const { return FemtoFastCut::Report(); }

  void FemtoFastCutPhi::TryRoundRange(Int_t i) {
    if (fMinTrue[i] > fMaxTrue[i]) {
      Double_t MinMod = TMath::Abs(fMinTrue[i]);
      Double_t MaxMod = TMath::Abs(fMaxTrue[i]);
      Double_t dMax   = TMath::Abs(TMath::Pi() - MaxMod);
      Double_t dMin   = TMath::Abs(MinMod - TMath::Pi());
      if (dMax < dMin) {
        fMaxTrue[i] = -fMaxTrue[i];
      } else if (dMin < dMax) {
        fMinTrue[i] = -fMinTrue[i];
      } else {
        Cout::PrintInfo(Form("Something is wrong with %s cut", this->ClassName()), EInfo::kWarning);
      }
    }
  }

  FemtoFastCutPhi::~FemtoFastCutPhi() {}

  FemtoFastCutEta::FemtoFastCutEta() {
    // TODO Auto-generated constructor stub
  }

  Int_t FemtoFastCutEta::GetBin() const {
    Double_t px  = fPair->GetPx1() + fPair->GetPx2();
    Double_t py  = fPair->GetPy1() + fPair->GetPy2();
    Double_t pz  = fPair->GetPz1() + fPair->GetPz2();
    Double_t p   = TMath::Sqrt(px * px + py * py + pz * pz);
    Double_t eta = 0.5 * TMath::Log((p + pz) / (p - pz));
    for (int i = 0; i < fBins; i++) {
      if (eta >= fMinTrue[i] && eta < fMaxTrue[i]) { return i; }
    }
    return -1;
  }

  Int_t FemtoFastCutEta::GetBinRotated() const {
    Double_t px  = fPair->GetPx1() - fPair->GetPx2();
    Double_t py  = fPair->GetPy1() - fPair->GetPy2();
    Double_t pz  = fPair->GetPz1() + fPair->GetPz2();
    Double_t p   = TMath::Sqrt(px * px + py * py + pz * pz);
    Double_t eta = 0.5 * TMath::Log((p + pz) / (p - pz));
    for (int i = 0; i < fBins; i++) {
      if (eta >= fMinTrue[i] && eta < fMaxTrue[i]) { return i; }
    }
    return -1;
  }

  Int_t FemtoFastCutEta::GetBinHemisphere() const {
    Double_t px  = fPair->GetPx1() - fPair->GetPx2();
    Double_t py  = fPair->GetPy1() - fPair->GetPy2();
    Double_t pz  = fPair->GetPz1() - fPair->GetPz2();
    Double_t p   = TMath::Sqrt(px * px + py * py + pz * pz);
    Double_t eta = 0.5 * TMath::Log((p + pz) / (p - pz));
    for (int i = 0; i < fBins; i++) {
      if (eta >= fMinTrue[i] && eta < fMaxTrue[i]) { return i; }
    }
    return -1;
  }

  FemtoFastCutEta::~FemtoFastCutEta() {}

  FemtoFastCutVirtual::FemtoFastCutVirtual(const FemtoFastCutVirtual& /*fast*/) : FemtoFastCut() {}

  FemtoFastCutKt::FemtoFastCutKt(const FemtoFastCutKt& fast) : FemtoFastCut(fast) {}

  FemtoFastCutPtSum::FemtoFastCutPtSum(const FemtoFastCutPtSum& fast) : FemtoFastCut(fast) {}

  FemtoFastCutPhi::FemtoFastCutPhi(const FemtoFastCutPhi& fast) :
    FemtoFastCut(fast), fEventPhi(0), fGlobalMin(0), fPhiCorrection(0) {}

  FemtoFastCutEta::FemtoFastCutEta(const FemtoFastCutEta& fast) : FemtoFastCut(fast) {}
}  // namespace Hal
