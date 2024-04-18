/*
 * CorrFit3DCFGauss2.cxx
 *
 *  Created on: 21 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFit3DCFGauss2.h"

#include <TMath.h>

namespace Hal {
  const Int_t CorrFit3DCF_Gauss2::fgRout2   = 5;
  const Int_t CorrFit3DCF_Gauss2::fgRside2  = 6;
  const Int_t CorrFit3DCF_Gauss2::fgRlong2  = 7;
  const Int_t CorrFit3DCF_Gauss2::fgLambda2 = 8;

  CorrFit3DCF_Gauss2::CorrFit3DCF_Gauss2() : CorrFit3DCF(9) {
    SetParameterName(Norm(), "Norm");
    SetParameterName(Lambda1(), "#Lambda_{1}");
    SetParameterName(Lambda2(), "#Lambda_{2}");
    SetParameterName(Rout1(), "R_{out1}");
    SetParameterName(Rout2(), "R_{out2}");
    SetParameterName(Rside1(), "R_{side1}");
    SetParameterName(Rside2(), "R_{side2}");
    SetParameterName(Rlong1(), "R_{long1}");
    SetParameterName(Rlong2(), "R_{long2}");
  }

  Double_t CorrFit3DCF_Gauss2::CalculateCF(const Double_t* x, const Double_t* params) const {
    return params[Norm()]
           * (1.0
              + params[this->Lambda1()]
                  * TMath::Exp(-Femto::FmToGeV() * Femto::FmToGeV()
                               * (TMath::Power(x[0] * params[this->Rout1()], 2) + TMath::Power(x[1] * params[this->Rside1()], 2)
                                  + TMath::Power(x[2] * params[this->Rlong1()], 2)))
              + params[this->Lambda2()]
                  * TMath::Exp(-Femto::FmToGeV() * Femto::FmToGeV()
                               * (TMath::Power(x[0] * params[this->Rout2()], 2) + TMath::Power(x[1] * params[this->Rside2()], 2)
                                  + TMath::Power(x[2] * params[this->Rlong2()], 2))));
  }

  CorrFit3DCF_Gauss2::~CorrFit3DCF_Gauss2() {}
}  // namespace Hal
