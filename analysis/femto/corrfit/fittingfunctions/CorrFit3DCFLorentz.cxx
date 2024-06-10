/*
 * CorrFit3DCFLorentz.cxx
 *
 *  Created on: 24 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFit3DCFLorentz.h"


namespace Hal {
  CorrFit3DCF_Lorentz::CorrFit3DCF_Lorentz() : CorrFit3DCF(5) {}

  Double_t CorrFit3DCF_Lorentz::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t X = x[0] * params[RoutID()] * Femto::FmToGeV();
    Double_t Y = x[1] * params[RsideID()] * Femto::FmToGeV();
    Double_t Z = x[2] * params[RlongID()] * Femto::FmToGeV();
    return params[NormID()] * (1.0 + params[LambdaID()] / (X * X + Y * Y + Z * Z + 1));
  }

  CorrFit3DCF_Lorentz::~CorrFit3DCF_Lorentz() {}
}  // namespace Hal
