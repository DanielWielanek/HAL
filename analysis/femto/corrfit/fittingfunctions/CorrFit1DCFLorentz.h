/*
 * CorrFit1DCF_Lorentz.h
 *
 *  Created on: 12 lis 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFIT1DCFLORENTZ_H_
#define HALCORRFIT1DCFLORENTZ_H_

#include "CorrFit1DCF.h"

namespace Hal {
  class CorrFit1DCF_Lorentz : public CorrFit1DCF {
  protected:
    Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit1DCF_Lorentz();
    virtual ~CorrFit1DCF_Lorentz();
    ClassDef(CorrFit1DCF_Lorentz, 1)
  };
}  // namespace Hal

#endif /* HALROOT_ANALYSIS_FEMTO_CORRFIT_FITTINGFUNCTIONS_HALCORRFIT1DCFLORENTZ_H_                                               \
        */
