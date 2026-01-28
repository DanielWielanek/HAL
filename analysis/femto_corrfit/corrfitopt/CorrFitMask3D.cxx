/*
 * CorrFitMask3D.cxx
 *
 *  Created on: 12 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitMask3D.h"

#include "Cout.h"
#include "Femto3DCF.h"
#include "StdHist.h"

#include <Rtypes.h>
#include <TH1.h>
#include <TString.h>

namespace Hal {
  Bool_t CorrFitMask3D::Init() {
    fActiveBins = 0;
    for (int i = 1; i < (int) fRawMask.size() - 1; i++) {  // dont take into account overflow/underflow bins
      for (int j = 1; j < (int) fRawMask[0].size() - 1; j++) {
        for (int k = 1; k < (int) fRawMask[0][0].size() - 1; k++) {
          if (fRawMask[i][j][k]) fActiveBins++;
        }
      }
    }
    return kTRUE;
  }

  Bool_t CorrFitMask3D::AreCompatible(TObject* cf) const {
    auto func = dynamic_cast<Femto3DCF*>(cf);
    if (!func) return kFALSE;

    TString opts[3] = {"x", "y", "z"};
    for (int i = 0; i < 3; i++) {
      Int_t bins;
      Double_t minim, maxim;
      Std::GetAxisPar(*func->GetNum(), bins, minim, maxim, opts[i]);
      if (fBins[i] != bins) return kFALSE;
      if (fMin[i] != minim) return kFALSE;
      if (fMax[i] != maxim) return kFALSE;
    }
    return kTRUE;
  }


  CorrFitMask3D::CorrFitMask3D(Int_t binsX,
                               Double_t minX,
                               Double_t maxX,
                               Int_t binsY,
                               Double_t minY,
                               Double_t maxY,
                               Int_t binsZ,
                               Double_t minZ,
                               Double_t maxZ) {
    fBins[0] = binsX;
    fBins[1] = binsY;
    fBins[2] = binsZ;
    fMin[0]  = minX;
    fMin[1]  = minY;
    fMin[2]  = minZ;
    fMax[0]  = maxX;
    fMax[1]  = maxY;
    fMax[2]  = maxZ;
    fRawMask.resize(fBins[0] + 2);
    for (auto& i : fRawMask) {
      i.resize(fBins[1] + 2);
      for (auto& j : i) {
        j.resize(fBins[2] + 2);
      }
    }
  }

  void CorrFitMask3D::ApplyRange(Double_t minX,
                                 Double_t maxX,
                                 Double_t minY,
                                 Double_t maxY,
                                 Double_t minZ,
                                 Double_t maxZ,
                                 Bool_t additive) {
    int low[3], high[3];
    double min[3] = {minX, minY, minZ};
    double max[3] = {maxX, maxY, maxZ};
    for (int i = 0; i < 3; i++) {
      Double_t binW = 1.0 / ((fMax[i] - fMin[i]) / double(fBins[i]));
      low[i]        = (min[i] - fMin[i]) * binW;
      high[i]       = (max[i] - fMin[i]) * binW;
      if (low[i] < 0) low[i] = 1;
      if (high[i] > fBins[i] + 1) high[i] = fBins[i] + 1;
    }
    if (additive) {
      for (int i = low[0]; i <= high[0]; i++) {
        for (int j = low[1]; j <= high[1]; j++) {
          for (int k = low[2]; k <= high[2]; k++) {
            fRawMask[i][j][k] = true;
          }
        }
      }
    } else {
      for (int i = 0; i < (int) fRawMask.size(); i++) {
        for (int j = 0; j < (int) fRawMask[0].size(); j++) {
          for (int k = 0; k < (int) fRawMask[0][0].size(); k++) {
            if (i < low[0]) fRawMask[i][j][k] = false;
            if (i > high[0]) fRawMask[i][j][k] = false;
            if (j < low[1]) fRawMask[i][j][k] = false;
            if (j > high[1]) fRawMask[i][j][k] = false;
            if (k < low[2]) fRawMask[i][j][k] = false;
            if (k > high[2]) fRawMask[i][j][k] = false;
          }
        }
      }
    }
  }

  void CorrFitMask3D::SetBin(Int_t binX, Int_t binY, Int_t binZ, Bool_t state) { fRawMask[binX][binY][binZ] = state; }

  void CorrFitMask3D::Reset(Bool_t state) {
    for (int i = 0; i < (int) fRawMask.size(); i++) {
      for (int j = 0; j < (int) fRawMask[0].size(); j++) {
        for (int k = 0; k < (int) fRawMask[0][0].size(); k++) {
          fRawMask[i][j][k] = state;
        }
      }
    }
  }

  void CorrFitMask3D::ApplyThreshold(const TH1& h, Double_t threshold) {
    for (int i = 1; i <= h.GetNbinsX(); i++) {
      for (int j = 1; j <= h.GetNbinsY(); j++) {
        for (int k = 1; k <= h.GetNbinsZ(); k++) {
          if (h.GetBinContent(i, j, k) <= threshold) { fRawMask[i][j][k] = false; }
        }
      }
    }
  }

  void CorrFitMask3D::ApplyMask(EFitExtraMask mask, Bool_t additive) {
    Array_3<Short_t> map;
    map.MakeBigger(GetNbinsX() + 1, GetNbinsY() + 1, GetNbinsZ() + 1);
    for (int i = 0; i <= GetNbinsX() + 1; i++) {
      for (int j = 0; j <= GetNbinsY() + 1; j++) {
        for (int k = 0; k <= GetNbinsZ() + 1; k++) {
          map[i][j][k] = false;
        }
      }
    }
    CalcMap(map, mask);
    for (int i = 0; i <= GetNbinsX() + 1; i++) {
      for (int j = 0; j <= GetNbinsY() + 1; j++) {
        for (int k = 0; k <= GetNbinsZ() + 1; k++) {
          if (additive) {
            if (map[i][j][k]) fRawMask[i][j][k] = true;
          } else {
            if (!map[i][j][k]) fRawMask[i][j][k] = false;
          }
        }
      }
    }
  }

  void CorrFitMask3D::CalcMap(Array_3<Short_t>& map, EFitExtraMask mask) {
    if (mask == EFitExtraMask::kSlice) {
      CalculateSliceBins(map);
    } else if (mask == EFitExtraMask::kDiagonalSlice || mask == EFitExtraMask::kDiagonalSliceIgnored) {
      CalculateDiagonalBins(map, mask);
    } else if (mask == EFitExtraMask::kUltraDiagonalSlice || mask == EFitExtraMask::kUltraDiagonalSliceIgnored) {
      CalculateUltradiagonalBins(map, mask);
    }
  }

  void CorrFitMask3D::CalculateDiagonalBins(Array_3<Short_t>& map, EFitExtraMask mask) {
    Int_t nBinX    = GetNbinsX();
    Int_t nBinY    = GetNbinsY();
    Int_t nBinZ    = GetNbinsZ();
    Double_t x     = 0;
    Int_t middle_x = 1 + int(nBinX * (x - fMin[0]) / (fMax[0] - fMin[0]));
    // Int_t middle_y = 1 + int(nBinX * (x - fMin[1]) / (fMax[1] - fMin[1]));
    // Int_t middle_z = 1 + int(nBinX * (x - fMin[2]) / (fMax[2] - fMin[2]));
    CalculateSliceBins(map);

    if (nBinX != nBinY || nBinY != nBinZ) {
      Cout::Text(
        "CorrFit3DCF::CalculateDiagonalBins cannot use cross bins, different number of bins in 3D, switch  to slice", "M", kRed);
      return;
    }

    // first diagonal with non-egde histo
    if (middle_x != 1 || mask == EFitExtraMask::kDiagonalSliceIgnored) {
      for (int i = 1; i <= nBinX; i++) {
        Int_t a = i;
        Int_t b = nBinX - i + 1;
        // first diagonal
        map[a][a][a] = true;
        map[a][a][b] = true;
        map[a][b][a] = true;
        map[a][b][b] = true;
      }
    } else {
      // diagonal with edges
      for (int i = 1; i <= nBinX; i++) {
        Int_t a = i;
        // first diagonal
        map[a][a][a] = true;
      }
    }
  }

  void CorrFitMask3D::CalculateUltradiagonalBins(Array_3<Short_t>& map, EFitExtraMask mask) {
    Int_t nBinX    = GetNbinsX();
    Int_t nBinY    = GetNbinsY();
    Int_t nBinZ    = GetNbinsZ();
    Double_t x     = 0;
    Int_t middle_x = 1 + int(nBinX * (x - fMin[0]) / (fMax[0] - fMin[0]));
    Int_t middle_y = 1 + int(nBinX * (x - fMin[1]) / (fMax[1] - fMin[1]));
    Int_t middle_z = 1 + int(nBinX * (x - fMin[2]) / (fMax[2] - fMin[2]));
    CalculateSliceBins(map);

    if (nBinX != nBinY || nBinY != nBinZ) {
      Cout::Text(
        "CorrFit3DCF::CalculateUltradiagonalBins cannot use cross bins, different number of bins in 3D, switch  to slice",
        "M",
        kRed);

      return;
    }

    if (middle_x != 1 || mask == EFitExtraMask::kUltraDiagonalSliceIgnored) {
      for (int i = 1; i <= nBinX; i++) {
        Int_t a = i;
        Int_t b = nBinX - i + 1;
        // diagonals
        map[a][a][a] = true;
        map[a][a][b] = true;
        map[a][b][a] = true;
        map[a][b][b] = true;

        map[middle_x][a][a] = true;
        map[middle_x][a][b] = true;
        map[a][middle_y][a] = true;
        map[a][middle_y][b] = true;
        map[a][a][middle_z] = true;
        map[a][b][middle_z] = true;
      }
    } else {
      for (int i = 1; i <= nBinX; i++) {

        Int_t a = i;
        // Int_t b = nBinX - i + 1;
        // diagonals
        map[a][a][a]        = true;
        map[middle_x][a][a] = true;
        map[a][middle_y][a] = true;
        map[a][a][middle_z] = true;
      }
    }
  }

  void CorrFitMask3D::CalculateSliceBins(Array_3<Short_t>& map) {
    Int_t nBinX    = GetNbinsX();
    Int_t nBinY    = GetNbinsY();
    Int_t nBinZ    = GetNbinsZ();
    Double_t x     = 0;
    Int_t middle_x = 1 + int(nBinX * (x - fMin[0]) / (fMax[0] - fMin[0]));
    Int_t middle_y = 1 + int(nBinX * (x - fMin[1]) / (fMax[1] - fMin[1]));
    Int_t middle_z = 1 + int(nBinX * (x - fMin[2]) / (fMax[2] - fMin[2]));
    for (int i = 1; i <= nBinX; i++) {
      map[i][middle_y][middle_z] = true;
    }
    for (int j = 1; j <= nBinY; j++) {
      map[middle_x][j][middle_z] = true;
    }
    for (int k = 1; k <= nBinZ; k++) {
      map[middle_x][middle_y][k] = true;
    }
  }

  CorrFitMask3D::CorrFitMask3D(const Hal::Femto3DCF& h) :
    CorrFitMask3D(h.GetNum()->GetNbinsX(),
                  h.GetNum()->GetXaxis()->GetBinLowEdge(1),
                  h.GetNum()->GetXaxis()->GetBinUpEdge(h.GetNum()->GetNbinsX()),
                  h.GetNum()->GetNbinsY(),
                  h.GetNum()->GetYaxis()->GetBinLowEdge(1),
                  h.GetNum()->GetYaxis()->GetBinUpEdge(h.GetNum()->GetNbinsY()),
                  h.GetNum()->GetNbinsZ(),
                  h.GetNum()->GetZaxis()->GetBinLowEdge(1),
                  h.GetNum()->GetZaxis()->GetBinUpEdge(h.GetNum()->GetNbinsZ())) {}

}  // namespace Hal
