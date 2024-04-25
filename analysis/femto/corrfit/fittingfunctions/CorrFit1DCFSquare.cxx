/*
 * CorrFit1DSquare.cxx
 *
 *  Created on: 22 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFit1DCFSquare.h"

namespace Hal {

  CorrFit1DCF_Square::CorrFit1DCF_Square(Int_t pol) : CorrFit1DCF(3) {
    SetParameterName(AID(), "A");
    SetParameterName(BID(), "B");
    if (pol < 1) {
      SetParameterName(AID(), "A (not used)");
      SetParLimits(AID(), 0, 0.1);
      FixParameter(AID(), 0);
    }
    if (pol < 2) {
      SetParameterName(BID(), "B (not used)");
      FixParameter(BID(), 0);
      SetParLimits(BID(), 0, 0.1);
    }
  }

  Double_t CorrFit1DCF_Square::CalculateCF(const Double_t* x, const Double_t* params) const {
    return (params[NormID()]) * (1.0 + params[AID()] * x[0] + params[BID()] * x[0] * x[0]);
  }

  CorrFit1DCF_Square::~CorrFit1DCF_Square() {}
}  // namespace Hal
