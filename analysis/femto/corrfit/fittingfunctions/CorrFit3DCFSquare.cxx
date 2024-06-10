/*
 * CorrFit3DSquare.cxx
 *
 *  Created on: 22 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFit3DCFSquare.h"


namespace Hal {

  CorrFit3DCF_Square::CorrFit3DCF_Square(Int_t pol) : CorrFit3DCF(7) {
    SetParameterName(AxID(), "A_{x}");
    SetParameterName(AyID(), "A_{y}");
    SetParameterName(AzID(), "A_{z}");
    SetParameterName(BxID(), "B_{x}");
    SetParameterName(ByID(), "B_{y}");
    SetParameterName(BzID(), "B_{z}");
    if (pol < 1) {
      SetParameterName(AxID(), "A_{x}(not used)");
      SetParameterName(AyID(), "A_{y}(not used)");
      SetParameterName(AzID(), "A_{z}(not used)");
      FixParameter(AxID(), 0);
      FixParameter(AyID(), 0);
      FixParameter(AzID(), 0);
    }
    if (pol < 2) {
      SetParameterName(BxID(), "B_{x}(not used)");
      SetParameterName(ByID(), "B_{y}(not used)");
      SetParameterName(BzID(), "B_{z}(not used)");
      FixParameter(BxID(), 0);
      FixParameter(ByID(), 0);
      FixParameter(BzID(), 0);
    }
  }

  Double_t CorrFit3DCF_Square::CalculateCF(const Double_t* x, const Double_t* params) const {
    return params[NormID()]
           * (1.0 + +TMath::Abs(x[0]) * params[AxID()] + x[0] * x[0] * params[BxID()] + TMath::Abs(x[1]) * params[AyID()]
              + x[1] * x[1] * params[ByID()] + TMath::Abs(x[2]) * params[AzID()] + x[2] * x[2] * params[BzID()]);
  }

  CorrFit3DCF_Square::~CorrFit3DCF_Square() {}
}  // namespace Hal
