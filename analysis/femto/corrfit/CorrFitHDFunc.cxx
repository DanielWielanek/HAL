/*
 * CorrFitTempMaps.cxx
 *
 *  Created on: 14 lut 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitHDFunc.h"

#include "Cout.h"

#include <iostream>

//#define DEBUG_FUNCHD
namespace Hal {
  CorrFitHDFunc::CorrFitHDFunc(Int_t dim) : fDummyVal(-100.01), fDim(dim) {
    fParams = new Double_t[fDim];
    fMins   = new Double_t[fDim];
    fSteps  = new Double_t[fDim];
  }

  CorrFitHDFunc::~CorrFitHDFunc() {
    delete[] fParams;
    delete[] fMins;
    delete[] fSteps;
  }

  CorrFitHDFunc1D::CorrFitHDFunc1D() : CorrFitHDFunc(1) {}


  void CorrFitHDFunc::RecalcHDBin(Int_t newSize) {
    newSize     = newSize * 2 + 1;
    Int_t count = 1;
    if (fHDToBin.GetSize() < newSize) fHDToBin.MakeBigger(newSize);
    fHDToBin[0] = 0;
    for (int i = 1; i < fHDToBin.GetSize(); i++) {
      fHDToBin[i] = count;
      if (i % 2 == 0) count++;
    }
  }
  void CorrFitHDFunc1D::SetMask(TH1* mask, TH1* denominator, Bool_t hd) {
    Int_t maxBins = mask->GetNbinsX() * 2 + 1;
    fDenominatorHD.Resize(maxBins);                        //+1 to keep compatible with bin id
    fDenominatorSum.Resize(denominator->GetNbinsX() + 1);  //+1 to keep compatible with bin id
    fMapHD.Resize(maxBins);
    fMins[0]  = denominator->GetXaxis()->GetBinLowEdge(1);
    fSteps[0] = denominator->GetXaxis()->GetBinWidth(1) * 0.5;
    RecalcHDBin(mask->GetNbinsX());
    CalculateBinsArrays(mask, hd);
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
      if (mask->GetBinContent(i) == 0) continue;
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

  CorrFitHDFunc1D::~CorrFitHDFunc1D() {}

  CorrFitHDFunc3D::CorrFitHDFunc3D() : CorrFitHDFunc(3) {}

  void CorrFitHDFunc3D::SetMask(TH1* mask, TH1* denominator, Bool_t hd) {
    TH3* mask3d = static_cast<TH3*>(mask);
    TH3* den3d  = static_cast<TH3*>(denominator);

    Int_t maxBins = TMath::Max(mask3d->GetNbinsX(), TMath::Max(mask->GetNbinsY(), mask->GetNbinsZ()));
    fDenominatorHD.MakeBigger(
      den3d->GetNbinsX() * 2 + 1, den3d->GetNbinsY() * 2 + 1, den3d->GetNbinsZ() * 2 + 1);  //+1 to keep compatible with bin id
    fDenominatorSum.MakeBigger(
      den3d->GetNbinsX() + 1, den3d->GetNbinsY() + 1, den3d->GetNbinsZ() + 1);  //+1 to keep compatible with bin id
    fMapHD.MakeBigger(den3d->GetNbinsX() * 2 + 1, den3d->GetNbinsY() * 2 + 1, den3d->GetNbinsZ() * 2 + 1);
    fMins[0]  = den3d->GetXaxis()->GetBinLowEdge(1);
    fMins[1]  = den3d->GetYaxis()->GetBinLowEdge(1);
    fMins[2]  = den3d->GetZaxis()->GetBinLowEdge(1);
    fSteps[0] = den3d->GetXaxis()->GetBinWidth(1) * 0.5;
    fSteps[1] = den3d->GetYaxis()->GetBinWidth(1) * 0.5;
    fSteps[2] = den3d->GetZaxis()->GetBinWidth(1) * 0.5;
    RecalcHDBin(maxBins);
    CalculateBinsArrays(mask, hd);
    if (hd == kFALSE) {
      for (int i = 0; i < fBinsX.GetSize(); i++) {
        Int_t binX                                                     = fBinsX[i];
        Int_t binY                                                     = fBinsY[i];
        Int_t binZ                                                     = fBinsZ[i];
        fDenominatorHD[GetBinHD(binX)][GetBinHD(binY)][GetBinHD(binZ)] = den3d->GetBinContent(binX, binY, binZ);
        fDenominatorSum[binX][binY][binZ]                              = 1;
      }
      return;
    }

    // calculate flags for HD bins && no of bins;


    Spline3D spline(den3d, "yes");
    std::cout << "REFIT START" << std::endl;
    spline.Refit();
    std::cout << "REFIT ENDL" << std::endl;
    for (int i = 0; i < fBinsHDX.GetSize(); i++) {
      Double_t binX                    = fBinsHDX[i];
      Double_t x                       = EvalHDX(binX);
      Double_t binY                    = fBinsHDY[i];
      Double_t y                       = EvalHDY(binY);
      Double_t binZ                    = fBinsHDZ[i];
      Double_t z                       = EvalHDZ(binZ);
      fDenominatorHD[binX][binY][binZ] = spline.Eval(x, y, z);
    }
    Int_t sizeX = fDenominatorSum.GetSize();
    Int_t sizeY = fDenominatorSum[0].GetSize();
    Int_t sizeZ = fDenominatorSum[0][0].GetSize();
    for (int i = 1; i < sizeX; i++) {
      Int_t binX = GetBinHD(i);
      for (int j = 1; j < sizeY; j++) {
        Int_t binY = GetBinHD(j);
        for (int k = 1; k < sizeZ; k++) {
          Int_t binZ = GetBinHD(k);
          if (mask->GetBinContent(i, j, k) == 0) continue;
          Double_t sum = 0;
          for (int a = -1; a < 2; a++) {
            for (int b = -1; b < 2; b++) {
              for (int c = -1; c < 2; c++) {
                sum += fDenominatorHD[binX + a][binY + b][binZ + c];
              }
            }
          }
          if (sum == 0)
            fDenominatorSum[i][j][k] = 0;
          else
            fDenominatorSum[i][j][k] = 1. / sum;
        }
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

  Double_t CorrFitHDFunc3D::GetBinCFVal(Int_t BinX, Int_t BinY, Int_t BinZ, Bool_t extrapolated) const {
    if (extrapolated) {
      Double_t val = 0;
      Int_t xbin0  = GetBinHD(BinX);
      Int_t ybin0  = GetBinHD(BinY);
      Int_t zbin0  = GetBinHD(BinZ);
      Double_t X[3];
      for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
          for (int k = -1; k < 2; k++) {
            val += fDenominatorHD.Get(xbin0 + i, ybin0 + j, zbin0 + k) * fMapHD.Get(xbin0 + i, ybin0 + j, zbin0 + k);
          }
        }
      }
      return val * fDenominatorSum.Get(BinX, BinY, BinZ);
    }
    return fMapHD.Get(GetBinHD(BinX), GetBinHD(BinY), GetBinHD(BinZ));
  }

  Int_t CorrFitHDFunc::CalcBinNonZero(TH1* h) const {
    Int_t sum = 0;
    if (dynamic_cast<TH3*>(h) != nullptr) {
      TH3* h3 = (TH3*) h;
      for (int i = 1; i <= h3->GetNbinsX(); i++) {
        for (int j = 1; j <= h3->GetNbinsY(); j++) {
          for (int k = 1; k <= h3->GetNbinsZ(); k++) {
            if (h3->GetBinContent(i, j, k) != 0) sum++;
          }
        }
      }
    } else {
      for (int i = 1; i <= h->GetNbinsX(); i++) {
        if (h->GetBinContent(i) != 0) sum++;
      }
    }
    return sum;
  }

  void CorrFitHDFunc1D::CalculateBinsArrays(TH1* mask, Bool_t hd) {
    Int_t nonZero = CalcBinNonZero(mask);
    if (nonZero != fBins.GetSize()) fBins.Resize(nonZero);
    Int_t binId    = 0;
    Int_t hdBinsNo = 0;
    Array_1<Short_t> tempFlags(mask->GetNbinsX() * 2 + 1);
    for (int i = 1; i <= mask->GetNbinsX(); i++) {
      if (mask->GetBinContent(i) != 0) {
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

  void CorrFitHDFunc3D::CalculateBinsArrays(TH1* mask, Bool_t hd) {
    // calculate standard bins----------------------------------------------------
    TH3* mask3d       = (TH3*) mask;
    Int_t nonZeroBins = CalcBinNonZero(mask);
    if (nonZeroBins != fBinsX.GetSize()) {
      fBinsX.Resize(nonZeroBins);
      fBinsY.Resize(nonZeroBins);
      fBinsZ.Resize(nonZeroBins);
    }

    Array_3<Short_t> tempFlags(mask3d->GetNbinsX() * 2 + 1, mask3d->GetNbinsY() * 2 + 1, mask3d->GetNbinsZ() * 2 + 1);
    Int_t hdBinsNo = 0;
    Int_t binId    = 0;
    for (int i = 1; i <= mask3d->GetNbinsX(); i++) {
      Int_t binX = GetBinHD(i);
      for (int j = 1; j <= mask3d->GetNbinsY(); j++) {
        Int_t binY = GetBinHD(j);
        for (int k = 1; k <= mask3d->GetNbinsZ(); k++) {
          Int_t binZ = GetBinHD(k);
          if (mask3d->GetBinContent(i, j, k) != 0) {
            fBinsX[binId]   = i;
            fBinsY[binId]   = j;
            fBinsZ[binId++] = k;
            if (hd) {
              for (int a = -1; a < 2; a++) {
                for (int b = -1; b < 2; b++) {
                  for (int c = -1; c < 2; c++) {
                    // empty bin that will be filled
                    if (tempFlags[binX + a][binY + b][binZ + c] == 0) {
                      hdBinsNo++;
                      tempFlags[binX + a][binY + b][binZ + c] = 1;
                    }
                  }
                }
              }
            } else {
              hdBinsNo++;
              tempFlags[binX][binY][binZ] = 1;
            }
          }
        }
      }
    }
    // calculate hd bins-------------------------------------------------------

    fBinsHDX.Resize(hdBinsNo);
    fBinsHDY.Resize(hdBinsNo);
    fBinsHDZ.Resize(hdBinsNo);
    Int_t hdBinNo = 0;
    for (int i = 0; i < tempFlags.GetSize(); i++) {
      for (int j = 0; j < tempFlags[0].GetSize(); j++) {
        for (int k = 0; k < tempFlags[0][0].GetSize(); k++) {
          if (tempFlags[i][j][k] == 1) {
            fBinsHDX[hdBinNo] = i;
            fBinsHDY[hdBinNo] = j;
            fBinsHDZ[hdBinNo] = k;
            ++hdBinNo;
          }
        }
      }
    }
  }
}  // namespace Hal
