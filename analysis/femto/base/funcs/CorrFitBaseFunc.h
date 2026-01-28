/*
 * CorrFitBaseFunc.h
 *
 *  Created on: 28 sty 2026
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_BASE_FUNCS_CORRFITBASEFUNC_H_
#define HAL_ANALYSIS_FEMTO_BASE_FUNCS_CORRFITBASEFUNC_H_


#include "Object.h"

namespace Hal {
  class Femto1DCF;
  class Femto3DCF;
  class FemtoSHCF;
  /**
   * pure abstract class to separate fit from analysis libraries
   */
  class CorrFitBaseFunc : public DrawableObject {
    friend class Femto1DCF;
    friend class Femto3DCF;
    friend class FemtoSHCF;

  protected:
    virtual void Fit(TObject* object)      = 0;
    virtual void FitDummy(TObject* object) = 0;

  public:
    CorrFitBaseFunc();
    virtual ~CorrFitBaseFunc();
    ClassDef(CorrFitBaseFunc, 1);
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_FUNCS_CORRFITBASEFUNC_H_ */
