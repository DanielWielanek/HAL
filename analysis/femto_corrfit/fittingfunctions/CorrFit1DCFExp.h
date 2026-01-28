/*
 * CorrFit1DCFExp.h
 *
 *  Created on: 29-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALCORRFIT1DCF_EXP_H_
#define HALCORRFIT1DCF_EXP_H_

#include "CorrFit1DCF.h"
/**
 * class for fitting CF with exponential source and only quantum statistic taken
 * into account
 */
namespace Hal {
  class CorrFit1DCF_Exp : public CorrFit1DCF {
  protected:
    Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit1DCF_Exp();
    virtual ~CorrFit1DCF_Exp();
    ClassDef(CorrFit1DCF_Exp, 1)
  };
}  // namespace Hal

#endif /* HALCORRFIT1DCFEXP_H_ */
