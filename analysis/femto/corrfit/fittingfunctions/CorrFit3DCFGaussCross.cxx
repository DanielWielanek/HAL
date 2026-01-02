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
#include <iostream>

namespace Hal {
  CorrFit3DCF_GaussCross::CorrFit3DCF_GaussCross() : CorrFit3DCF(e3DMode::kNormal6R, 8) {
    SetParameterName(RoutsideID(), "R_{out-side}");
    FixParameter(RoutsideID(), 0);
    SetParameterName(RoutlongID(), "R_{out-long}");
    FixParameter(RoutlongID(), 0);
    SetParameterName(RsidelongID(), "R_{side-long}");
    FixParameter(RsidelongID(), 0);
  }

  CorrFit3DCF_GaussCross::~CorrFit3DCF_GaussCross() {}
  Double_t CorrFit3DCF_GaussCross::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t scale = -25.76578;
    if (fKinematics == Hal::Femto::EKinematics::kPRF) { scale = -25.76578 * 4.0; }
    return params[NormID()]
           * (1
              + params[LambdaID()]
                  * TMath::Exp(scale
                               * (x[0] * x[0] * params[RoutID()] * params[RoutID()]
                                  + x[1] * x[1] * params[RsideID()] * params[RsideID()]
                                  + x[2] * x[2] * params[RlongID()] * params[RlongID()]
                                  + 2.0 * x[0] * x[1] * params[RoutsideID()]       //* params[RoutsideID()]
                                  + 2.0 * x[0] * x[2] * params[RoutlongID()]       // * params[RoutlongID()]
                                  + 2.0 * x[1] * x[2] * params[RsidelongID()])));  //* params[RsidelongID()])));
  }
}  // namespace Hal
