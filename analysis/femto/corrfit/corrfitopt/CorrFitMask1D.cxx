/*
 * CorrFitMask1D.cxx
 *
 *  Created on: 12 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitMask1D.h"

namespace Hal {
  Bool_t CorrFitMask1D::AreCompatible(TObject* cf) const {
    auto func = dynamic_cast<Femto1DCF*>(cf);
    if (!func) return kFALSE;
    Int_t bins;
    Double_t minim, maxim;
    Std::GetAxisPar(*func->GetNum(), bins, minim, maxim, "x");
    if (fBins != bins) return kFALSE;
    if (fMin != minim) return kFALSE;
    if (fMax != maxim) return kFALSE;
    return kTRUE;
  }

  void CorrFitMask1D::Reset(Bool_t state) {
    for (auto& i : fRawMask) {
      i = state;
    }
  }

  void CorrFitMask1D::ApplyThreshold(const TH1& h, Double_t threshold) {
    for (int i = 1; i <= h.GetNbinsX(); i++) {
      if (h.GetBinContent(i) <= threshold) { fRawMask[i] = false; }
    }
  }

  CorrFitMask1D::CorrFitMask1D(Int_t bins, Double_t min, Double_t max) : fBins(bins), fMin(min), fMax(max) {
    fRawMask.resize(fBins + 2);
    for (auto& i : fRawMask) {
      i = false;
    }
  }

  void CorrFitMask1D::ApplyRange(Double_t min, Double_t max, Bool_t additive) {
    Double_t binW = 1.0 / ((fMax - fMin) / double(fBins));
    int low       = (min - fMin) * binW + 1;
    int high      = (max - fMin) * binW + 1;
    if (low < 0) low = 1;
    if (high > fBins + 1) high = fBins + 1;
    if (additive) {
      for (int i = low; i <= high; i++) {
        fRawMask[i] = true;
      }
    } else {
      for (int i = 0; i < low; i++) {
        fRawMask[i] = false;
      }
      for (int i = high + 1; i < fRawMask.size(); i++) {
        fRawMask[i] = false;
      }
    }
  }

  Bool_t CorrFitMask1D::Init() {
    fActiveBins = 0;
    for (auto i : fRawMask) {
      if (i) fActiveBins++;
    }
    return kTRUE;
  }

  void CorrFitMask1D::SetBin(Int_t bin, Bool_t state) { fRawMask[bin] = state; }

}  // namespace Hal
