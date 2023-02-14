/*
 * CorrFitMask1D.h
 *
 *  Created on: 12 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITMASK1D_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITMASK1D_H_

#include "CorrFitMask.h"

#include <vector>

#include <RtypesCore.h>

namespace Hal {
  class CorrFitMask1D : public CorrFitMask {
    Int_t fBins;
    Double_t fMin, fMax;
    std::vector<Short_t> fRawMask;

  public:
    CorrFitMask1D(Int_t bins = 100, Double_t min = 0, Double_t max = 1);
    void ApplyRange(Double_t min, Double_t max, Bool_t additive = kTRUE);
    void SetBin(Int_t bin, Bool_t state = true);
    void Reset(Bool_t state = kTRUE);
    Bool_t Init();
    Bool_t GetBinFlag(Int_t no) const { return fRawMask[no]; };
    Bool_t AreCompatible(TObject* cf) const;
    void ApplyThreshold(const TH1& h, Double_t threshold = 0);
    Int_t GetNBins() const { return fRawMask.size(); };
    Double_t GetMin() const { return fMin; };
    Double_t GetMax() const { return fMax; }
    virtual ~CorrFitMask1D() {};
    ClassDef(CorrFitMask1D, 1)
  };

}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITOPT_CORRFITMASK1D_H_ */
