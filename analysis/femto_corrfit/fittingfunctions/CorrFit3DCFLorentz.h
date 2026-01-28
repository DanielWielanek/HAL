/*
 * CorrFit3DCFLorentz.h
 *
 *  Created on: 24 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFIT3DCFLORENTZ_H_
#define HALCORRFIT3DCFLORENTZ_H_

#include "CorrFit3DCF.h"


namespace Hal {
  class CorrFit3DCF_Lorentz : public CorrFit3DCF {
  protected:
    Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit3DCF_Lorentz();
    virtual ~CorrFit3DCF_Lorentz();
    ClassDef(CorrFit3DCF_Lorentz, 1)
  };
}  // namespace Hal
#endif /* HALCORRFIT3DCFLORENTZ_H_ */
