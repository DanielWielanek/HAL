/*
 * CorrFit1DCF_Lorentz.cxx
 *
 *  Created on: 12 lis 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFit1DCFLorentz.h"

#include <TMath.h>

namespace Hal {
  CorrFit1DCF_Lorentz::CorrFit1DCF_Lorentz() {}

  Double_t CorrFit1DCF_Lorentz::CalculateCF(const Double_t* x, const Double_t* params) const {
    return params[Norm()]
           * (1
              + params[Lambda()]
                  / TMath::Power(Femto::FmToGeV() * Femto::FmToGeV() * (x[0] * x[0] * params[Radius()] * params[Radius()]) + 1,
                                 2));
  }

  CorrFit1DCF_Lorentz::~CorrFit1DCF_Lorentz() {}
}  // namespace Hal
