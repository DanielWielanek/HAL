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
#include "Std.h"

#include <RtypesCore.h>
#include <vector>

namespace Hal {
  class CorrFitMask : public TObject {
  public:
    /**
     * enum used to mask map, the "Mask" is applied mask, Criteria - is the mask defined by criteria (e.g. range)
     */
    enum class ELogic {
      kAnd, /**< kAnd Mask is true only if Mask and Criteria are true*/
      kOr,  /**< kOr  Mask true if Criteria or Mask are true*/
      kNot  /**< kNot Mask is set false if Criteria are true*/
    };

  protected:
    Int_t fActiveBins = {0};
    void Mask(Array_1<Short_t>& map, const Array_1<Short_t>& mask, ELogic logic) const;
    void Mask(Array_2<Short_t>& map, const Array_2<Short_t>& mask, ELogic logic) const;
    void Mask(Array_3<Short_t>& map, const Array_3<Short_t>& mask, ELogic logic) const;
    void SetGlobalStatus(Array_1<Short_t>& map, Int_t status) const;
    void SetGlobalStatus(Array_2<Short_t>& map, Int_t status) const;
    void SetGlobalStatus(Array_3<Short_t>& map, Int_t status) const;

  public:
    CorrFitMask();
    virtual Bool_t AreCompatible(TObject* /*cf*/) const { return kFALSE; }
    virtual void Reset(Bool_t /*state*/ = kTRUE) {};
    virtual void ApplyThreshold(const TH1& h, Double_t threshold = 0, ELogic logic = ELogic::kAnd);
    virtual Bool_t Init() = 0;
    Int_t GetActiveBins() const { return fActiveBins; };
    virtual ~CorrFitMask() {};
    ClassDef(CorrFitMask, 1)
  };

}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_CORRFITMASK_H_ */
