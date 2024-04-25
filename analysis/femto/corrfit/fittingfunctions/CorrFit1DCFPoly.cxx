/*
 * CorrFit1DCFPoly.cxx
 *
 *  Created on: 28 lis 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFit1DCFPoly.h"

namespace Hal {
  CorrFit1DCF_Poly::CorrFit1DCF_Poly(Int_t degree) {
    switch (degree) {
      case 1: {
        FixParameter(BID(), 0);
        FixParameter(CID(), 0);
      } break;
      case 2: {
        FixParameter(CID(), 0);
      } break;
      default: {
        // do nothing
      } break;
    }
    FixParameter(NormID(), 1);
    SetParameterName(AID(), "A");
    SetParameterName(BID(), "B");
    SetParameterName(CID(), "C");
    SetParameterName(NormID(), "N");
  }

  Double_t CorrFit1DCF_Poly::CalculateCF(const Double_t* x, const Double_t* params) const {
    return params[NormID()] * (params[AID()] * x[0] + params[BID()] * x[0] * x[0] + params[CID()] * x[0] * x[0] * x[0]);
  }

  CorrFit1DCF_Poly::~CorrFit1DCF_Poly() {}

  Double_t CorrFit1DCF_Poly2::CalculateCF(const Double_t* x, const Double_t* params) const {
    return params[NormID()] * (1 + params[AID()] * x[0] + params[BID()] * x[0] * x[0] + params[CID()] * x[0] * x[0] * x[0]);
  }

  CorrFit1DCF_Poly2::CorrFit1DCF_Poly2(Int_t /*degree*/) {}

  CorrFit1DCF_Poly2::~CorrFit1DCF_Poly2() {}
}  // namespace Hal
