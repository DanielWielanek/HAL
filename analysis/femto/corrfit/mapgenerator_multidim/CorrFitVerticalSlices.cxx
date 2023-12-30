/*
 * CorrFitSlices.cxx
 *
 *  Created on: 2 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitVerticalSlices.h"

#include "Cout.h"
#include "Femto1DCF.h"
#include "Femto3DCF.h"
#include "FemtoPair.h"
#include "FemtoSHCF.h"
#include "Std.h"

#include <iostream>

namespace Hal {

  void CorrFitVerticalSlices1D::FillNum(Int_t bin, FemtoPair* pair) { fNum[bin] += pair->GetWeight(); }

  void CorrFitVerticalSlices1D::FillDen(Int_t bin, FemtoPair* pair) { fDen[bin] += pair->GetWeight(); }

  void CorrFitVerticalSlices3D::FillNum(Int_t bin, FemtoPair* pair) {  // fNum[bin] += pair->GetWeight();
                                                                       // TODO
  }

  void CorrFitVerticalSlices3D::FillDen(Int_t bin, FemtoPair* pair) {
    // fDen[bin] += pair->GetWeight();  TODO
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

  void CorrFitVerticalSlicesSH::FillNumBuffer(std::complex<double>* shCoord, Double_t weight, Int_t paramBin) {
    for (int ilm = 0; ilm < fMaxJM; ilm++) {
      fShNumReal[paramBin][ilm] += real(shCoord[ilm]) * weight;
      fShNumImag[paramBin][ilm] -= imag(shCoord[ilm]) * weight;
      fNum[paramBin] += weight;
      for (int ilm2 = 0; ilm2 < fMaxJM; ilm2++) {}
    }
    Double_t weight2 = weight * weight;
    for (int ilmzero = 0; ilmzero < fMaxJM; ilmzero++) {
      const int twoilmzero = ilmzero * 2;
      for (int ilmprim = 0; ilmprim < fMaxJM; ilmprim++) {
        const int twoilmprim = ilmprim * 2;
        fCovMatrix[paramBin][twoilmzero][twoilmprim] += real(shCoord[ilmzero]) * real(shCoord[ilmprim]) * weight2;
        fCovMatrix[paramBin][twoilmzero][twoilmprim + 1] += real(shCoord[ilmzero]) * -imag(shCoord[ilmprim]) * weight2;
        fCovMatrix[paramBin][twoilmzero + 1][twoilmprim] -= imag(shCoord[ilmzero]) * real(shCoord[ilmprim]) * weight2;
        fCovMatrix[paramBin][twoilmzero + 1][twoilmprim + 1] -= imag(shCoord[ilmzero]) * -imag(shCoord[ilmprim]) * weight2;
      }
    }
  }

  void CorrFitVerticalSlicesSH::FillDenBuffer(std::complex<double>* shCoord, Double_t weight, Int_t paramBin) {
    for (int ilm = 0; ilm < fMaxJM; ilm++) {
      fShDenReal[paramBin][ilm] += real(shCoord[ilm]) * weight;
      fShDenImag[paramBin][ilm] -= imag(shCoord[ilm]) * weight;
      fDen[paramBin] += weight;
      for (int ilm2 = 0; ilm2 < fMaxJM; ilm2++) {}
    }
  }

  void CorrFitVerticalSlices1D::ExportToFlat(Array_1<Float_t>* array, Int_t paramId, Bool_t first) const {
    if (first) { array->MakeBigger(2); }
    array->Set(0, fNum[paramId]);
    array->Set(1, fDen[paramId]);
  }

  void CorrFitVerticalSlices3D::ExportToFlat(Array_1<Float_t>* array, Int_t paramId, Bool_t first) const {
    if (first) { array->MakeBigger(fOutBins * fSideBins * 2); }
    for (unsigned int iO = 0; iO < fOutBins; iO++) {
      for (unsigned int iS = 0; iS < fSideBins; iS++) {
        array->Set(2 * (fSideBins * iO + iS), fNum[iO][iS][paramId]);
        array->Set(2 * (fSideBins * iO + iS) + 1, fDen[iO][iS][paramId]);
      }
    }
  }

  std::complex<double>* CorrFitVerticalSlicesSH::GetBufferCalc(FemtoPair* pair) {
    return fLmMath.YlmUpToL(fLmVals.GetMaxL(), pair->GetX(), pair->GetY(), pair->GetZ());
  }

  void CorrFitVerticalSlicesSH::ExportToFlat(Array_1<Float_t>* array, Int_t paramId, Bool_t first) const {
    if (first) {
      unsigned int dim = 2 + fMaxJM * 4 + fMaxJM * fMaxJM * 4;
      array->MakeBigger(dim);
    }
    int count = 0;
    array->Set(count++, fNum[paramId]);
    array->Set(count++, fDen[paramId]);
    for (int i = 0; i < fMaxJM; i++) {
      array->Set(count++, fShNumReal[paramId][i]);
      array->Set(count++, fShDenReal[paramId][i]);
      array->Set(count++, fShNumImag[paramId][i]);
      array->Set(count++, fShDenImag[paramId][i]);
    }
    for (int i = 0; i < fMaxJM * 2; i++) {
      for (int j = 0; j < fMaxJM * 2; j++) {
        array->Set(count++, fCovMatrix[paramId][i][j]);
      }
    }
  }

  CorrFitVerticalSlices* CorrFitVerticalSlices::MakeSlice(const Hal::DividedHisto1D& h, Int_t nParams) {
    if (dynamic_cast<const Hal::FemtoSHCF*>(&h)) {
      return new CorrFitVerticalSlicesSH(dynamic_cast<const Hal::FemtoSHCF&>(h), nParams);
    }
    if (dynamic_cast<const Hal::Femto1DCF*>(&h)) {
      return new CorrFitVerticalSlices1D(dynamic_cast<const Hal::Femto1DCF&>(h), nParams);
    }
    if (dynamic_cast<const Hal::Femto3DCF*>(&h)) {
      return new CorrFitVerticalSlices3D(dynamic_cast<const Hal::Femto3DCF&>(h), nParams);
    }
    return nullptr;
  }

  CorrFitVerticalSlices1D::CorrFitVerticalSlices1D(const Hal::Femto1DCF& h, Int_t nSamples) {
    Hal::Std::ResizeVector1D(fNum, nSamples);
    Hal::Std::ResizeVector1D(fDen, nSamples);
  }

  CorrFitVerticalSlices3D::CorrFitVerticalSlices3D(const Hal::Femto3DCF& h, Int_t nSamples) {
    fOutBins     = h.GetNum()->GetNbinsX();
    fSideBins    = h.GetNum()->GetNbinsY();
    fMin[0]      = h.GetNum()->GetXaxis()->GetBinLowEdge(1);
    fMin[1]      = h.GetNum()->GetYaxis()->GetBinLowEdge(1);
    fOverStep[0] = 1.0 / h.GetNum()->GetXaxis()->GetBinWidth(1);
    fOverStep[1] = 1.0 / h.GetNum()->GetYaxis()->GetBinWidth(1);
    Hal::Std::ResizeVector3D(fNum, fOutBins, fSideBins, nSamples);
    Hal::Std::ResizeVector3D(fDen, fOutBins, fSideBins, nSamples);
  }

  CorrFitVerticalSlicesSH::CorrFitVerticalSlicesSH(const Hal::FemtoSHCF& h, Int_t nSamples) :
    fMaxJM((h.GetLMax() + 1) * (h.GetLMax() + 1)) {
    fLmVals = Hal::FemtoYlmIndexes(h.GetLMax());
    Hal::Std::ResizeVector1D(fNum, nSamples);
    Hal::Std::ResizeVector1D(fDen, nSamples);
    Hal::Std::ResizeVector2D(fShNumReal, nSamples, fMaxJM);
    Hal::Std::ResizeVector2D(fShDenReal, nSamples, fMaxJM);
    Hal::Std::ResizeVector2D(fShNumImag, nSamples, fMaxJM);
    Hal::Std::ResizeVector2D(fShDenImag, nSamples, fMaxJM);
    Hal::Std::ResizeVector3D(fCovMatrix, nSamples, fMaxJM * 2, fMaxJM * 2);
  }

  std::pair<int, int> CorrFitVerticalSlices3D::FindBin(FemtoPair* pair) {
    std::pair<int, int> res;
    res.first  = int((pair->GetX() - fMin[0]) * fOverStep[0]);
    res.second = int((pair->GetY() - fMin[1]) * fOverStep[1]);
    return res;
  }

} /* namespace Hal */
