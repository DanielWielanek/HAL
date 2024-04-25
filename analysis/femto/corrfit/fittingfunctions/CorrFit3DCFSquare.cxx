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
  const Int_t CorrFit3DCF_Square::fgAx = 0;
  const Int_t CorrFit3DCF_Square::fgBx = 1;
  const Int_t CorrFit3DCF_Square::fgAy = 2;
  const Int_t CorrFit3DCF_Square::fgBy = 3;
  const Int_t CorrFit3DCF_Square::fgAz = 5;
  const Int_t CorrFit3DCF_Square::fgBz = 6;

  CorrFit3DCF_Square::CorrFit3DCF_Square(Int_t pol) : CorrFit3DCF(7) {
    SetParameterName(Ax(), "A_{x}");
    SetParameterName(Ay(), "A_{y}");
    SetParameterName(Az(), "A_{z}");
    SetParameterName(Bx(), "B_{x}");
    SetParameterName(By(), "B_{y}");
    SetParameterName(Bz(), "B_{z}");
    if (pol < 1) {
      SetParameterName(Ax(), "A_{x}(not used)");
      SetParameterName(Ay(), "A_{y}(not used)");
      SetParameterName(Az(), "A_{z}(not used)");
      FixParameter(Ax(), 0);
      FixParameter(Ay(), 0);
      FixParameter(Az(), 0);
    }
    if (pol < 2) {
      SetParameterName(Bx(), "B_{x}(not used)");
      SetParameterName(By(), "B_{y}(not used)");
      SetParameterName(Bz(), "B_{z}(not used)");
      FixParameter(Bx(), 0);
      FixParameter(By(), 0);
      FixParameter(Bz(), 0);
    }
  }

  Double_t CorrFit3DCF_Square::CalculateCF(const Double_t* x, const Double_t* params) const {
    return params[NormID()]
           * (1.0 + +TMath::Abs(x[0]) * params[Ax()] + x[0] * x[0] * params[Bx()] + TMath::Abs(x[1]) * params[Ay()]
              + x[1] * x[1] * params[By()] + TMath::Abs(x[2]) * params[Az()] + x[2] * x[2] * params[Bz()]);
  }

  CorrFit3DCF_Square::~CorrFit3DCF_Square() {}
}  // namespace Hal
