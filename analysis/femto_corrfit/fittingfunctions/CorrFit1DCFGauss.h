/*
 * CorrFit1DCFGauss.h
 *
 *  Created on: 03-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALCORRFIT1DCFGAUSS_H_
#define HALCORRFIT1DCFGAUSS_H_
#include "CorrFit1DCF.h"
/**
 * function for fitting 1-dim CF with gaussian source, taking into account only
 * quantum statistic effect
 */
namespace Hal {
  class CorrFit1DCF_Gauss : public CorrFit1DCF {
  protected:
    Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit1DCF_Gauss();
    virtual ~CorrFit1DCF_Gauss();
    ClassDef(CorrFit1DCF_Gauss, 1)
  };
}  // namespace Hal
#endif /* HALCORRFIT1DCFGAUSS_H_ */
