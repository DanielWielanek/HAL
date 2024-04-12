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
  CorrFit3DCF_GaussCross::CorrFit3DCF_GaussCross() : CorrFit3DCF(e3DMode::kNormal6R, 8) {
    SetParameterName(Routside(), "R_{out-side}");
    FixParameter(Routside(), 0);
    SetParameterName(Routlong(), "R_{out-long}");
    FixParameter(Routlong(), 0);
    SetParameterName(Rsidelong(), "R_{side-long}");
    FixParameter(Rsidelong(), 0);
  }

  CorrFit3DCF_GaussCross::~CorrFit3DCF_GaussCross() {}
  Double_t CorrFit3DCF_GaussCross::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t scale = 1;
    if (fKinematics == Hal::Femto::EKinematics::kPRF) { scale = 4.0; }
    return params[Normidx()]
           * (1
              + params[Lambdaidx()]
                  * TMath::Exp(-25.76578 * scale
                               * (x[0] * x[0] * params[Routidx()] * params[Routidx()]
                                  + x[1] * x[1] * params[Rsideidx()] * params[Rsideidx()]
                                  + x[2] * x[2] * params[Rlongidx()] * params[Rlongidx()]
                                  + 2.0 * x[0] * x[1] * TMath::Abs(params[Routside()]) * params[Routside()]
                                  + 2.0 * x[0] * x[2] * TMath::Abs(params[Routlong()]) * params[Routlong()]
                                  + 2.0 * x[1] * x[2] * TMath::Abs(params[Rsidelong()]) * params[Rsidelong()])));
  }
}  // namespace Hal
