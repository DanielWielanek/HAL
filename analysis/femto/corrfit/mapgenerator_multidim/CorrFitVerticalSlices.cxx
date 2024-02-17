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
      Double_t wRe = real(YlmBuffer[ilm]) * pair->GetWeight();
      Double_t wIm = imag(YlmBuffer[ilm]) * pair->GetWeight();
      fShNumReal[bin][ilm] += wRe;
      fShNumImag[bin][ilm] -= wIm;
      fShNumRealE[bin][ilm] += wRe * wRe;
      fShNumImagE[bin][ilm] += wIm * wIm;
      fNum[bin] += pair->GetWeight();
    }
  }

  void CorrFitVerticalSlicesSH::FillDen(Int_t bin, FemtoPair* pair) {
    std::complex<double>* YlmBuffer = fLmMath.YlmUpToL(fLmVals.GetMaxL(), pair->GetX(), pair->GetY(), pair->GetZ());
    for (int ilm = 0; ilm < fMaxJM; ilm++) {
      Double_t wRe = real(YlmBuffer[ilm]) * pair->GetWeight();
      Double_t wIm = imag(YlmBuffer[ilm]) * pair->GetWeight();
      fShDenReal[bin][ilm] += wRe;
      fShDenImag[bin][ilm] -= wIm;
      fShDenRealE[bin][ilm] += wRe * wRe;
      fShDenImagE[bin][ilm] += wIm * wIm;
      fDen[bin] += pair->GetWeight();
    }
  }

  void CorrFitVerticalSlicesSH::FillNumBuffer(std::complex<double>* shCoord, Double_t weight, Int_t paramBin) {
    for (int ilm = 0; ilm < fMaxJM; ilm++) {
      Double_t wRe = real(shCoord[ilm]) * weight;
      Double_t wIm = imag(shCoord[ilm]) * weight;
      fShNumReal[paramBin][ilm] += wRe;
      fShNumImag[paramBin][ilm] -= wIm;
      fShNumRealE[paramBin][ilm] += wRe * wRe;
      fShNumImagE[paramBin][ilm] += wIm * wIm;
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
      Double_t wRe = real(shCoord[ilm]) * weight;
      Double_t wIm = imag(shCoord[ilm]) * weight;
      fShDenReal[paramBin][ilm] += wRe;
      fShDenImag[paramBin][ilm] -= wIm;
      fShDenRealE[paramBin][ilm] += wRe * wRe;
      fShDenImagE[paramBin][ilm] += wIm * wIm;
      fDen[paramBin] += weight;
      for (int ilm2 = 0; ilm2 < fMaxJM; ilm2++) {}
    }
  }

  std::complex<double>* CorrFitVerticalSlicesSH::GetBufferCalc(FemtoPair* pair) {
    return fLmMath.YlmUpToL(fLmVals.GetMaxL(), pair->GetX(), pair->GetY(), pair->GetZ());
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
    Hal::Std::ResizeVector2D(fShNumRealE, nSamples, fMaxJM);
    Hal::Std::ResizeVector2D(fShDenRealE, nSamples, fMaxJM);
    Hal::Std::ResizeVector2D(fShNumImagE, nSamples, fMaxJM);
    Hal::Std::ResizeVector2D(fShDenImagE, nSamples, fMaxJM);
    Hal::Std::ResizeVector3D(fCovMatrix, nSamples, fMaxJM * 2, fMaxJM * 2);
  }

  std::pair<int, int> CorrFitVerticalSlices3D::FindBin(FemtoPair* pair) {
    std::pair<int, int> res;
    res.first  = int((pair->GetX() - fMin[0]) * fOverStep[0]);
    res.second = int((pair->GetY() - fMin[1]) * fOverStep[1]);
    return res;
  }

  void CorrFitVerticalSlicesSH::Print(Option_t* option) const {
    TString opt = option;
    auto vec    = Hal::Std::ExplodeString(opt, '+');
    Int_t pos   = vec[0].Atoi();
    TString secondOpt;
    if (vec.size() == 2) secondOpt = vec[1];
    if (secondOpt == "short") {
      std::cout << "NUM " << fNum[pos] << std::endl;
      std::cout << "DEN " << fDen[pos] << std::endl;
    } else {
      std::cout << "Pos " << pos << std::endl;
      std::cout << "NUM " << fNum[pos] << std::endl;
      std::cout << "DEN " << fDen[pos] << std::endl;
      auto printing = [](const std::vector<Double_t>& arr, TString label) {
        std::cout << label << std::endl;
        for (auto val : arr) {
          std::cout << " " << val << std::endl;
        }
      };
      printing(fShNumReal[pos], "NumRe");
      printing(fShNumImag[pos], "NumIm");
      printing(fShDenReal[pos], "DenRe");
      printing(fShDenImag[pos], "DenIm");
    }
  }

} /* namespace Hal */
