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
  const Int_t CorrFit1DCF_Square::fgA = 0;
  const Int_t CorrFit1DCF_Square::fgB = 1;

  CorrFit1DCF_Square::CorrFit1DCF_Square(Int_t pol) : CorrFit1DCF(3) {
    SetParameterName(A(), "A");
    SetParameterName(B(), "B");
    if (pol < 1) {
      SetParameterName(A(), "A (not used)");
      SetParLimits(A(), 0, 0.1);
      FixParameter(A(), 0);
    }
    if (pol < 2) {
      SetParameterName(B(), "B (not used)");
      FixParameter(B(), 0);
      SetParLimits(B(), 0, 0.1);
    }
  }

  Double_t CorrFit1DCF_Square::CalculateCF(const Double_t* x, const Double_t* params) const {
    return (params[Norm()]) * (1.0 + params[A()] * x[0] + params[B()] * x[0] * x[0]);
  }

  CorrFit1DCF_Square::~CorrFit1DCF_Square() {}
}  // namespace Hal
