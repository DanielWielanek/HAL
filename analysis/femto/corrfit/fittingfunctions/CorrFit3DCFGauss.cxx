/*
 * CorrFit3DCFGauss.cxx
 *
 *  Created on: 08-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFit3DCFGauss.h"

#include <TMath.h>

namespace Hal {
  Double_t CorrFit3DCF_Gauss::CalculateCF(const Double_t* x, const Double_t* params) const {
    if (fKinematics == Femto::EKinematics::kLCMS)
      return params[Normidx()]
             * (1
                + params[Lambdaidx()]
                    * TMath::Exp(-25.76578
                                 * (x[0] * x[0] * params[Routidx()] * params[Routidx()]
                                    + x[1] * x[1] * params[Rsideidx()] * params[Rsideidx()]
                                    + x[2] * x[2] * params[Rlongidx()] * params[Rlongidx()])));
    else
      return params[Normidx()]
             * (1
                + params[Lambdaidx()]
                    * TMath::Exp(-25.76578 * 4.0
                                 * (x[0] * x[0] * params[Routidx()] * params[Routidx()]
                                    + x[1] * x[1] * params[Rsideidx()] * params[Rsideidx()]
                                    + x[2] * x[2] * params[Rlongidx()] * params[Rlongidx()])));
  }

  CorrFit3DCF_Gauss::CorrFit3DCF_Gauss() : CorrFit3DCF(5) {}

  CorrFit3DCF_Gauss::~CorrFit3DCF_Gauss() {}
}  // namespace Hal
