/*
 * CorrFitMask.h
 *
 *  Created on: 11 lut 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITMASK_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITMASK_H_

#include "Femto1DCF.h"

#include "CorrFit3DCF.h"
#include "FemtoYlmIndexes.h"
#include "Std.h"

#include <RtypesCore.h>
#include <vector>

namespace Hal {
  class CorrFitMask : public TObject {

  protected:
    Int_t fActiveBins = {0};

  public:
    CorrFitMask();
    virtual Bool_t AreCompatible(TObject* cf) const { return kFALSE; }
    virtual void Reset(Bool_t state = kTRUE) {};
    virtual void ApplyThreshold(const TH1& h, Double_t threshold = 0);
    virtual Bool_t Init() = 0;
    Int_t GetActiveBins() const { return fActiveBins; };
    virtual ~CorrFitMask() {};
    ClassDef(CorrFitMask, 1)
  };

}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITMASK_H_ */
