/*
 * FemtoCorrFuncSimple.h
 *
 *  Created on: 8 lut 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_FEMTO_BASE_CORRFUNC_HALFEMTOCORRFUNCSIMPLE_H_
#define HALFEMTO_ANALYSIS_FEMTO_BASE_CORRFUNC_HALFEMTOCORRFUNCSIMPLE_H_

#include "FemtoCorrFunc.h"

namespace Hal {
  class FemtoCorrFuncSimple : public FemtoCorrFunc1D {
  public:
    FemtoCorrFuncSimple();
    FemtoCorrFuncSimple(const DividedHisto1D& h);
    FemtoCorrFuncSimple(const FemtoCorrFuncSimple& other);
    virtual void FillNum(FemtoPair* pair);
    virtual void FillDenPerfect(FemtoPair* pair) { FillDenMixed(pair); };
    virtual void FillDenRotated(FemtoPair* pair);
    virtual void FillDenMixed(FemtoPair* pair);
    virtual void FillDenHemisphere(FemtoPair* pair) { FillDenRotated(pair); };
    virtual void FillDenCharged(FemtoPair* pair) { FillDenMixed(pair); };
    virtual ~FemtoCorrFuncSimple() {};
    ClassDef(FemtoCorrFuncSimple, 1)
  };
}  // namespace Hal


#endif /* HALFEMTO_ANALYSIS_FEMTO_BASE_CORRFUNC_HALFEMTOCORRFUNCSIMPLE_H_ */
