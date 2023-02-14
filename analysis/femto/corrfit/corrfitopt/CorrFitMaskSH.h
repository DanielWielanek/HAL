/*
 * CorrFitMaskSHa.h
 *
 *  Created on: 12 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITMASKSH_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITMASKSH_H_

#include "CorrFitMask.h"
#include "FemtoYlmIndexes.h"

#include <vector>

#include <RtypesCore.h>

namespace Hal {
  class CorrFitMaskSH : public CorrFitMask {
    Int_t fL;
    Int_t fBins;
    Double_t fMin, fMax;
    std::vector<std::vector<Short_t>> fRawMaskRe;
    std::vector<std::vector<Short_t>> fRawMaskIm;
    std::vector<Int_t> fActiveReal;
    std::vector<Int_t> fActiveImag;
    FemtoYlmIndexes fMath;

  public:
    CorrFitMaskSH(Int_t bins = 100, Double_t min = 0, Double_t max = 100, Int_t maxL = 3);
    void ApplyRange(Double_t min, Double_t max, Bool_t additive = kTRUE);
    void SetBin(Int_t bin, Bool_t state = true);
    void SetBin(Int_t L, Int_t M, Int_t bin, Bool_t state = true);
    void SetBinRe(Int_t L, Int_t M, Int_t bin, Bool_t state = true);
    void SetBinIm(Int_t L, Int_t M, Int_t bin, Bool_t state = true);
    void Reset(Bool_t state = kTRUE);
    Int_t GetActiveBins(Int_t L, Int_t M, Bool_t real) const;
    const std::vector<Short_t>& GetMaskArray(Int_t L, Int_t M, Bool_t real) const;
    Bool_t Init();
    Bool_t AreCompatible(TObject* cf) const;
    void ApplyThreshold(const TH1& h, Double_t threshold = 0);
    Int_t GetNBins() const { return fRawMaskRe[0].size(); };
    Double_t GetMin() const { return fMin; };
    Double_t GetMax() const { return fMax; }
    virtual ~CorrFitMaskSH() {};
    ClassDef(CorrFitMaskSH, 1)
  };
}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITMASKSHA_H_ */
