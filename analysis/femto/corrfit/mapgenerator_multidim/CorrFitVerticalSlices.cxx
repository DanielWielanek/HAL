/*
 * CorrFitSlices.cxx
 *
 *  Created on: 2 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitVerticalSlices.h"

#include "FemtoPair.h"

namespace Hal {

  CorrFitVerticalSlices::CorrFitVerticalSlices(Int_t paramSpace, Int_t binNo) : fBinNo(binNo) {
    if (paramSpace > 0) {
      fNum.resize(paramSpace);
      fDen.resize(paramSpace);
    }
  }

  CorrFitVerticalSlices::~CorrFitVerticalSlices() {}

  void CorrFitVerticalSlices1D::FillNum(Int_t bin, FemtoPair* pair) { fNum[bin] += pair->GetWeight(); }

  void CorrFitVerticalSlices1D::FillDen(Int_t bin, FemtoPair* pair) { fDen[bin] += pair->GetWeight(); }

  void CorrFitVerticalSlices3D::FillNum(Int_t bin, FemtoPair* pair) { fNum[bin] += pair->GetWeight(); }

  void CorrFitVerticalSlices3D::FillDen(Int_t bin, FemtoPair* pair) { fDen[bin] += pair->GetWeight(); }

  CorrFitVerticalSlicesSH::CorrFitVerticalSlicesSH(Int_t paramSpace, Int_t binNo, Int_t lmax) :
    CorrFitVerticalSlices(paramSpace, binNo), fLmVals(FemtoYlmIndexes(lmax)), fMaxJM((lmax + 1) * (lmax + 1)) {
    auto resize = [&](std::vector<std::vector<Double_t>>& vec) {
      vec.resize(paramSpace);
      for (auto& s : vec)
        s.resize(fMaxJM);
    };
    resize(fShDenReal);
    resize(fShDenImag);
    resize(fShNumReal);
    resize(fShNumImag);
  }

  void CorrFitVerticalSlicesSH::FillNum(Int_t bin, FemtoPair* pair) {
    std::complex<double>* YlmBuffer = fLmMath.YlmUpToL(fLmVals.GetMaxL(), pair->GetX(), pair->GetY(), pair->GetZ());
    for (int ilm = 0; ilm < fMaxJM; ilm++) {
      fShNumReal[bin][ilm] += real(YlmBuffer[ilm]) * pair->GetWeight();
      fShNumImag[bin][ilm] -= imag(YlmBuffer[ilm]) * pair->GetWeight();
      fNum[bin] += pair->GetWeight();
    }
  }

  void CorrFitVerticalSlicesSH::FillDen(Int_t bin, FemtoPair* pair) {
    std::complex<double>* YlmBuffer = fLmMath.YlmUpToL(fLmVals.GetMaxL(), pair->GetX(), pair->GetY(), pair->GetZ());
    for (int ilm = 0; ilm < fMaxJM; ilm++) {
      fShDenReal[bin][ilm] += real(YlmBuffer[ilm]) * pair->GetWeight();
      fShDenImag[bin][ilm] -= imag(YlmBuffer[ilm]) * pair->GetWeight();
      fDen[bin] += pair->GetWeight();
    }
  }

} /* namespace Hal */
