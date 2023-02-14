/*
 * CorrFitMask3Da.h
 *
 *  Created on: 12 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITMASK3DA_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITMASK3DA_H_

#include "Array.h"
#include "CorrFitMask.h"

#include <vector>

#include <RtypesCore.h>

namespace Hal {
  class CorrFitMask3D : public CorrFitMask {
  public:
    enum class EFitExtraMask {
      kStandard,
      kSlice,
      kDiagonalSlice,
      kUltraDiagonalSlice,
      kDiagonalSliceIgnored,
      kUltraDiagonalSliceIgnored
    };

  private:
    Int_t fBins[3];
    Double_t fMin[3], fMax[3];
    std::vector<std::vector<std::vector<Short_t>>> fRawMask;
    void CalcMap(Array_3<Short_t>& map, EFitExtraMask mask);
    void CalculateDiagonalBins(Array_3<Short_t>& map, EFitExtraMask mask);
    void CalculateUltradiagonalBins(Array_3<Short_t>& map, EFitExtraMask mask);
    void CalculateSliceBins(Array_3<Short_t>& map);

  public:
    CorrFitMask3D(Int_t binsX   = 100,
                  Double_t minX = 0,
                  Double_t maxX = 1,
                  Int_t binsY   = 100,
                  Double_t minY = 0,
                  Double_t maxY = 1,
                  Int_t binsZ   = 100,
                  Double_t minZ = 0,
                  Double_t maxZ = 1);
    void
    ApplyRange(Double_t minX, Double_t maxX, Double_t minY, Double_t maxY, Double_t minZ, Double_t maxZ, Bool_t additive = kTRUE);
    void ApplyMask(EFitExtraMask mask, Bool_t additive = kFALSE);
    void SetBin(Int_t binX, Int_t binY, Int_t binZ, Bool_t state = true);
    void Reset(Bool_t state = kTRUE);
    void ApplyThreshold(const TH1& h, Double_t threshold = 0);
    Bool_t Init();
    inline Bool_t GetBinFlag(Int_t x, Int_t y, Int_t z) const { return fRawMask[x][y][z]; };
    Bool_t AreCompatible(TObject* cf) const;
    Int_t GetNBins() const { return fRawMask.size(); };
    Int_t GetNbinsX() const { return fRawMask.size(); };
    Int_t GetNbinsY() const { return fRawMask[0].size(); };
    Int_t GetNbinsZ() const { return fRawMask[0][0].size(); };
    virtual ~CorrFitMask3D() {};
    ClassDef(CorrFitMask3D, 1)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITMASK3DA_H_ */
