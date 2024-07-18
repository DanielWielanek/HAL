/*
 * FemtoWeightGeneratorDummy.cxx
 *
 *  Created on: 8 lip 2024
 *      Author: daniel
 */

#include "FemtoWeightGeneratorDummy.h"

#include <RtypesCore.h>
#include <TComplex.h>
#include <TMath.h>
#include <complex>
#include <iostream>

#include "Const.h"
#include "FemtoConst.h"
#include "Std.h"

namespace Hal {

  FemtoWeightGeneratorK0K0::FemtoWeightGeneratorK0K0() {}

  Double_t FemtoWeightGeneratorK0K0::GenerateWeight(FemtoPair* pair) {
    CalculateKinematics(pair);


    const Double_t mK2   = Const::KaonZeroMass() * Const::KaonZeroMass();
    const Double_t mpi2  = Const::PionPlusMass() * Const::PionPlusMass();
    const Double_t meta2 = 0.547862 * 0.547862;
    const Double_t ak2   = fKStar * fKStar;
    Double_t s           = 4. * (ak2 + mK2);
    Double_t U           = TMath::Sqrt(s);
    Double_t k1prim      = TMath::Sqrt(fKStar * fKStar + mK2 - mpi2);
    Double_t e           = TMath::Sqrt(ak2 + mK2) * 2.0;
    Double_t k3prim =
      TMath::Sqrt(mpi2 * mpi2 - 2.0 * mpi2 * meta2 - 2.0 * e * e * mpi2 + meta2 * meta2 - 2.0 * e * e * meta2 + e * e * e * e)
      / (2.0 * e);

    Double_t k2prim = TMath::Sqrt(mpi2 * mpi2 + meta2 * meta2 + s * s - 2.0 * (mpi2 * meta2 + mpi2 * s + meta2 * s)) / (2.0 * U);

    TComplex num1(fMf0 * fMf0 - s, -fGamma_f0KK * fKStar - fGamma_f0pipi * k1prim);
    TComplex num2(fMa0 * fMa0 - s, -fGamma_a0KK * fKStar - fGamma_a0pieta * k2prim);

    TComplex f_1(fGamma_f0KK, 0);
    TComplex f_2(fGamma_a0KK, 0);
    f_1            = f_1 / num1;
    f_2            = f_2 / num2;
    TComplex fk    = 0.5 * (f_1 + f_2);
    Double_t tRSS  = Hal::Femto::FmToGeV(fRStarSide);
    Double_t tROS  = Hal::Femto::FmToGeV(fRStarOut);
    Double_t tRLS  = Hal::Femto::FmToGeV(fRStarLong);
    Double_t krvec = fKStarOut * tROS + fKStarSide * tRSS + fKStarLong * tRLS;
    TComplex ikrPlus(0, -krvec);
    TComplex ikrScalarPlus(0, TMath::Abs(fKStar * Hal::Femto::FmToGeV(fRStar)));
    TComplex rescattered1 = TComplex::Exp(ikrScalarPlus) / TMath::Abs(Hal::Femto::FmToGeV(fRStar));
    TComplex rescattered2 = TComplex::Exp(TComplex::Conjugate(ikrScalarPlus)) / TMath::Abs(Hal::Femto::FmToGeV(fRStar));
    TComplex psiK0K0bar   = TComplex::Exp(ikrPlus) + fAssymetry * fk * rescattered1;
    TComplex psiK0K0bar2  = TComplex::Exp(-ikrPlus) + fAssymetry * fk * rescattered2;


    Double_t psiK0K0 = (psiK0K0bar + psiK0K0bar2) * (psiK0K0bar + psiK0K0bar2) * 0.5;  // 0.5 - normalization
    /*   Double_t quantum = 1.0 + TMath::Cos(2.0 * (fKStarOut * tROS + fKStarSide * tRSS + fKStarLong * tRLS));

       double fr    = fk.Re() / (fk.Re() * fk.Re() + fk.Im() * fk.Im());
       double fi    = -fk.Im() / (fk.Im() * fk.Re() + fk.Im() * fk.Im());
       double kstar = fKStar;
       double rstar = Femto::FmToGeV(fRStar);

       dr = 0.5 * fD0s.re * kstar * kstar;

       double ir = fr + dr;
       double ii = fi - kstar;

       double ar = ir / (ir * ir + ii * ii);
       double ai = -ii / (ir * ir + ii * ii);

       double srk = sin(kstar * rstar);
       double crk = cos(kstar * rstar);

       double sgrp = (ar * crk - ai * srk) / rstar;
       double sgip = (ai * crk + ar * srk) / rstar;

       double srm = sin(kstar * rstar);
       double crm = cos(kstar * rstar);

       double sgrm = (ar * crm - ai * srm) / rstar;
       double sgim = (ai * crm + ar * srm) / rstar;

       double skr = sin(krvec);
       double ckr = cos(krvec);

       double skm = sin(-krvec);
       double ckm = cos(-krvec);

       double snr = ckr + sgrp + ckm + sgrm;
       double sni = skr + sgip + skm + sgim;

       double singlet = 0.5 * (snr * snr + sni * sni);


       //   return (0.25 * ((ckr+sr)*(ckr+sr) + (skr+si)*(skr+si)) +
       //      0.75 * ((ckr+tr)*(ckr+tr) + (skr+ti)*(skr+ti)));

       //   std::cout << "sng tri full " << singlet << " "<< triplet << "  "  << (0.25 * singlet + 0.75 * triplet) << std::endl;

       return 0.25 * singlet;*/
    return psiK0K0;  // * 0.5 + quantum * 0.5;
    ;
  }

  void FemtoWeightGeneratorK0K0::SetDefParams(Int_t opt) {
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
    fMf0           = params[opt][0];
    fGamma_f0KK    = params[opt][1];
    fGamma_f0pipi  = params[opt][2];
    fMa0           = params[opt][3];
    fGamma_a0KK    = params[opt][4];
    fGamma_a0pieta = params[opt][5];
    fAssymetry     = 0.5;
  }

} /* namespace Hal */
