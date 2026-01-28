/*
 * CorrFit1DCFExp.cxx
 *
 *  Created on: 29-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFit1DCFExp.h"

#include <TMath.h>

namespace Hal {
  CorrFit1DCF_Exp::CorrFit1DCF_Exp() {
    // TODO Auto-generated constructor stub
  }

  Double_t CorrFit1DCF_Exp::CalculateCF(const Double_t* x, const Double_t* params) const {
    return params[2] * (1 + params[1] * TMath::Exp(-Femto::FmToGeV() * x[0] * params[0]));
  }

  CorrFit1DCF_Exp::~CorrFit1DCF_Exp() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
