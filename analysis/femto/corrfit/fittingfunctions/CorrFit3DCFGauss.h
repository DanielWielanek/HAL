/*
 * CorrFit3DCFGauss.h
 *
 *  Created on: 08-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALCORRFIT3DCFGAUSS_H_
#define HALCORRFIT3DCFGAUSS_H_
#include "CorrFit3DCF.h"
/**
 * class for fitting simple 3d CF with gaussian shape where only quantum
 * statistic play role
 */

namespace Hal {
  class CorrFit3DCF_Gauss : public CorrFit3DCF {
  protected:
    Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit3DCF_Gauss();
    virtual ~CorrFit3DCF_Gauss();
    ClassDef(CorrFit3DCF_Gauss, 1)
  };
}  // namespace Hal
#endif /* HALCORRFIT3DCFGAUSS_H_ */
