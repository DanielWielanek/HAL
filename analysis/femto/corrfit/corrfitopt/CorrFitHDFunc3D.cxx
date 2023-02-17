/*
 * CorrFitHDFunc3D.cxx
 *
 *  Created on: 12 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitHDFunc3D.h"

#include "CorrFitMask3D.h"

#include <iostream>

namespace Hal {
  CorrFitHDFunc3D::CorrFitHDFunc3D() : CorrFitHDFunc(3) {}

  void CorrFitHDFunc3D::SetMask(const CorrFitMask& mask, TH1* denominator, Bool_t hd) {
    TH3* den3d                  = static_cast<TH3*>(denominator);
    const CorrFitMask3D* mask3d = dynamic_cast<const CorrFitMask3D*>(&mask);
    Int_t maxBins               = TMath::Max(mask3d->GetNbinsX(), TMath::Max(mask3d->GetNbinsY(), mask3d->GetNbinsZ()));
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
    CalculateBinsArrays(*mask3d, hd);
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
          if (!mask3d->GetBinFlag(i, j, k)) continue;
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

  Double_t CorrFitHDFunc3D::GetBinCFVal(Int_t BinX, Int_t BinY, Int_t BinZ, Bool_t extrapolated) const {
    if (extrapolated) {
      Double_t val = 0;
      Int_t xbin0  = GetBinHD(BinX);
      Int_t ybin0  = GetBinHD(BinY);
      Int_t zbin0  = GetBinHD(BinZ);
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

  void CorrFitHDFunc3D::CalculateBinsArrays(const CorrFitMask3D& mask, Bool_t hd) {
    // calculate standard bins----------------------------------------------------
    Int_t nonZeroBins = mask.GetActiveBins();
    if (nonZeroBins != fBinsX.GetSize()) {
      fBinsX.Resize(nonZeroBins);
      fBinsY.Resize(nonZeroBins);
      fBinsZ.Resize(nonZeroBins);
    }
    Array_3<Short_t> tempFlags(mask.GetNbinsX() * 2 + 1, mask.GetNbinsY() * 2 + 1, mask.GetNbinsZ() * 2 + 1);
    Int_t hdBinsNo = 0;
    Int_t binId    = 0;
    for (int i = 1; i <= mask.GetNbinsX(); i++) {
      Int_t binX = GetBinHD(i);
      for (int j = 1; j <= mask.GetNbinsY(); j++) {
        Int_t binY = GetBinHD(j);
        for (int k = 1; k <= mask.GetNbinsZ(); k++) {
          Int_t binZ = GetBinHD(k);
          if (mask.GetBinFlag(i, j, k)) {
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
