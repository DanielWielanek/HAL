/*
 * CorrFit3DCFGaus.cxx
 *
 *  Created on: 01-05-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFit3DCFGaussCross.h"

#include <TMath.h>

namespace Hal {
  const Int_t CorrFit3DCF_GaussCross::fgRoutlong  = 5;
  const Int_t CorrFit3DCF_GaussCross::fgRoutside  = 6;
  const Int_t CorrFit3DCF_GaussCross::fgRsidelong = 7;
  CorrFit3DCF_GaussCross::CorrFit3DCF_GaussCross() : CorrFit3DCF(8) {
    SetParameterName(Routside(), "R_{out-side}");
    FixParameter(Routside(), 0);
    SetParameterName(Routlong(), "R_{out-long}");
    FixParameter(Routlong(), 0);
    SetParameterName(Rsidelong(), "R_{side-long}");
    FixParameter(Rsidelong(), 0);
  }

  CorrFit3DCF_GaussCross::~CorrFit3DCF_GaussCross() {}
  Double_t CorrFit3DCF_GaussCross::CalculateCF(const Double_t* x, const Double_t* params) const {
    return params[Norm()]
           * (1
              + params[Lambda()]
                  * TMath::Exp(-25.76578
                               * (x[0] * x[0] * params[Rout()] * params[Rout()] + x[1] * x[1] * params[Rside()] * params[Rside()]
                                  + x[2] * x[2] * params[Rlong()] * params[Rlong()]
                                  + 2.0 * x[0] * x[1] * TMath::Abs(params[Routside()]) * params[Routside()]
                                  + 2.0 * x[0] * x[2] * TMath::Abs(params[Routlong()]) * params[Routlong()]
                                  + 2.0 * x[1] * x[2] * TMath::Abs(params[Rsidelong()]) * params[Rsidelong()])));
  }
}  // namespace Hal
