/*
 * CorrFitHDFunc1Da.cpp
 *
 *  Created on: 12 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitHDFunc1D.h"

namespace Hal {
  CorrFitHDFunc1D::CorrFitHDFunc1D() : CorrFitHDFunc(1) {}

  void CorrFitHDFunc1D::SetMask(const CorrFitMask& mask, TH1* denominator, Bool_t hd) {
    const CorrFitMask1D* mask1d = dynamic_cast<const CorrFitMask1D*>(&mask);
    Int_t maxBins               = mask1d->GetNBins() * 2 + 1;
    fDenominatorHD.Resize(maxBins);                        //+1 to keep compatible with bin id
    fDenominatorSum.Resize(denominator->GetNbinsX() + 1);  //+1 to keep compatible with bin id
    fMapHD.Resize(maxBins);
    fMins[0]  = denominator->GetXaxis()->GetBinLowEdge(1);
    fSteps[0] = denominator->GetXaxis()->GetBinWidth(1) * 0.5;
    RecalcHDBin(mask1d->GetNBins());
    CalculateBinsArrays(*mask1d, hd);
    if (hd == kFALSE) {
      for (int i = 0; i < fBins.GetSize(); i++) {
        Int_t bin                     = fBins[i];
        fDenominatorHD[GetBinHD(bin)] = denominator->GetBinContent(bin);
        fDenominatorSum[bin]          = 1;
      }
      return;
    }
    // calculate flags for HD bins && no of bins;

    Spline1D spline(denominator, denominator->GetBinContent(1), denominator->GetBinContent(denominator->GetNbinsX()));
    spline.Refit();
    for (int i = 0; i < fBinsHD.GetSize(); i++) {
      Double_t bin        = fBinsHD[i];
      Double_t x          = EvalHD(bin);
      fDenominatorHD[bin] = spline.Eval(x);
    }
    for (int i = 1; i < fDenominatorSum.GetSize(); i++) {
      if (!mask1d->GetBinFlag(i)) continue;
      Int_t bin     = GetBinHD(i);
      Double_t prev = fDenominatorHD[bin - 1];
      Double_t curr = fDenominatorHD[bin];
      Double_t next = fDenominatorHD[bin + 1];
      Double_t sum  = prev + curr + next;
      if (sum == 0) {
        fDenominatorSum[i] = 0;
      } else {
        fDenominatorSum[i] = 1.0 / sum;
      }
    }
  }

  Double_t CorrFitHDFunc1D::GetBinCFVal(Int_t BinX, Bool_t extrapolated) const {
    Int_t bin = GetBinHD(BinX);
    if (extrapolated) {
      // CF *weight
      Double_t low   = fMapHD.Get(bin - 1) * fDenominatorHD.Get(bin - 1);
      Double_t med   = fMapHD.Get(bin) * fDenominatorHD.Get(bin);
      Double_t high  = fMapHD.Get(bin + 1) * fDenominatorHD.Get(bin + 1);
      Double_t scale = fDenominatorSum.Get(BinX);
      return scale * (low + med + high);
    } else {
      return fMapHD.Get(bin);
    }
  }

  void CorrFitHDFunc1D::CalculateBinsArrays(const CorrFitMask1D& mask, Bool_t hd) {
    Int_t nonZero = mask.GetActiveBins();
    if (nonZero != fBins.GetSize()) fBins.Resize(nonZero);
    Int_t binId    = 0;
    Int_t hdBinsNo = 0;
    Array_1<Short_t> tempFlags(mask.GetNBins() * 2 + 1);
    for (int i = 1; i <= mask.GetNBins(); i++) {
      if (mask.GetBinFlag(i)) {
        fBins[binId++] = i;
        Int_t binHd    = GetBinHD(i);
        if (!hd) {
          ++hdBinsNo;
          tempFlags[binHd] = 1;
        } else {
          for (int a = -1; a <= -1; a++) {
            if (tempFlags[binHd + a] == 0) {
              ++hdBinsNo;
              tempFlags[binHd + a] = 1;
            }
          }
        }
      }
    }
    fBinsHD.Resize(hdBinsNo);
    Int_t hdBinsId = 0;
    for (int i = 0; i < tempFlags.GetSize(); i++) {
      if (tempFlags[i] == 1) fBinsHD[hdBinsId++] = i;
    }
  }

  CorrFitHDFunc1D::~CorrFitHDFunc1D() {}
}  // namespace Hal
