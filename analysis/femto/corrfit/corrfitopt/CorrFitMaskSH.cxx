/*
 * CorrFitMaskSH.cxx
 *
 *  Created on: 12 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitMaskSH.h"

#include "FemtoSHCF.h"
#include "StdHist.h"

#include <TH1.h>

namespace Hal {
  CorrFitMaskSH::CorrFitMaskSH(Int_t bins, Double_t min, Double_t max, Int_t maxL) : fL(maxL) {
    fMin     = min;
    fBins    = bins;
    fMax     = max;
    int size = (maxL + 1) * (maxL + 1);
    fRawMaskRe.resize(size);
    fActiveReal.resize(size);
    fActiveImag.resize(size);
    for (auto& i : fRawMaskRe)
      i.resize(bins + 2);
    fRawMaskIm.resize(size * 2);
    for (auto& i : fRawMaskIm)
      i.resize(bins + 2);
  }

  void CorrFitMaskSH::ApplyRange(Double_t min, Double_t max, Bool_t additive) {
    int low = 1 + int(fBins * (min - fMin) / (fMax - fMin));
    if (low < 0) low = 0;
    int high = 1 + int(fBins * (max - fMin) / (fMax - fMin));
    if (max >= high + 1) max = high + 1;
    if (additive) {
      for (auto& i : fRawMaskRe)
        for (int j = low; j <= high; j++)
          i[j] = true;
      for (auto& i : fRawMaskIm)
        for (int j = low; j <= high; j++)
          i[j] = true;
    } else {
      for (auto& i : fRawMaskRe) {
        for (int j = 0; j < low; j++) {
          i[j] = false;
        }
        for (int j = high + 1; j <= fBins + 1; j++) {
          i[j] = false;
        }
      }
      for (auto& i : fRawMaskIm) {
        for (int j = 0; j < low; j++) {
          i[j] = false;
        }
        for (int j = high + 1; j <= fBins + 1; j++) {
          i[j] = false;
        }
      }
    }
  }

  void CorrFitMaskSH::SetBin(Int_t bin, Bool_t state) {
    for (auto& i : fRawMaskRe)
      i[bin] = state;
    for (auto& i : fRawMaskIm)
      i[bin] = state;
  }

  void CorrFitMaskSH::Reset(Bool_t state) {
    fActiveBins = 0;
    for (unsigned int i = 0; i < fActiveReal.size(); i++) {
      fActiveReal[i] = fActiveImag[i] = 0;
      for (auto& j : fRawMaskRe[i]) {
        j = state;
      }
      for (auto& j : fRawMaskIm[i]) {
        j = state;
      }
    }
  }

  Bool_t CorrFitMaskSH::Init() {
    fActiveBins = 0;
    for (auto& bin : fActiveImag) {
      bin = 0;  // reset for l=0 and imag
    }
    for (auto& bin : fActiveReal) {
      bin = 0;  // reset for l=0 and imag
    }

    for (unsigned int i = 0; i < fRawMaskRe.size(); i++) {
      fActiveImag[i] = 0;
      fActiveReal[i] = 0;
      int count      = -1;
      for (auto& bin : fRawMaskRe[i]) {
        ++count;
        if (bin) {
          fActiveBins++;
          fActiveReal[i]++;
        }
      }
      count = -1;
      for (auto& bin : fRawMaskIm[i]) {
        ++count;
        if (bin) {
          fActiveBins++;
          fActiveImag[i]++;
        }
      }
    }
    return kTRUE;
  }

  Bool_t CorrFitMaskSH::AreCompatible(TObject* cf) const {
    FemtoSHCF* sh = dynamic_cast<FemtoSHCF*>(cf);
    if (sh == nullptr) return kFALSE;
    Int_t bins;
    Double_t min, max;
    Hal::Std::GetAxisPar(*sh->GetNum(), bins, min, max);
    if (bins != fBins) return kFALSE;
    if (max != fMax) return kFALSE;
    if (min != fMin) return kFALSE;
    if (sh->GetL() != fL) return kFALSE;
    return kTRUE;
  }

  Int_t CorrFitMaskSH::GetActiveBins(Int_t L, Int_t M, Bool_t real) const {
    if (real) return fActiveReal[fMath.GetIndex(L, M)];
    return fActiveImag[fMath.GetIndex(L, M)];
  }

  const std::vector<Short_t>& CorrFitMaskSH::GetMaskArray(Int_t L, Int_t M, Bool_t real) const {
    if (real) {
      return fRawMaskRe[fMath.GetIndex(L, M)];
    } else {
      return fRawMaskIm[fMath.GetIndex(L, M)];
    }
  }

  void CorrFitMaskSH::ApplyThreshold(const TH1& h, Double_t threshold) {
    for (int i = 1; i <= h.GetNbinsX(); i++) {
      if (h.GetBinContent(i) <= threshold) { SetBin(i, false); }
    }
  }

  void CorrFitMaskSH::SetBin(Int_t L, Int_t M, Int_t bin, Bool_t state) {
    int index              = fMath.GetIndex(L, M);
    fRawMaskRe[index][bin] = state;
    fRawMaskIm[index][bin] = state;
  }

  void CorrFitMaskSH::SetBinRe(Int_t L, Int_t M, Int_t bin, Bool_t state) {
    int index              = fMath.GetIndex(L, M);
    fRawMaskRe[index][bin] = state;
  }

  void CorrFitMaskSH::SetBinIm(Int_t L, Int_t M, Int_t bin, Bool_t state) {
    int index              = fMath.GetIndex(L, M);
    fRawMaskIm[index][bin] = state;
  }

}  // namespace Hal
