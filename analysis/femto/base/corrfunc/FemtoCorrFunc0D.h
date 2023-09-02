/*
 * FemtoCorrFunc0D.h
 *
 *  Created on: 26 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNC0D_H_
#define HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNC0D_H_

#include "FemtoCorrFunc.h"
namespace Hal {

  class FemtoCorrFunc0D : public FemtoCorrFunc {
  public:
    FemtoCorrFunc0D() : FemtoCorrFunc() {};
    virtual Bool_t Check() { return kTRUE; };
    FemtoCorrFunc0D(const DividedHisto1D& h) : FemtoCorrFunc(h) {};
    virtual ~FemtoCorrFunc0D() {}
    ClassDef(FemtoCorrFunc0D, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNC0D_H_ */
