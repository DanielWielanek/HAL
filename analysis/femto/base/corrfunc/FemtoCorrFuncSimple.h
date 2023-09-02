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

#include "FemtoCorrFunc0D.h"

namespace Hal {
  class FemtoCorrFuncSimple : public FemtoCorrFunc0D {
    virtual void FillDenMixed(Hal::FemtoPair* pair);
    virtual void FillDenCharged(Hal::FemtoPair* pair);
    virtual void FillNum(Hal::FemtoPair* pair);
    virtual void FillDenHemisphere(Hal::FemtoPair* pair);
    virtual void FillDenPerfect(Hal::FemtoPair* pair);
    virtual void FillDenRotated(Hal::FemtoPair* pair);

  public:
    FemtoCorrFuncSimple();
    virtual Bool_t Check() { return kTRUE; }
    FemtoCorrFuncSimple(const DividedHisto1D& h);
    FemtoCorrFuncSimple(const FemtoCorrFuncSimple& other);
    virtual ~FemtoCorrFuncSimple() {};
    ClassDef(FemtoCorrFuncSimple, 1)
  };
}  // namespace Hal


#endif /* HALFEMTO_ANALYSIS_FEMTO_BASE_CORRFUNC_HALFEMTOCORRFUNCSIMPLE_H_ */
