/*
 * CorrFit3DCFGausCross.h
 *
 *  Created on: 01-05-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALCORRFIT3DCFGAUSCROSS_H_
#define HALCORRFIT3DCFGAUSCROSS_H_
#include "CorrFit3DCF.h"
/**
 * class for fitting 3-dim CF's with only quantum statistic effects where are
 * present all radii components
 */

namespace Hal {
  class CorrFit3DCF_GaussCross : public CorrFit3DCF {

  protected:
    Double_t CalculateCF(const Double_t* x, const Double_t* params) const;

  public:
    CorrFit3DCF_GaussCross();
    virtual ~CorrFit3DCF_GaussCross();
    ClassDef(CorrFit3DCF_GaussCross, 1)
  };
}  // namespace Hal
#endif /* HALCORRFIT3DCFGAUSCROSS_H_ */
