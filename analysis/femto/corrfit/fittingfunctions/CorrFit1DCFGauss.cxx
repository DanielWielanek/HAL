/*
 * CorrFit1DCFGauss.cxx
 *
 *  Created on: 03-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFit1DCFGauss.h"

#include <TMath.h>

namespace Hal {
  CorrFit1DCF_Gauss::CorrFit1DCF_Gauss() : CorrFit1DCF(3) {}

  Double_t CorrFit1DCF_Gauss::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t q = x[0];
    if (fKinematics == Femto::EKinematics::kPRF) q = q * 2.0;
    return params[NormID()]
           * (1
              + params[LambdaID()]
                  * TMath::Exp(-Femto::FmToGeV() * Femto::FmToGeV() * (q * q * params[RadiusID()] * params[RadiusID()])));
  }

  CorrFit1DCF_Gauss::~CorrFit1DCF_Gauss() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
