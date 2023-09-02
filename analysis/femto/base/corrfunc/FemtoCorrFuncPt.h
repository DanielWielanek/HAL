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
#include "FemtoCorrFunc1D.h"
/**
 * collection of cf's grouped in pt-sum bins
 */
namespace Hal {
  class FemtoCorrFuncPt : public FemtoCorrFunc1D {
  protected:
    Double_t GetPairValDenRot(Hal::FemtoPair* pair) const { return GetPairValNum(pair); };
    Double_t GetPairValDenHemi(Hal::FemtoPair* pair) const { return GetPairValNum(pair); };
    Double_t GetPairValNum(Hal::FemtoPair* pair) const;
    Double_t GetPairValDen(Hal::FemtoPair* pair) const { return GetPairValNum(pair); };

  public:
    FemtoCorrFuncPt() : FemtoCorrFunc1D() {};
    FemtoCorrFuncPt(const DividedHisto1D& h, const std::initializer_list<double>& init = {0, 10}, Int_t bins = 100);
    virtual ~FemtoCorrFuncPt() {};
    ClassDef(FemtoCorrFuncPt, 1)
  };
}  // namespace Hal
#endif /* HALFEMTOCORRFUNCCOLLECTIONPT_H_ */
