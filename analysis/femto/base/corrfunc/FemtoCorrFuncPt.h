/*
 * CorrFuncCollectionPt.h
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOCORRFUNCCOLLECTIONPT_H_
#define HALFEMTOCORRFUNCCOLLECTIONPT_H_
#include "FemtoCorrFunc.h"
/**
 * collection of cf's grouped in pt-sum bins
 */
namespace Hal {
  class FemtoCorrFuncPt : public FemtoCorrFunc1D {
  protected:
    FemtoCorrFuncPt(const DividedHisto1D* h = NULL, const std::initializer_list<double>& init = {0, 10});

  public:
    FemtoCorrFuncPt() : FemtoCorrFunc1D() {};
    FemtoCorrFuncPt(const DividedHisto1D& h, const std::initializer_list<double>& init = {0, 10});
    virtual Bool_t Check();
    virtual void FillNum(FemtoPair* pair);
    virtual void FillDenPerfect(FemtoPair* pair) { FillDenMixed(pair); };
    virtual void FillDenRotated(FemtoPair* pair) { FillDenMixed(pair); };
    virtual void FillDenMixed(FemtoPair* pair);
    virtual void FillDenHemisphere(FemtoPair* pair) { FillDenMixed(pair); };
    virtual void FillDenCharged(FemtoPair* pair) { FillDenMixed(pair); };
    virtual ~FemtoCorrFuncPt();
    ClassDef(FemtoCorrFuncPt, 1)
  };
}  // namespace Hal
#endif /* HALFEMTOCORRFUNCCOLLECTIONPT_H_ */
