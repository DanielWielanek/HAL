/*
 * CorrFit3DCFBowlerSinyukovExtra.cxx
 *
 *  Created on: 31 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFit3DCFBowlerSinyukovExtra.h"

#include <TVector3.h>

namespace Hal {
  Double_t CorrFit3DCFBowlerSinyukovEllipse::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t cf_free = 0;
    Double_t kq      = 0;
    switch (fFrame) {
      case Femto::EKinematics::kLCMS: {
        Double_t rout   = GetRoutPRF(params[Routidx()], params[Tau()]);
        Double_t k_star = fQinvMap->Eval(x[0], x[1], x[2]);
        Double_t kout   = TMath::Sqrt(k_star * k_star - x[1] * x[1] - x[2] * x[2]);
        k_star          = k_star * 0.5;
        Double_t rinv   = GetRinv(rout, params[Rsideidx()], params[Rlongidx()], kout, x[1], x[2]);
        kq              = fCFs->Eval(k_star, rinv);
        cf_free         = 1.
                  + fQSMode
                      * TMath::Exp(-25.76578
                                   * (x[0] * x[0] * params[Routidx()] * params[Routidx()]
                                      + x[1] * x[1] * params[Rsideidx()] * params[Rsideidx()]
                                      + x[2] * x[2] * params[Rlongidx()] * params[Rlongidx()]));

      } break;
      case Femto::EKinematics::kPRF: {
        Double_t k_star = TMath::Sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
        Double_t rinv   = GetRinv(params[Routidx()], params[Rsideidx()], params[Rlongidx()], x[0], x[1], x[2]);
        std::cout << rinv << std::endl;
        cf_free = 1.
                  + fQSMode
                      * TMath::Exp(-25.76578 * 4.0
                                   * (x[0] * x[0] * params[Routidx()] * params[Routidx()]
                                      + x[1] * x[1] * params[Rsideidx()] * params[Rsideidx()]
                                      + x[2] * x[2] * params[Rlongidx()] * params[Rlongidx()]));  // x4 cause PRF
        kq = fCFs->Eval(k_star, rinv);
      } break;
      default:
        // r_out = params[Routidx()];
        break;
    }
    return params[Normidx()] * (1.0 - params[Lambdaidx()] + params[Lambdaidx()] * kq * cf_free);
  }

  CorrFit3DCFBowlerSinyukovEllipse::CorrFit3DCFBowlerSinyukovEllipse() : CorrFit3DCFBowlerSinyukov() {}

  Double_t CorrFit3DCFBowlerSinyukovEllipse::GetRinv(Double_t xout,
                                                     Double_t xside,
                                                     Double_t xlong,
                                                     Double_t kout,
                                                     Double_t kside,
                                                     Double_t klong) const {
    TVector3 vec(kout, kside, klong);
    Double_t P    = vec.Phi();
    Double_t T    = vec.Theta();
    Double_t sinP = TMath::Power(TMath::Sin(P), 2);
    Double_t sinT = TMath::Power(TMath::Sin(T), 2);
    Double_t cosP = TMath::Power(TMath::Cos(P), 2);
    Double_t cosT = TMath::Power(TMath::Cos(T), 2);
    Double_t a    = xout * xout;
    Double_t b    = xside * xside;
    Double_t c    = xlong * xlong;
    Double_t rinv = TMath::Sqrt(1.0 / (cosP * sinT / a + sinP * sinT / b + cosT / c));
    if (TMath::IsNaN(rinv)) rinv = 20;
    return rinv;
  }

  CorrFit3DCFBowlerSinyukovEllipse::~CorrFit3DCFBowlerSinyukovEllipse() {}

  Double_t CorrFit3DCFBowlerSinyukovClassic::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t cf_free = 0;
    Double_t kq      = 0;

    switch (fFrame) {
      case Femto::EKinematics::kLCMS: {
        Double_t k_star = 0;
        k_star          = 0.5 * fQinvMap->Eval(x[0], x[1], x[2]);
        kq              = fCFs->Eval(k_star, params[RCoul()]);
        cf_free         = 1.
                  + fQSMode
                      * TMath::Exp(-25.76578
                                   * (x[0] * x[0] * params[Routidx()] * params[Routidx()]
                                      + x[1] * x[1] * params[Rsideidx()] * params[Rsideidx()]
                                      + x[2] * x[2] * params[Rlongidx()] * params[Rlongidx()]));
      } break;
      case Femto::EKinematics::kPRF: {
        Double_t k_star = TMath::Sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
        cf_free         = 1.
                  + fQSMode
                      * TMath::Exp(-25.76578 * 4.0
                                   * (x[0] * x[0] * params[Routidx()] * params[Routidx()]
                                      + x[1] * x[1] * params[Rsideidx()] * params[Rsideidx()]
                                      + x[2] * x[2] * params[Rlongidx()] * params[Rlongidx()]));  // x4 cause PRF
        kq = fCFs->Eval(k_star, params[RCoul()]);
      } break;
      default: break;
    }
    if (kq < 0 || TMath::IsNaN(kq)) kq = 0;
    return params[Normidx()] * (1.0 - params[Lambdaidx()] + params[Lambdaidx()] * kq * cf_free);
  }

  CorrFit3DCFBowlerSinyukovClassic::CorrFit3DCFBowlerSinyukovClassic() : CorrFit3DCFBowlerSinyukov(1) {
    SetParameterName(6, "R_{coul}");
    FixParameter(RCoul(), 4);
  }

  CorrFit3DCFBowlerSinyukovClassic::~CorrFit3DCFBowlerSinyukovClassic() {}
}  // namespace Hal
