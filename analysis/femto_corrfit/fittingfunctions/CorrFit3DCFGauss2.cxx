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

  CorrFit3DCF_Gauss2::CorrFit3DCF_Gauss2() : CorrFit3DCF(9) {
    SetParameterName(NormID(), "Norm");
    SetParameterName(Lambda1ID(), "#Lambda_{1}");
    SetParameterName(Lambda2ID(), "#Lambda_{2}");
    SetParameterName(Rout1ID(), "R_{out1}");
    SetParameterName(Rout2ID(), "R_{out2}");
    SetParameterName(Rside1ID(), "R_{side1}");
    SetParameterName(Rside2ID(), "R_{side2}");
    SetParameterName(Rlong1ID(), "R_{long1}");
    SetParameterName(Rlong2ID(), "R_{long2}");
  }

  Double_t CorrFit3DCF_Gauss2::CalculateCF(const Double_t* x, const Double_t* params) const {
    return params[NormID()]
           * (1.0
              + params[this->Lambda1ID()]
                  * TMath::Exp(-Femto::FmToGeV() * Femto::FmToGeV()
                               * (TMath::Power(x[0] * params[this->Rout1ID()], 2)
                                  + TMath::Power(x[1] * params[this->Rside1ID()], 2)
                                  + TMath::Power(x[2] * params[this->Rlong1ID()], 2)))
              + params[this->Lambda2ID()]
                  * TMath::Exp(-Femto::FmToGeV() * Femto::FmToGeV()
                               * (TMath::Power(x[0] * params[this->Rout2ID()], 2)
                                  + TMath::Power(x[1] * params[this->Rside2ID()], 2)
                                  + TMath::Power(x[2] * params[this->Rlong2ID()], 2))));
  }

  CorrFit3DCF_Gauss2::~CorrFit3DCF_Gauss2() {}
}  // namespace Hal
