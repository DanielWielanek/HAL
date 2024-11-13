/*
 * CorrFIt1DCFCumac.cxx
 *
 *  Created on: 13 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFit1DCFCumac.h"

#include "Const.h"

#include <TComplex.h>
#include <TF1.h>
#include <TMath.h>

namespace Hal {
  CorrFit1DCFCumac::CorrFit1DCFCumac() : CorrFit1DCFCumac(2) {}

  CorrFit1DCFCumac::CorrFit1DCFCumac(Int_t params) : CorrFit1DCF(params), fPis(TMath::Sqrt(TMath::Pi())), fHc(.1973), fZ(0) {}

  CorrFit1DCFCumac::~CorrFit1DCFCumac() {}

  Double_t CorrFit1DCFCumac::F1(Double_t z) const {
    fZ = z;
    return Integr();
  }

  Double_t CorrFit1DCFCumac::F(Double_t d, Double_t r0) const { return 1.0 - d / (2.0 * fPis * r0); }

  Double_t CorrFit1DCFCumac::F2(Double_t z) const { return (1.0 - TMath::Exp(-z * z)) / z; }

  Double_t CorrFit1DCFCumac::Ff1(Double_t x) const { return TMath::Exp(x * x - fZ * fZ) / fZ; }

  Double_t CorrFit1DCFCumac::Integr() const {
    Int_t n;
    Double_t xx, de1, a1h, ddd, abs__, eps;
    eps           = 1E-18;
    ddd           = .005;
    n             = 100;
    de1           = fZ / n;
    Double_t zero = 0.0;
    return Simps(zero, fZ, de1, ddd, eps, xx, a1h, abs__);
  }

  Double_t CorrFit1DCFCumac::Simps(Double_t a1,
                                   Double_t b1,
                                   Double_t h1,
                                   Double_t reps1,
                                   Double_t aeps1,
                                   Double_t x,
                                   Double_t aih,
                                   Double_t aiabs) const {
    Double_t r__1;
    Double_t result = 0;

    Double_t a, b, c__, f[7], h__;
    Int_t k;
    Double_t p[5], s, x0, di1, di2, di3, eps, aeps, reps, delta;

    r__1  = b1 - a1;
    h__   = Sign(h1, r__1);
    s     = Sign(1.0, h__);
    a     = a1;
    b     = b1;
    aih   = 0.f;
    aiabs = 0.f;
    p[1]  = 4.f;
    p[3]  = 4.f;
    p[2]  = 2.f;
    p[4]  = 1.f;
    if (b - a != 0.f) {
      goto L1;
    } else {
      goto L2;
    }
  L1:
    reps = TMath::Abs(reps1);
    aeps = TMath::Abs(aeps1);
    for (k = 1; k <= 7; ++k) {
      /* L3: */
      f[k - 1] = 1e17f;
    }
    x    = a;
    c__  = 0.f;
    f[0] = Ff1(x) / 3;
  L4:
    x0 = x;
    if ((x0 + h__ * 4 - b) * s <= 0.f) {
      goto L5;
    } else {
      goto L6;
    }
  L6:
    h__ = (b - x0) / 4;
    if (h__ != 0.f) {
      goto L7;
    } else {
      goto L2;
    }
  L7:
    for (k = 2; k <= 7; ++k) {
      /* L8: */
      f[k - 1] = 1e17f;
    }
    c__ = 1.f;
  L5:
    di2 = f[0];
    di3 = TMath::Abs(f[0]);
    for (k = 2; k <= 5; ++k) {
      x += h__;
      if ((x - b) * s >= 0.f) {
        goto L24;
      } else {
        goto L23;
      }
    L24:
      x = b;
    L23:
      if (f[k - 1] - 1e17f != 0.f) {
        goto L10;
      } else {
        goto L11;
      }
    L11:
      f[k - 1] = Ff1(x) / 3.0;
    L10:
      di2 += p[k - 1] * f[k - 1];
      /* L9: */
      di3 += p[k - 1] * (r__1 = f[k - 1], TMath::Abs(r__1));
    }
    di1 = (f[0] + f[2] * 4 + f[4]) * 2 * h__;
    di2 *= h__;
    di3 *= h__;
    if (reps != 0.f) {
      goto L12;
    } else {
      goto L13;
    }
  L13:
    if (aeps != 0.f) {
      goto L12;
    } else {
      goto L14;
    }
  L12:
    eps = (r__1 = (aiabs + di3) * reps, TMath::Abs(r__1));
    if (eps - aeps >= 0.f) {
      goto L16;
    } else {
      goto L15;
    }
  L15:
    eps = aeps;
  L16:
    delta = (r__1 = di2 - di1, TMath::Abs(r__1));
    if (delta - eps >= 0.f) {
      goto L21;
    } else {
      goto L20;
    }
  L20:
    if (delta - eps / 8 >= 0.f) {
      goto L14;
    } else {
      goto L17;
    }
  L17:
    h__ *= 2;
    f[0] = f[4];
    f[1] = f[5];
    f[2] = f[6];
    for (k = 4; k <= 7; ++k) {
      /* L19: */
      f[k - 1] = 1e17f;
    }
    goto L18;
  L14:
    f[0] = f[4];
    f[2] = f[5];
    f[4] = f[6];
    f[1] = 1e17f;
    f[3] = 1e17f;
    f[5] = 1e17f;
    f[6] = 1e17f;
  L18:
    di1 = di2 + (di2 - di1) / 15;
    result += di1;
    aih += di2;
    aiabs += di3;
    goto L22;
  L21:
    h__ /= 2;
    f[6] = f[4];
    f[5] = f[3];
    f[4] = f[2];
    f[2] = f[1];
    f[1] = 1e17f;
    f[3] = 1e17f;
    x    = x0;
    c__  = 0.f;
    goto L5;
  L22:
    if (c__ != 0.f) {
      goto L2;
    } else {
      goto L4;
    }
  L2:
    return result;
  }

  Double_t CorrFit1DCFCumac::Sign(Double_t a, Double_t b) const {
    if (b >= 0) return TMath::Abs(a);
    return -TMath::Abs(a);
  }
  //*****************************************************************************//

  CorrFIt1DCFCumacLamLam::CorrFIt1DCFCumacLamLam() : CorrFit1DCFCumac(8) {}

  Double_t CorrFIt1DCFCumacLamLam::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t d0, f0, r0, ai, ak, al, an, ex, f0m, r0m, zs, den, fim, fre, fsi, r_s__, r_t__, fsq, pol, r0res, alres;
    Double_t ret_val, r__1;
    switch (fKinematics) {
      case Femto::EKinematics::kPRF: ak = x[0]; break;
      case Femto::EKinematics::kLCMS: ak = x[0] * 0.5; break;
      default: ak = 0.0; break;
    }
    an    = params[NormID()];
    al    = params[LambdaID()];
    r0    = params[RadiusID()];
    f0    = -params[ScatteringLengthID()];
    d0    = params[EffectiveRadiusID()];
    pol   = params[LambdaPolarizationID()];
    alres = params[ResidualAmplitudeID()];
    r0res = params[ResidualGaussWidhtID()];
    f0m   = 1E+6;
    r0m   = 1E+6;
    if (f0 != 0.0) { f0m = .1973 / f0 + d0 * .5 * ak * ak / .1973; }
    if (r0 != 0.0) { r0m = .1973 / r0; }
    den = f0m * f0m + ak * ak;
    fre = f0m / den;
    fim = ak / den;
    fsq = fre * fre + fim * fim;
    fZ  = (ak * 2.0) / r0m;
    zs  = fZ * fZ;
    ex  = TMath::Exp(-zs);
    fsi = 0.0;
    if (params[RadiusID()] != 0.0) {
      ai = Integr();
      //	integr_(ai, fZ_1);
      fsi = fsq * (r0m * r0m) * (1 - d0 / (r0 * 3.5449077020000002f)) + fre * 4 * r0m * ai / 1.772453851f;
      fsi -= fim * 2 * r0m * (1 - ex) / fZ;
    }
    r_t__   = (pol * pol + 3) * .25f * (1 - al * ex);
    r_s__   = (1 - pol * pol) * .25f * (al * (ex + fsi) + 1);
    r__1    = ak * 2.0 * r0res / .1973f;
    ret_val = an * (r_s__ + r_t__ + alres * exp(-(r__1 * r__1)));
    return ret_val;
  }

  CorrFIt1DCFCumacLamLam::~CorrFIt1DCFCumacLamLam() {}
  //******************************************************************************
  CorrFit1DCFCumacPLam::CorrFit1DCFCumacPLam() : CorrFit1DCFCumac(8) {
    SetParameterName(SingletScatteringLengthID(), "f_{0s}");
    SetParameterName(SingletEffectiveRadiusID(), "d_{0s}");
    SetParameterName(TripletScatteringLenghtID(), "f_{0t}");
    SetParameterName(TripletEffectiveRadiusID(), "d_{0t}");
    SetParameterName(LambdaPolarizationID(), "POL");
    FixParameter(SingletScatteringLengthID(), 2.31);
    FixParameter(SingletEffectiveRadiusID(), 3.04);
    FixParameter(TripletScatteringLenghtID(), 1.78);
    FixParameter(TripletEffectiveRadiusID(), 3.22);
    FixParameter(LambdaPolarizationID(), 0);
  }

  CorrFit1DCFCumacPLam::~CorrFit1DCFCumacPLam() {}

  Double_t CorrFit1DCFCumacPLam::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t ak;
    switch (fKinematics) {
      case Femto::EKinematics::kPRF: ak = x[0]; break;
      case Femto::EKinematics::kLCMS: ak = x[0] * 0.5; break;
      default: ak = 0.0; break;
    }

    Double_t al = params[LambdaID()];
    // c      r0=p(2)
    Double_t r0   = params[RadiusID()];
    Double_t f0_s = params[SingletScatteringLengthID()];
    Double_t d0_s = params[SingletEffectiveRadiusID()];
    Double_t f0_t = params[TripletScatteringLenghtID()];
    Double_t d0_t = params[TripletEffectiveRadiusID()];
    Double_t POL  = params[LambdaPolarizationID()];

    Double_t r0m   = fHc / r0;
    Double_t f0m_s = (fHc / f0_s + 0.5 * ak * ak * d0_s / fHc);
    Double_t den_s = TMath::Power(f0m_s, 2) + TMath::Power(ak, 2);
    Double_t fre_s = f0m_s / den_s;
    Double_t fim_s = ak / den_s;
    Double_t fsq_s = TMath::Power(fre_s, 2) + TMath::Power(fim_s, 2);

    Double_t f0m_t = (fHc / f0_t + 0.5 * ak * ak * d0_t / fHc);
    Double_t den_t = TMath::Power(f0m_t, 2) + TMath::Power(ak, 2);
    Double_t fre_t = f0m_t / den_t;
    Double_t fim_t = ak / den_t;
    Double_t fsq_t = TMath::Power(fre_t, 2) + TMath::Power(fim_t, 2);

    fZ             = 2. * ak / r0m;
    Double_t zs    = fZ * fZ;
    Double_t ex    = TMath::Exp(-zs);
    Double_t ai    = Integr();
    Double_t fsi_s = 0.5 * (fsq_s * TMath::Power(r0m, 2) * (1. - d0_s / (2. * fPis * r0)) + 4. * fre_s * r0m * ai / fPis);
    fsi_s          = fsi_s - fim_s * r0m * (1. - ex) / fZ;
    Double_t fsi_t = 0.5 * (fsq_t * TMath::Power(r0m, 2) * (1. - d0_t / (2. * fPis * r0)) + 4. * fre_t * r0m * ai / fPis);
    fsi_t          = fsi_t - fim_t * r0m * (1. - ex) / fZ;
    Double_t R_t   = .25 * (3. + TMath::Power(POL, 2)) * (1. + al * fsi_t);
    Double_t R_s   = .25 * (1. - TMath::Power(POL, 2)) * (1. + al * fsi_s);
    return params[NormID()] * (R_t + R_s);
  }
  /******************************************************************************/
  Double_t CorrFit1DCFCumacK0K0::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t ak = 0;
    switch (fKinematics) {
      case Femto::EKinematics::kPRF: ak = x[0]; break;
      case Femto::EKinematics::kLCMS: ak = x[0] * 0.5; break;
      default: ak = 0.0; break;
    }
    const Double_t mK2   = Const::KaonZeroMass() * Const::KaonZeroMass();
    const Double_t mpi2  = Const::PionPlusMass() * Const::PionPlusMass();
    const Double_t meta2 = 0.547862 * 0.547862;
    const Double_t ak2   = ak * ak;
    Double_t s           = 4. * (ak2 + mK2);
    Double_t U           = TMath::Sqrt(s);
    Double_t k1prim      = TMath::Sqrt(ak * ak + mK2 - mpi2);
    Double_t e           = TMath::Sqrt(ak2 + mK2) * 2.0;
    // Double_t k3prim =
    //   TMath::Sqrt(mpi2 * mpi2 - 2.0 * mpi2 * meta2 - 2.0 * e * e * mpi2 + meta2 * meta2 - 2.0 * e * e * meta2 + e * e * e * e)
    //   / (2.0 * e);

    Double_t k2prim = TMath::Sqrt(mpi2 * mpi2 + meta2 * meta2 + s * s - 2.0 * (mpi2 * meta2 + mpi2 * s + meta2 * s)) / (2.0 * U);

    TComplex num1(params[Mf0ID()] * params[Mf0ID()] - s, -params[Gamma_f0KKID()] * ak - params[Gamma_f0pipiID()] * k1prim);
    TComplex num2(params[Ma0ID()] * params[Ma0ID()] - s, -params[Gamma_a0KKID()] * ak - params[Gamma_a0PiEtaID()] * k2prim);

    TComplex f_1(params[Gamma_f0KKID()], 0);
    TComplex f_2(params[Gamma_a0KKID()], 0);
    f_1          = f_1 / num1;
    f_2          = f_2 / num2;
    TComplex fk  = 0.5 * (f_1 + f_2);
    Double_t r   = params[RadiusID()] * Femto::FmToGeV();
    Double_t qr  = ak * r * 2.;
    Double_t fkM = (fk / r).Rho();

    Double_t strong =
      params[AssymetryID()] * (fkM * fkM + 4. * fk.Re() / (fPis * r) * F1(qr) - 2.0 * fk.Im() * F2(qr) / (r));  // brakowalo pi?
    return params[NormID()] * (1 + params[LambdaID()] * (TMath::Exp(-qr * qr) + strong));
  }

  CorrFit1DCFCumacK0K0::CorrFit1DCFCumacK0K0() : CorrFit1DCFCumac(10) {
    SetParameterName(Mf0ID(), "m_{f_0}");
    SetParameterName(Ma0ID(), "m_{a_0}");
    SetParameterName(Gamma_f0KKID(), "#gamma_{f_{0K#bar{K}}");
    SetParameterName(Gamma_a0KKID(), "#gamma_{a_{0K#bar{K}}");
    SetParameterName(Gamma_f0pipiID(), "#gamma_{f_{0#pi#pi}");
    SetParameterName(Gamma_a0PiEtaID(), "#gamma_{a_{0#pi#eta}}");
    SetParameterName(AssymetryID(), "#alpha");
    SetDefParams(3);
  }

  void CorrFit1DCFCumacK0K0::SetDefParams(Int_t opt) {
    Double_t params[5][6] = {{0.978, 0.792, 0.199, 0.974, 0.333, 0.222},
                             {0.973, 2.763, 0.5283, 0.985, 0.4038, 0.3711},
                             {0.996, 1.305, 0.2684, 0.992, 0.5555, 0.4401},
                             {0.996, 1.305, 0.2684, 1.003, 0.8365, 0.4580},
                             {}};
    for (int i = 0; i < 6; i++) {
      Double_t val = 0;
      for (int j = 0; j < 4; j++)
        val += params[j][i];
      params[4][i] = val / 4.0;
    }

    if (opt > 4) opt = 4;
    FixParameter(Mf0ID(), params[opt][0]);
    FixParameter(Gamma_f0KKID(), params[opt][1]);
    FixParameter(Gamma_f0pipiID(), params[opt][2]);
    FixParameter(Ma0ID(), params[opt][3]);
    FixParameter(Gamma_a0KKID(), params[opt][4]);
    FixParameter(Gamma_a0PiEtaID(), params[opt][5]);
    FixParameter(AssymetryID(), 0.5);
  }

  //==================================================================
  Double_t CorrFit1DCFCumacK0Kch::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t ak = 0;
    switch (fKinematics) {
      case Femto::EKinematics::kPRF: ak = x[0]; break;
      case Femto::EKinematics::kLCMS: ak = x[0] * 0.5; break;
      default: ak = 0.0; break;
    }
    const Double_t mK2   = Const::KaonZeroMass() * Const::KaonZeroMass();
    const Double_t mpi2  = Const::PionPlusMass() * Const::PionPlusMass();
    const Double_t meta2 = 0.547862 * 0.547862;
    const Double_t ak2   = ak * ak;
    Double_t s           = 4. * (ak2 + mK2);
    Double_t U           = TMath::Sqrt(s);
    Double_t k1prim      = TMath::Sqrt(ak * ak + mK2 - mpi2);
    Double_t e           = TMath::Sqrt(ak2 + mK2) * 2.0;
    Double_t k3prim =
      TMath::Sqrt(mpi2 * mpi2 - 2.0 * mpi2 * meta2 - 2.0 * e * e * mpi2 + meta2 * meta2 - 2.0 * e * e * meta2 + e * e * e * e)
      / (2.0 * e);

    Double_t k2prim = TMath::Sqrt(mpi2 * mpi2 + meta2 * meta2 + s * s - 2.0 * (mpi2 * meta2 + mpi2 * s + meta2 * s)) / (2.0 * U);
    double k2prim_old = k2prim;
    k2prim            = fGammaCalc.Calculate(ak);
    std::cout << "k- " << k2prim_old << " " << k2prim << std::endl;
    TComplex num(params[Ma0ID()] * params[Ma0ID()] - s, -params[Gamma_a0KKID()] * ak - params[Gamma_a0PiEtaID()] * k2prim);
    TComplex f(params[Gamma_a0KKID()], 0);
    f            = f / num;
    TComplex fk  = f;
    Double_t r   = params[RadiusID()] * Femto::FmToGeV();
    Double_t qr  = ak * r * 2.;
    Double_t fkM = (fk / r).Rho();

    Double_t strong = 0.25 * (fkM * fkM + 4.0 * fk.Re() / (fPis * r) * F1(qr) - 2.0 * fk.Im() * F2(qr) / (r));

    double val = params[NormID()] * (1 + params[LambdaID()] * strong);
    std::cout << ak << " fk " << fk.Re() << " " << fk.Im() << " " << F1(qr) << std::endl;

    return val;
  }

  CorrFit1DCFCumacK0Kch::CorrFit1DCFCumacK0Kch() : CorrFit1DCFCumac(10) {
    SetParameterName(Ma0ID(), "m_{a_0}");
    SetParameterName(Gamma_a0KKID(), "#gamma_{a_{0K#bar{K}}");
    SetParameterName(Gamma_a0PiEtaID(), "#gamma_{a_{0#pi#eta}}");
    SetDefParams(3);
  }

  void CorrFit1DCFCumacK0Kch::SetDefParams(Int_t opt) {
    Double_t params[5][3] = {
      {0.985, 0.4038, 0.3711}, {0.992, 0.5555, 0.4401}, {1.003, 0.8365, 0.4580}, {0.974, 0.3330, 0.2220}, {}};

    //.9467,.9698,2.763,
    for (int i = 0; i < 3; i++) {
      Double_t val = 0;
      for (int j = 0; j < 4; j++)
        val += params[j][i];
      params[4][i] = val / 4.0;
    }

    if (opt > 4) opt = 4;
    fGammaCalc.ReInit(Const::KaonZeroMass(), Const::KaonZeroMass(), Const::PionPlusMass(), 0.547862);
    FixParameter(Gamma_a0KKID(), params[opt][1]);
    FixParameter(Gamma_a0PiEtaID(), params[opt][2]);
    FixParameter(Ma0ID(), params[opt][0]);
    for (int i = 0; i < GetParametersNo(); i++) {
      fTempParamsEval[i] = fParameters[i].GetMin();
    }
  }

  Double_t CorrFit1DCFCumacDLam::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t ak = 0;
    switch (fKinematics) {
      case Femto::EKinematics::kPRF: ak = x[0]; break;
      case Femto::EKinematics::kLCMS: ak = x[0] * 0.5; break;
      default: ak = 0.0; break;
    }

    Double_t f0_d  = params[DoubletScatteringLengthID()];
    Double_t d0_d  = params[DoubletEffectiveRadiusID()];
    Double_t f0_q  = params[QuartetScatteringLenghtID()];
    Double_t d0_q  = params[QuartetEffectiveRadiusID()];
    Double_t r0    = Femto::FmToGeV(params[RadiusID()]);
    Double_t q     = 2.0 * ak;
    auto component = [&](double f0, double d0) {
      f0 = Femto::FmToGeV(f0);
      d0 = Femto::FmToGeV(d0);
      // TComplex tmp = TComplex(d0 * ak * ak * 0.5, -ak) - 1.0 / f0;  // https://arxiv.org/pdf/2005.05012
      TComplex tmp = TComplex(d0 * ak * ak * 0.5, -ak) + 1.0 / f0;  // Yu Hu
      auto fk      = TComplex(1.0, 0) / tmp;
      return (fk * fk).Rho() * F(d0, r0) / (2.0 * r0 * r0) + 2.0 * fk.Re() / (fPis * r0) * F1(q * r0) - fk.Im() / r0 * F2(q * r0);
    };
    // std::cout << "FK " << component(f0_d, d0_d) << " " << component(f0_q, d0_q) << std::endl;
    Double_t strong = (component(f0_d, d0_d) + 2.0 * component(f0_q, d0_q)) / 3.0;
    return params[NormID()] * (1.0 + params[LambdaID()] * strong);
  }

  CorrFit1DCFCumacDLam::CorrFit1DCFCumacDLam() : CorrFit1DCFCumac(7) {
    SetParameterName(DoubletScatteringLengthID(), "f_{0d}");
    SetParameterName(DoubletEffectiveRadiusID(), "d_{0d}");
    SetParameterName(QuartetScatteringLenghtID(), "f_{0q}");
    SetParameterName(QuartetEffectiveRadiusID(), "d_{0q}");
    FixParameter(DoubletScatteringLengthID(), 2.31);
    FixParameter(DoubletEffectiveRadiusID(), 3.04);
    FixParameter(QuartetScatteringLenghtID(), 1.78);
    FixParameter(QuartetEffectiveRadiusID(), 3.22);
  }


  Double_t CorrFit1DCFCumac::Get(Double_t q, Double_t r) {
    Double_t val[1];
    val[0] = q;
    Double_t params[GetParametersNo()];
    for (int i = 0; i < GetParametersNo(); i++)
      params[i] = GetParameter(i);
    FixParameter(RadiusID(), r);
    return CalculateCF(val, params);
  }

  Double_t CorrFit1DCFCumacStrong::CalculateCF(const Double_t* x, const Double_t* params) const {
    Double_t ak = 0;
    switch (fKinematics) {
      case Femto::EKinematics::kPRF: ak = x[0]; break;
      case Femto::EKinematics::kLCMS: ak = x[0] * 0.5; break;
      default: ak = 0.0; break;
    }
    Double_t r0    = Femto::FmToGeV(params[RadiusID()]);
    Double_t q     = 2.0 * ak;
    auto component = [&](double f0, double d0) {
      f0 = Femto::FmToGeV(f0);
      d0 = Femto::FmToGeV(d0);
      // TComplex tmp = TComplex(d0 * ak * ak * 0.5, -ak) - 1.0 / f0;  // https://arxiv.org/pdf/2005.05012
      TComplex tmp = TComplex(d0 * ak * ak * 0.5, -ak) + 1.0 / f0;  // Yu Hu
      auto fk      = TComplex(1.0, 0) / tmp;
      return (fk * fk).Rho() * F(d0, r0) / (2.0 * r0 * r0) + 2.0 * fk.Re() / (fPis * r0) * F1(q * r0) - fk.Im() / r0 * F2(q * r0);
    };
    double cf = 0;
    for (int iSpin = 0; iSpin < fSpinStates; iSpin++) {
      cf += params[SpinWeightID(iSpin)] * component(params[F0ID(iSpin)], params[D0ID(iSpin)]);
    }
    return params[NormID()] * (1.0 + params[LambdaID()] * cf);
  }

  CorrFit1DCFCumacStrong::CorrFit1DCFCumacStrong(Int_t spinStates) :
    CorrFit1DCFCumac(3 + spinStates * 3), fSpinStates(spinStates) {
    fNormParIndex   = 0;
    fLambdaParIndex = 1;
    fRinvParIndex   = 2;
    SetParameterName(LambdaID(), "#lambda");
    SetParameterName(NormID(), "N");
    SetParameterName(RadiusID(), "R");
    int start_par = 3;
    for (int i = 0; i < fSpinStates; i++) {
      SetParameterName(start_par++, Form("f0_%i", i));
      SetParameterName(start_par++, Form("d0_%i", i));
      SetParameterName(start_par++, Form("frac_%i", i));
    }
  }

  Int_t CorrFit1DCFCumacStrong::SpinWeightID(Int_t spin) const { return 3 + spin * 3 + 2; }

  Int_t CorrFit1DCFCumacStrong::D0ID(Int_t spin) const { return 3 + spin * 3 + 1; }

  Int_t CorrFit1DCFCumacStrong::F0ID(Int_t spin) const { return 3 + spin * 3; }

}  // namespace Hal
