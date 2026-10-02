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
    for (int i = 0; i < fRawMask.GetSize(); i++) {
      fRawMask[i] = (int) state;
    }
  }

  void CorrFitMask1D::ApplyThreshold(const TH1& h, Double_t threshold, ELogic logic) {
    auto mask = fRawMask;
    SetGlobalStatus(mask, 0);
    for (int i = 1; i <= h.GetNbinsX(); i++) {
      if (h.GetBinContent(i) <= threshold) mask[i] = 1;
    }
    Mask(fRawMask, mask, logic);
  }

  CorrFitMask1D::CorrFitMask1D(Int_t bins, Double_t min, Double_t max) : fBins(bins), fMin(min), fMax(max) {
    fRawMask.MakeBigger(fBins + 2);
    SetGlobalStatus(fRawMask, 0);
  }

  void CorrFitMask1D::ApplyRange(Double_t min, Double_t max, ELogic flag) {
    Double_t binW = 1.0 / ((fMax - fMin) / double(fBins));
    int low       = (min - fMin) * binW + 1;
    int high      = (max - fMin) * binW + 1;
    if (low < 0) low = 1;
    if (high > fBins + 1) high = fBins + 1;
    auto map = fRawMask;
    SetGlobalStatus(map, 0);
    for (int i = low; i <= high; i++) {
      map[i] = 1;
    }
    Mask(fRawMask, map, flag);
  }

  Bool_t CorrFitMask1D::Init() {
    fActiveBins = 0;
    for (unsigned int i = 1; i < fRawMask.GetSize() - 1; i++) {
      if (fRawMask[i]) fActiveBins++;
    }

    return kTRUE;
  }

  void CorrFitMask1D::SetBin(Int_t bin, Bool_t state) { fRawMask[bin] = state; }

  CorrFitMask1D::CorrFitMask1D(const Hal::Femto1DCF& cf) :
    CorrFitMask1D(cf.GetNum()->GetNbinsX(),
                  cf.GetNum()->GetXaxis()->GetBinLowEdge(1),
                  cf.GetNum()->GetXaxis()->GetBinUpEdge(cf.GetNum()->GetNbinsX())) {}

}  // namespace Hal
