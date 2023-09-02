/*
 * FemtoCorrFuncY.h
 *
 *  Created on: 25 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNCY_H_
#define HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNCY_H_

#include "FemtoCorrFunc1D.h"
namespace Hal {
  class FemtoCorrFuncY : public FemtoCorrFunc1D {
  protected:
    Double_t GetPairValNum(FemtoPair* pair) const;
    Double_t GetPairValDenRot(FemtoPair* pair) const;
    Double_t GetPairValDenHemi(FemtoPair* pair) const;

  public:
    FemtoCorrFuncY(const DividedHisto1D& h, const std::initializer_list<double>& init = {0, 10}, Int_t ktBins = 100);
    FemtoCorrFuncY(const FemtoCorrFuncY& other);
    virtual ~FemtoCorrFuncY() {};
  };
}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_BASE_CORRFUNC_FEMTOCORRFUNCY_H_ */
