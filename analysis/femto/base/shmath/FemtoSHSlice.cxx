/*
 * FemtoSHSlice.cxx
 *
 *  Created on: 24 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSHSlice.h"

#include "FemtoSHCF.h"

#include <TH1D.h>
#include <iostream>

namespace Hal {

  FemtoSHSlice::FemtoSHSlice(Int_t L) : fMaxJM((L + 1) * (L + 1)) {
    fCFImag.resize(fMaxJM);
    fCFReal.resize(fMaxJM);
    fCFImagE.resize(fMaxJM);
    fCFRealE.resize(fMaxJM);
    fNumReal.resize(fMaxJM);
    fNumImag.resize(fMaxJM);
    fDenReal.resize(fMaxJM);
    fDenImag.resize(fMaxJM);
    fNumRealE.resize(fMaxJM);
    fNumImagE.resize(fMaxJM);
    fDenRealE.resize(fMaxJM);
    fDenImagE.resize(fMaxJM);
    fCovCF.MakeBigger(fMaxJM * 2, fMaxJM * 2);
    fCovNum.MakeBigger(fMaxJM * 2, fMaxJM * 2);
    fCovDen.MakeBigger(fMaxJM * 2, fMaxJM * 2);
  }

  void FemtoSHSlice::BuildSlice(FemtoSHCF& cf, Int_t bin) {
    for (int i = 0; i < fMaxJM; i++) {
      fNumReal[i]  = cf.fNumReal[i]->GetBinContent(bin);
      fDenReal[i]  = cf.fDenReal[i]->GetBinContent(bin);
      fNumImag[i]  = cf.fNumImag[i]->GetBinContent(bin);
      fDenImag[i]  = cf.fDenImag[i]->GetBinContent(bin);
      fNumRealE[i] = cf.fNumReal[i]->GetBinError(bin);
      fDenRealE[i] = cf.fDenReal[i]->GetBinError(bin);
      fNumImagE[i] = cf.fNumImag[i]->GetBinError(bin);
      fDenImagE[i] = cf.fDenImag[i]->GetBinError(bin);
    }

    for (int ilmzero = 0; ilmzero < 2 * fMaxJM; ilmzero++)
      for (int ilmprim = 0; ilmprim < 2 * fMaxJM; ilmprim++) {
        fCovNum[ilmzero][ilmprim] = cf.fCovNum[bin - 1][ilmzero][ilmprim];
      }
  }

  FemtoSHSlice::~FemtoSHSlice() {}

} /* namespace Hal */
