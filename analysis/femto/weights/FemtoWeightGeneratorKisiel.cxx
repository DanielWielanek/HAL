/*
 * FemtoWeightGeneratorKisiel.cxx
 *
 *  Created on: 30 sie 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoWeightGeneratorKisiel.h"

#include "FemtoPair.h"
#include "Package.h"
#include "Parameter.h"

#include <TMath.h>

#include <cmath>
#include <iostream>

#define COULOMBSTEPS 170

namespace Hal {
  FemtoWeightGeneratorKisiel::FemtoWeightGeneratorKisiel() :
    FemtoWeightGenerator(),
    fPionac(0),
    fOneoveracsq(0),
    fTwopioverac(0),
    fCoulqscpart(0),
    fTwospin(0),
    fWritegrps(0),
    fPcount(0),
    fEuler(0),
    fF0(0),
    fD0(0),
    fRStarOutS(0),
    fRStarSideS(0),
    fRStarLongS(0),
    fRStarS(0) {}

  void FemtoWeightGeneratorKisiel::InitializeGamow() {
    fTwospin = 0;

    fEuler = 0.577215665;
    fF0    = 7.77 / 0.197327;
    fD0    = 2.77 / 0.197327;

    fD0s.re = 0.0;
    fD0s.im = 0.0;
    fD0t.re = 0.0;
    fD0t.im = 0.0;

    fF0s.re = 0.0;
    fF0s.im = 0.0;
    fF0t.re = 0.0;
    fF0t.im = 0.0;

    static unsigned short int isospin = 0;
    switch (fPairType) {
      case Femto::EPairType::kPionPlusPionPlus: {  // 0
        fPionac = 387.5 / 0.197327;
      } break;
      case Femto::EPairType::kKaonPlusKaonPlus: {  // 1
        fPionac = 109.55 / 0.197327;
      } break;
      case Femto::EPairType::kPionPlusKaonPlus: {  // 2
        fPionac    = 248.519 / 0.197327;
        fWritegrps = 0;
      } break;
      case Femto::EPairType::kPionPlusKaonMinus: {  // 3
        fPionac    = -248.519 / 0.197327;
        fWritegrps = 0;
      } break;
      case Femto::EPairType::kPionPlusProton: {  // 4
        fPionac    = 222.564 / 0.197327;
        fWritegrps = 0;
      } break;
      case Femto::EPairType::kPionPlusAntiproton: {  // 5
        fPionac    = -222.564 / 0.197327;
        fWritegrps = 0;
      } break;
      case Femto::EPairType::kProtonSigmaPlus: {  // 6
        fPionac  = 51.5553 / 0.197327;
        fTwospin = 1;
        // Rijken, Nagels, Yamamoto ESC08 model PTPS No. 185, 2010
        fF0s.re = 3.85 / 0.197327;
        fD0s.re = 3.40 / 0.197327;
        fF0t.re = -0.62 / 0.197327;
        fD0t.re = -2.13 / 0.197327;
      } break;
      case Femto::EPairType::kProtonAntiSigmaPlus: {  // 7
        fPionac = -51.5553 / 0.197327;

        // from antiproton - lambda
        fF0s.re = -1.20 / 0.197327;
        fF0s.im = 0.37 / 0.197327;
        fF0t.re = -1.20 / 0.197327;
        fF0t.im = 0.37 / 0.197327;
      } break;
      case Femto::EPairType::kProtonLambda: {  // 8
        // ESC08
        fF0s.re = 2.70 / 0.197327;
        fD0s.re = 2.97 / 0.197327;
        fF0t.re = 1.65 / 0.197327;
        fD0t.re = 3.63 / 0.197327;
      } break;
      case Femto::EPairType::kProtonAntiLambda: {  // 9
        fPionac = 0.0;

        fF0s.re = -1.20 / 0.197327;
        fF0s.im = 0.37 / 0.197327;
        fF0t.re = -1.20 / 0.197327;
        fF0t.im = 0.37 / 0.197327;
      } break;
      case Femto::EPairType::kSigmaPlusSigmaPlus: {  // 10
        fPionac  = 45.4709 / 0.197327;
        fTwospin = 1;
      } break;
      case Femto::EPairType::kSigmaPlusAntiSigmaPlus: {  // 11
        fPionac = -45.4709 / 0.197327;
      } break;
      case Femto::EPairType::kProtonProton: {  // 12
        fPionac  = 57.63975274 / 0.197327;
        fTwospin = 1;

        //    d0s.re = 2.77 / 0.197327;
        //    f0s.re = 7.77 / 0.197327;
        fD0t.re = 1.7 / 0.197327;
        fF0t.re = -5.4 / 0.197327;

        // ESC08
        fF0s.re = 7.771 / 0.197327;
        fD0s.re = 2.754 / 0.197327;
        //    f0t.re = ? / 0.197327; no data from ESC08
        //    d0t.re = ? / 0.197327;
      } break;
      case Femto::EPairType::kKaonPlusProton: {  // 13
        fPionac = 83.59432006 / 0.197327;
      } break;
      case Femto::EPairType::kKaonPlusAntiproton: {  // 14
        fPionac = -83.59432006 / 0.197327;
      } break;
      case Femto::EPairType::kPionPlusPionMinus: {  // 15
        fPionac = -387.5 / 0.197327;
      } break;
        /*   case Femto::EPairType::kPionPlusPionPlus: {//16 ?? same
      pionac = -387.5 / 0.197327;
           } break;*/
      case Femto::EPairType::kLambdaLambda: {  // 17
        fPionac = 0;

        // ESC08
        fF0s.re = 0.88 / 0.197327;
        fD0s.re = 4.34 / 0.197327;
        fF0t.re = 0.88 / 0.197327;
        fD0t.re = 4.34 / 0.197327;
      } break;
      case Femto::EPairType::kProtonXiZero: {  // 18
        fPionac = 0;

        // ESC08
        if (isospin == 0) {
          fF0s.re = 0.0;
          fD0s.re = 0.0;
          fF0t.re = -6.90 / 0.197327;
          fD0t.re = 1.18 / 0.197327;
          isospin = 1;
        } else if (isospin == 1) {
          fF0s.re = -0.58 / 0.197327;
          fD0s.re = -2.71 / 0.197327;
          fF0t.re = -3.49 / 0.197327;
          fD0t.re = 0.60 / 0.197327;
          isospin = 0;
        }
      } break;
      case Femto::EPairType::kNeutronXiMinus: {  // 19
        fPionac = 0;

        // ESC08
        if (isospin == 0) {
          fF0s.re = 0.0;
          fD0s.re = 0.0;
          fF0t.re = -6.90 / 0.197327;
          fD0t.re = 1.18 / 0.197327;
          isospin = 1;
        } else if (isospin == 1) {
          fF0s.re = -0.58 / 0.197327;
          fD0s.re = -2.71 / 0.197327;
          fF0t.re = -3.49 / 0.197327;
          fD0t.re = 0.60 / 0.197327;
          isospin = 0;
        }
      } break;
      case Femto::EPairType::kProtonXIMInus: {  // 20
        fPionac = 49.2788901 / 0.197327;

        // ESC08
        if (isospin == 0) {
          fF0s.re = 0.0;
          fD0s.re = 0.0;
          fF0t.re = -6.90 / 0.197327;
          fD0t.re = 1.18 / 0.197327;
          isospin = 1;
        } else if (isospin == 1) {
          fF0s.re = -0.58 / 0.197327;
          fD0s.re = -2.71 / 0.197327;
          fF0t.re = -3.49 / 0.197327;
          fD0t.re = 0.60 / 0.197327;
          isospin = 0;
        }
      } break;
      case Femto::EPairType::kNeutronXiZero: {  // 21
        fPionac = 0;

        // ESC08
        if (isospin == 0) {
          fF0s.re = 0.0;
          fD0s.re = 0.0;
          fF0t.re = -6.90 / 0.197327;
          fD0t.re = 1.18 / 0.197327;
          isospin = 1;
        } else if (isospin == 1) {
          fF0s.re = -0.58 / 0.197327;
          fD0s.re = -2.71 / 0.197327;
          fF0t.re = -3.49 / 0.197327;
          fD0t.re = 0.60 / 0.197327;
          isospin = 0;
        }
      } break;
      case Femto::EPairType::kNeutronProton: {  // 22
        fPionac = 0;

        // ESC08
        fF0s.re = 23.735 / 0.197327;
        fD0s.re = 2.694 / 0.197327;
        fF0t.re = -5.428 / 0.197327;
        fD0t.re = 1.753 / 0.197327;
      } break;
      case Femto::EPairType::kNeutronNeutron: {  // 23
        fPionac = 0;

        // ESC08
        fF0s.re = 16.51 / 0.197327;
        fD0s.re = 2.85 / 0.197327;
      } break;
      case Femto::EPairType::kProtonSigmaZero: {  // 24
        fPionac = 0.0;

        // ESC08
        fF0s.re = 2.70 / 0.197327;
        fD0s.re = 2.97 / 0.197327;
        fF0t.re = 1.65 / 0.197327;
        fD0t.re = 3.63 / 0.197327;
      } break;
      case Femto::EPairType::kSigmaZeroSigmaZero: {  // 25
        fPionac = 0;

        // ESC08
        fF0s.re = 0.88 / 0.197327;
        fD0s.re = 4.34 / 0.197327;
        fF0t.re = 0.88 / 0.197327;
        fD0t.re = 4.34 / 0.197327;
      } break;
      case Femto::EPairType::kLambdaSigmaZero: {  // 26
        fPionac = 0;
        // ESC08
        fF0s.re = 0.88 / 0.197327;
        fD0s.re = 4.34 / 0.197327;
        fF0t.re = 0.88 / 0.197327;
        fD0t.re = 4.34 / 0.197327;
      } break;
      default: fPionac = 0; break;
    }
    fOneoveracsq = 1.0 / (fPionac * fPionac);
    fTwopioverac = 2.0 * TMath::Pi() / fPionac;
    double tpaoverk;

    for (int iter = 0; iter < 2000; iter++) {
      tpaoverk = fTwopioverac / (iter * 0.0002 + 0.0001);
      //  gamov[iter] = tpaoverk * 1.0 / (exp(tpaoverk) - 1);
    }
  }

  void FemtoWeightGeneratorKisiel::PairKinematics(FemtoPair* pair) {
    Double_t tPx = pair->TruePx1() + pair->TruePx2();
    Double_t tPy = pair->TruePy1() + pair->TruePy2();
    Double_t tPz = pair->TruePz1() + pair->TruePz2();
    Double_t tE1 = pair->TrueE1();
    Double_t tE2 = pair->TrueE2();
    Double_t tE  = tE1 + tE2;
    Double_t tPt = tPx * tPx + tPy * tPy;
    Double_t tMt = tE * tE - tPz * tPz;  // mCVK;
    Double_t tM  = sqrt(tMt - tPt);
    tMt          = sqrt(tMt);
    tPt          = sqrt(tPt);
    // Double_t tBetat = tPt/tMt;

    // Boost to LCMS
    Double_t tBeta  = tPz / tE;
    Double_t tGamma = tE / tMt;
    fKStarLong      = tGamma * (pair->TruePz1() - tBeta * tE1);
    Double_t tE1L   = tGamma * (tE1 - tBeta * pair->TruePz1());

    fKStarOut  = (pair->TruePx1() * tPx + pair->TruePy1() * tPy) / tPt;
    fKStarSide = (-pair->TruePx1() * tPy + pair->TruePy1() * tPx) / tPt;

    fKStarOut = tMt / tM * (fKStarOut - tPt / tMt * tE1L);


    Double_t tDX    = pair->GetX1() - pair->GetX2();
    Double_t tDY    = pair->GetY1() - pair->GetY2();
    Double_t tRLong = pair->GetZ1() - pair->GetZ2();
    Double_t tDTime = pair->GetT1() - pair->GetT2();

    Double_t tROut  = (tDX * tPx + tDY * tPy) / tPt;
    Double_t tRSide = (-tDX * tPy + tDY * tPx) / tPt;

    fRStarSide  = tRSide;
    fRStarSideS = fRStarSide / 0.197327;

    fRStarLong              = tGamma * (tRLong - tBeta * tDTime);
    Double_t tDTimePairLCMS = tGamma * (tDTime - tBeta * tRLong);

    fRStarLongS = fRStarLong / 0.197327;
    // 1/.1973
    tBeta      = tPt / tMt;
    tGamma     = tMt / tM;
    fRStarOut  = tGamma * (tROut - tBeta * tDTimePairLCMS);
    fRStarOutS = fRStarOut / 0.197327;
    //   Double_t tDTimePairCMS = tGamma*(tDTimePairLCMS - tBeta* tROut);
    fRStar = ::sqrt(fRStarOut * fRStarOut + fRStarSide * fRStarSide + fRStarLong * fRStarLong);
    fKStar = ::sqrt(fKStarOut * fKStarOut + fKStarSide * fKStarSide + fKStarLong * fKStarLong);
    if (fKStarOut < 0) fKStar = -fKStar;
    fRStarS = fRStar / 0.197327;
  }

  Double_t FemtoWeightGeneratorKisiel::GenerateWeight(FemtoPair* pair) {
    double coulombweight;
    fPairType = Femto::PidToPairType(pair->GetPdg1(), pair->GetPdg2());

    InitializeGamow();
    // std::cout << "Czy sa identyczne? " << IsIdentical(localpairtype) << " z pary: "<< localpairtype << std::endl;

    if (Femto::IsPairIdentical(fPairType) == kFALSE) {
      if (fabs(fPionac) > 0.1) {

        if (fabs(fD0s.re) > 0.00001)
          coulombweight = GetCoulombStrong();
        else
          coulombweight = GetCoulomb();
      } else {
        coulombweight = GetStrong();
        if (std::isnan(coulombweight) == 1) return 1;
      }
    } else {
      if (fPairType == Femto::EPairType::kPionZeroPionZero) { return GetQuantum(); }
      if (fabs(fPionac) > 0.1) {
        if (fPairType != Femto::EPairType::kProtonProton)
          coulombweight = GetQuantumCoulomb();
        else
          coulombweight = GetQuantumCoulombStrong();
      } else {
        coulombweight = GetQuantumStrong();
      }
    }


    return coulombweight;
  }

  double FemtoWeightGeneratorKisiel::Gamow(double arg) const {
    //   if (arg<0.0001)
    //     return 0.0;
    //   if (arg > 0.4)
    long double eta = fTwopioverac / arg;
    return (eta) *1.0 / (exp(eta) - 1.0);
    //   int bin = arg / 0.0002;
    //   double b1 = bin*0.0002 + 0.0001;
    //   double b2 = bin*0.0002 + 0.0003;
    //   return ((gamov[bin+1] * (arg - b1) + gamov[bin] * (b2 - arg)) / 0.0002);
  }

  double FemtoWeightGeneratorKisiel::GetQuantumCoulombStrong() {
    if (fRStarS < 0.0000000001) return 1.0;

    if (fRStarS < 1.0 / 0.197327) return GetQuantumCoulomb();

    double tKstRst    = fKStarOut * fRStarOutS + fKStarSide * fRStarSideS + fKStarLong * fRStarLongS;
    long double kstar = fabs(fKStar);
    long double rho   = fRStarS * kstar;

    int ccase         = 0;
    static int pcount = 0;
    int wavesign      = 1;

    // Classical limit - if distance is larger than Coulomb radius,
    // the interaction does not matter
    //  if (fabs(fRStarS) > fabs(pionac)) return (1.0 + wavesign*cos(2*tKstRst));

    // Classical limit - in the case of large k* we go to
    // classical coulomb interaction
    long double testp = rho * (1.0 + tKstRst / (rho));
    long double testm = rho * (1.0 - tKstRst / (rho));

    dcomplex ffplus, ffminus;
    if ((testp > 15.0) && (testm > 15.0)) {
      double asym;
      asym = (1.0 - 1.0 / (fRStarS * (1.0 - tKstRst / rho) * fPionac * kstar * kstar)) / Gamow(kstar);
      //      std::cout << "as1 " << asym << std::endl;
      asym = sqrt(asym);
      if (asym < 1.0)
        ffminus.re = 1.0 + (asym - 1.0) * 2.0;
      else
        ffminus.re = 1.0 + (asym - 1.0) / 2.0;
      ffminus.im = sqrt(asym * asym - ffminus.re * ffminus.re);

      asym = (1.0 - 1.0 / (fRStarS * (1.0 + tKstRst / rho) * fPionac * kstar * kstar)) / Gamow(kstar);
      //      std::cout << "as2 " << asym << std::endl;
      asym = sqrt(asym);
      if (asym < 1.0)
        ffplus.re = 1.0 + (asym - 1.0) * 2.0;
      else
        ffplus.re = 1.0 + (asym - 1.0) / 2.0;
      ffplus.im = sqrt(asym * asym - ffplus.re * ffplus.re);

    }

    // Check for the classical limit in both functions separately
    else if (((testp < 15.0) && (testm < 15.0)))  // ||
    {
      // Calculate the F function
      GetFFdouble(ffplus, ffminus);
      ccase = 1;
    } else if (testp < 15.0) {
      double asym;
      GetFFsingle(ffplus, 1);
      GetFFsingle(ffminus, -1);
      if ((fabs(ffminus.re) > 2.0) || fabs(ffminus.im) > 2.0) {
        asym = (1.0 - 1.0 / (fRStarS * (1.0 - tKstRst / (rho) *fPionac * kstar * kstar))) / Gamow(kstar);
        asym = sqrt(asym);
        if (asym < 1.0)
          ffminus.re = 1.0 + (asym - 1.0) * 2.0;
        else
          ffminus.re = 1.0 + (asym - 1.0) / 2.0;
        ffminus.im = sqrt(asym * asym - ffminus.re * ffminus.re);
      }
      ccase = 2;
    } else {
      double asym;
      GetFFsingle(ffminus, -1);
      GetFFsingle(ffplus, 1);
      if ((fabs(ffplus.re) > 2.0) || fabs(ffplus.im) > 2.0) {
        asym = (1.0 - 1.0 / (fRStarS * (1.0 + tKstRst / (rho) *fPionac * kstar * kstar))) / Gamow(kstar);
        asym = sqrt(asym);
        if (asym < 1.0)
          ffplus.re = 1.0 + (asym - 1.0) * 2.0;
        else
          ffplus.re = 1.0 + (asym - 1.0) / 2.0;
        ffplus.im = sqrt(asym * asym - ffplus.re * ffplus.re);
      }
      ccase = 3;
    }

    long double eta  = 1.0 / (kstar * fPionac);
    long double hfun = GetH(eta);
    dcomplex gtilde  = GetG(eta, rho, hfun);
    dcomplex gtilor  = mult(gtilde, 1.0 / fRStarS);

    dcomplex fcouls, fcoult;
    Getfc(kstar, eta, hfun, fcouls, fcoult);

    dcomplex fgs       = mult(gtilor, fcouls);
    long double fgmods = modl2(fgs);

    dcomplex fgt       = mult(gtilor, fcoult);
    long double fgmodt = modl2(fgt);

    dcomplex expikr;
    expikr.re         = cos(tKstRst);
    expikr.im         = -sin(tKstRst);
    dcomplex expikrc  = conj(expikr);
    dcomplex ffplusc  = conj(ffplus);
    dcomplex ffminusc = conj(ffminus);

    dcomplex expikr2  = mult(expikr, expikr);
    dcomplex expikrc2 = conj(expikr2);
    dcomplex sterm    = mult(expikr2, mult(ffplus, ffminusc));
    dcomplex tterm    = mult(expikrc2, mult(ffminus, ffplusc));

    dcomplex epfpc = mult(expikr, ffplus);
    dcomplex emfmc = mult(expikrc, ffminus);

    long double fcgefhs = (fgs.re * emfmc.re + fgs.im * emfmc.im);
    long double fcgefgs = (fgs.re * epfpc.re + fgs.im * epfpc.im);

    long double fcgefht = (fgt.re * emfmc.re + fgt.im * emfmc.im);
    long double fcgefgt = (fgt.re * epfpc.re + fgt.im * epfpc.im);

    long double smult = 1 + wavesign;

    if (!std::isfinite(ffminus.re))
      std::cout << "FFMinus Re not a number ! " << testp << " " << testm << " " << ccase << std::endl;

    if (!std::isfinite(ffminus.im))
      std::cout << "FFMinus Im not a number !" << testp << " " << testm << " " << ccase << std::endl;

    if ((ffplus.re > 2.0) || (ffplus.re < -2.0))
      std::cout << std::endl << "FFplus Re wild !" << ffplus.re << " case " << ccase << " " << testp << " " << testm << std::endl;

    if ((ffplus.im > 2.0) || (ffplus.im < -2.0))
      std::cout << "FFplus Im wild !" << ffplus.im << " case " << ccase << " " << testp << " " << testm << std::endl;

    if ((ffminus.re > 2.0) || (ffminus.re < -2.0))
      std::cout << std::endl << "FFminus Re wild !" << ffminus.re << " case " << ccase << std::endl;

    if ((ffminus.im > 2.0) || (ffminus.im < -2.0))
      std::cout << "FFminus Im wild !" << ffminus.im << " case " << ccase << std::endl;

    //  coulqscpart = 0.5 * Gamow(kstar) * (modl2(ffplus) + modl2(ffminus));

    //   return (0.5 * Gamow(kstar) *
    //      (modl2(ffplus) + wavesign*sterm.re + wavesign*tterm.re + modl2(ffminus)));
    if (fTwospin == 1) {
      wavesign            = 1;
      smult               = 2;
      long double singlet = (0.5 * Gamow(kstar)
                             * (2.0 * fgmods * smult + modl2(ffplus) + modl2(ffminus) + wavesign * sterm.re + wavesign * tterm.re
                                + smult * 2 * (fcgefhs + fcgefgs)));
      wavesign            = -1;
      smult               = 0;
      long double triplet = (0.5 * Gamow(kstar)
                             * (2.0 * fgmodt * smult + modl2(ffplus) + modl2(ffminus) + wavesign * sterm.re + wavesign * tterm.re
                                + smult * 2 * (fcgefht + fcgefgt)));
      //    scmp = singlet;
      //    tcmp = triplet;

      //    ccmp = 0.5 * Gamow(kstar) * (modl2(ffplus) + modl2(ffminus));
      //    gcmp = fgmod;

      return (0.25 * singlet + 0.75 * triplet);
      //    return triplet;
    } else
      return (0.5 * Gamow(kstar)
              * (2.0 * fgmods * smult + modl2(ffplus) + modl2(ffminus) + wavesign * sterm.re + wavesign * tterm.re
                 + smult * 2 * (fcgefhs + fcgefgs)));
  }

  double FemtoWeightGeneratorKisiel::GetCoulombStrong() {
    if (fRStarS < 0.0000000001) return 1.0;

    if (fRStarS < 1.0 / 0.197327) return GetCoulomb();

    double tKstRst    = fKStarOut * fRStarOutS + fKStarSide * fRStarSideS + fKStarLong * fRStarLongS;
    long double kstar = fabs(fKStar);
    long double rho   = fRStarS * kstar;

    // Classical limit - in the case of large k* we go to
    // classical coulomb interaction
    long double testp = rho * (1.0 + tKstRst / (rho));

    dcomplex ffplus;
    if (testp > 15.0) {
      double asym;
      asym = (1.0 - 1.0 / (fRStarS * (1.0 + tKstRst / rho) * fPionac * kstar * kstar)) / Gamow(kstar);
      asym = sqrt(asym);
      if (asym < 1.0)
        ffplus.re = 1.0 + (asym - 1.0) * 2.0;
      else
        ffplus.re = 1.0 + (asym - 1.0) / 2.0;
      ffplus.im = sqrt(asym * asym - ffplus.re * ffplus.re);
    } else
      GetFFsingle(ffplus, 1);


    long double eta  = 1.0 / (kstar * fPionac);
    long double hfun = GetH(eta);
    dcomplex gtilde  = GetG(eta, rho, hfun);
    dcomplex gtilor  = mult(gtilde, 1.0 / fRStarS);

    dcomplex fcouls, fcoult;
    Getfc(kstar, eta, hfun, fcouls, fcoult);

    dcomplex fgs = mult(gtilor, fcouls);
    //  long double fgmods = modl2(fgs);

    dcomplex fgt = mult(gtilor, fcoult);
    //  long double fgmodt = modl2(fgt);

    dcomplex expikr;
    expikr.re = cos(tKstRst);
    expikr.im = -sin(tKstRst);
    //  dcomplex expikrc = conj(expikr);
    //  dcomplex ffplusc = conj(ffplus);

    //  dcomplex expikr2 = mult(expikr, expikr);
    //  dcomplex expikrc2 = conj(expikr2);

    dcomplex epfpc = mult(expikr, ffplus);
    //  dcomplex emfmc = mult(expikrc, ffminus);

    //   long double fcgefhs = (fgs.re*emfmc.re + fgs.im*emfmc.im);
    //  long double fcgefgs = (fgs.re*epfpc.re + fgs.im*epfpc.im);
    //  long double fcgefgt = (fgt.re*epfpc.re + fgt.im*epfpc.im);
    //  long double fcgefht = (fgt.re*emfmc.re + fgt.im*emfmc.im);

    dcomplex fvfs;
    fvfs.re = epfpc.re + fgs.re;
    fvfs.im = epfpc.im + fgs.im;

    dcomplex fvft;
    fvft.re = epfpc.re + fgt.re;
    fvft.im = epfpc.im + fgt.im;

    //   long double smult = 1+wavesign;

    //  return 0.5 * Gamow(kstar);
    //   return (0.5 * Gamow(kstar) *
    //      (2.0 * fgmods * smult + modl2(ffplus) + modl2(ffminus) +
    //       wavesign*sterm.re + wavesign*tterm.re +
    //       smult * 2 * (fcgefhs + fcgefgs)));

    if (fTwospin == 1) {
      //   wavesign = 1;
      //     smult = 2;
      //     long double singlet = (0.5 * Gamow(kstar) *
      //             (2.0 * fgmods * smult + modl2(ffplus) + modl2(ffminus) +
      //              wavesign*sterm.re + wavesign*tterm.re +
      //              smult * 2 * (fcgefhs + fcgefgs)));
      //     wavesign = -1;
      //     smult = 0;
      //     long double triplet = (0.5 * Gamow(kstar) *
      //             (2.0 * fgmodt * smult + modl2(ffplus) + modl2(ffminus) +
      //              wavesign*sterm.re + wavesign*tterm.re +
      //              smult * 2 * (fcgefht + fcgefgt)));
      //    scmp = singlet;
      //    tcmp = triplet;

      //    ccmp = 0.5 * Gamow(kstar) * (modl2(ffplus) + modl2(ffminus));
      //    gcmp = fgmod;

      long double singlet = Gamow(kstar) * modl2(fvfs);
      long double triplet = Gamow(kstar) * modl2(fvft);

      //    std::cout << " s t " << singlet << "  " << triplet << "   -   "  << (0.25*singlet+0.75*triplet) << std::endl;

      return (0.25 * singlet + 0.75 * triplet);
      //    return triplet;
    } else
      return Gamow(kstar) * modl2(fvfs);
    //     return (0.5 * Gamow(kstar) *
    //        (2.0 * fgmods * smult + modl2(ffplus) + modl2(ffminus) +
    //         wavesign*sterm.re + wavesign*tterm.re +
    //                smult * 2 * (fcgefhs + fcgefgs)));
  }

  double FemtoWeightGeneratorKisiel::GetStrong() {
    double fr, fi;
    double dr;
    double ar, ai;
    double ir, ii;

    double sr, si;
    double tr, ti;

    double skr, ckr;
    double srk, crk;

    double d0_sr = fD0s.re;
    double d0_si = fD0s.im;
    double d0_tr = fD0t.re;
    double d0_ti = fD0t.im;

    double f0_sr = fF0s.re;
    double f0_si = fF0s.im;
    double f0_tr = fF0t.re;
    double f0_ti = fF0t.im;

    double kstar  = fabs(fKStar);
    double rstar  = fRStarS;
    double kstrst = fKStarOut * fRStarOutS + fKStarSide * fRStarSideS + fKStarLong * fRStarLongS;
    /*
      if (pairtype == 8) {
      f0_sr = 2.88 /0.197327;
      f0_si = 0.0  /0.197327;
      d0_sr = 2.92 /0.197327;
      d0_si = 0.0  /0.197327;
      f0_tr = 1.66 /0.197327;
      f0_ti = 0.0  /0.197327;
      d0_tr = 3.78 /0.197327;
      d0_ti = 0.0  /0.197327;
      }
      else if (pairtype == 9) {
      f0_sr = -1.20 /0.197327;
      f0_si = 0.37  /0.197327;
      d0_sr = 0.0   /0.197327;
      d0_si = 0.0   /0.197327;
      f0_tr = -1.20 /0.197327;
      f0_ti = 0.37  /0.197327;
      d0_tr = 0.0   /0.197327;
      d0_ti = 0.0   /0.197327;
      }
    */
    if (rstar < 1.0 / 0.197327) return 1.0;

    fr = f0_sr / (f0_sr * f0_sr + f0_si * f0_si);
    fi = -f0_si / (f0_sr * f0_sr + f0_si * f0_si);

    dr = 0.5 * d0_sr * kstar * kstar;

    ir = fr + dr;
    ii = fi - kstar;

    ar = ir / (ir * ir + ii * ii);
    ai = -ii / (ir * ir + ii * ii);

    srk = sin(kstar * rstar);
    crk = cos(kstar * rstar);

    sr = (ar * crk - ai * srk) / rstar;
    si = (ai * crk + ar * srk) / rstar;

    fr = f0_tr / (f0_tr * f0_tr + f0_ti * f0_ti);
    fi = -f0_ti / (f0_tr * f0_tr + f0_ti * f0_ti);

    dr = 0.5 * d0_tr * kstar * kstar;

    ir = fr + dr;
    ii = fi - kstar;

    ar = ir / (ir * ir + ii * ii);
    ai = -ii / (ir * ir + ii * ii);

    tr = (ar * crk - ai * srk) / rstar;
    ti = (ai * crk + ar * srk) / rstar;

    skr = sin(kstrst);
    ckr = cos(kstrst);

    return (0.25 * ((ckr + sr) * (ckr + sr) + (skr + si) * (skr + si))
            + 0.75 * ((ckr + tr) * (ckr + tr) + (skr + ti) * (skr + ti)));
  }

  double FemtoWeightGeneratorKisiel::GetQuantumStrong() {
    double fr, fi;
    double dr;
    double ar, ai;
    double ir, ii;

    double sgrp, sgip, sgrm, sgim;
    double trrp, trip, trrm, trim;

    double skr, ckr;
    double skm, ckm;
    double srk, crk;
    double srm, crm;

    //   double d0_sr;
    //   double d0_si;
    //   double d0_tr;
    //   double d0_ti;

    //   double f0_sr;
    //   double f0_si;
    //   double f0_tr;
    //   double f0_ti;

    double kstar  = fabs(fKStar);
    double rstar  = fRStarS;
    double kstrst = fKStarOut * fRStarOutS + fKStarSide * fRStarSideS + fKStarLong * fRStarLongS;

    //   std::cout << "k o s l " << kstar << " " << fKStarOut  << " " << fKStarSide << " " << fKStarLong << std::endl;
    //   std::cout << "r o s l " << fRStarS << " " << fRStarOutS  << " " << fRStarSideS << " " << fRStarLongS << std::endl;
    //   std::cout << "kr " << kstrst << std::endl;

    double snr, sni, trr, tri;

    double singlet, triplet;

    //   if (pairtype == 8) {
    //     f0_sr = 2.88 /0.197327;
    //     f0_si = 0.0  /0.197327;
    //     d0_sr = 2.92 /0.197327;
    //     d0_si = 0.0  /0.197327;
    //     f0_tr = 1.66 /0.197327;
    //     f0_ti = 0.0  /0.197327;
    //     d0_tr = 3.78 /0.197327;
    //     d0_ti = 0.0  /0.197327;
    //   }
    //   else if (pairtype == 9) {
    //     f0_sr = -1.20 /0.197327;
    //     f0_si = 0.37  /0.197327;
    //     d0_sr = 0.0   /0.197327;
    //     d0_si = 0.0   /0.197327;
    //     f0_tr = -1.20 /0.197327;
    //     f0_ti = 0.37  /0.197327;
    //     d0_tr = 0.0   /0.197327;
    //     d0_ti = 0.0   /0.197327;
    //   }

    if (rstar < 1.0 / 0.197327) return 1.0;

    fr = fF0s.re / (fF0s.re * fF0s.re + fF0s.im * fF0s.im);
    fi = -fF0s.im / (fF0s.re * fF0s.re + fF0s.im * fF0s.im);

    dr = 0.5 * fD0s.re * kstar * kstar;

    ir = fr + dr;
    ii = fi - kstar;

    ar = ir / (ir * ir + ii * ii);
    ai = -ii / (ir * ir + ii * ii);

    srk = sin(kstar * rstar);
    crk = cos(kstar * rstar);

    sgrp = (ar * crk - ai * srk) / rstar;
    sgip = (ai * crk + ar * srk) / rstar;

    srm = sin(kstar * rstar);
    crm = cos(kstar * rstar);

    sgrm = (ar * crm - ai * srm) / rstar;
    sgim = (ai * crm + ar * srm) / rstar;

    skr = sin(kstrst);
    ckr = cos(kstrst);

    skm = sin(-kstrst);
    ckm = cos(-kstrst);

    snr = ckr + sgrp + ckm + sgrm;
    sni = skr + sgip + skm + sgim;

    singlet = 0.5 * (snr * snr + sni * sni);

    fr = fF0t.re / (fF0t.re * fF0t.re + fF0t.im * fF0t.im);
    fi = -fF0t.im / (fF0t.re * fF0t.re + fF0t.im * fF0t.im);

    dr = 0.5 * fD0t.re * kstar * kstar;

    ir = fr + dr;
    ii = fi - kstar;

    ar = ir / (ir * ir + ii * ii);
    ai = -ii / (ir * ir + ii * ii);

    trrp = (ar * crk - ai * srk) / rstar;
    trip = (ai * crk + ar * srk) / rstar;

    trrm = (ar * crm - ai * srm) / rstar;
    trim = (ai * crm + ar * srm) / rstar;

    trr = ckr + trrp - ckm - trrm;
    tri = skr + trip - skm - trim;

    triplet = 0.5 * (trr * trr + tri * tri);

    //   return (0.25 * ((ckr+sr)*(ckr+sr) + (skr+si)*(skr+si)) +
    //      0.75 * ((ckr+tr)*(ckr+tr) + (skr+ti)*(skr+ti)));

    //   std::cout << "sng tri full " << singlet << " "<< triplet << "  "  << (0.25 * singlet + 0.75 * triplet) << std::endl;

    return (0.25 * singlet + 0.75 * triplet);
  }

  double FemtoWeightGeneratorKisiel::GetQuantum() {
    double quantumweight;

    if (fTwospin == 0) {
      quantumweight = 1.0 + TMath::Cos(2.0 * (fKStarOut * fRStarOutS + fKStarSide * fRStarSideS + fKStarLong * fRStarLongS));
    } else if (fTwospin == 1) {
      if (fPcount == 3) {
        quantumweight = 1.0 + TMath::Cos(2.0 * (fKStarOut * fRStarOutS + fKStarSide * fRStarSideS + fKStarLong * fRStarLongS));
      } else {
        quantumweight = 1.0 - TMath::Cos(2.0 * (fKStarOut * fRStarOutS + fKStarSide * fRStarSideS + fKStarLong * fRStarLongS));
      }
      fPcount++;
      if (fPcount == 4) fPcount = 0;
    }

    return quantumweight;
  }

  FemtoWeightGeneratorKisiel::dcomplex
  FemtoWeightGeneratorKisiel::GetG(long double eta, long double rho, long double hfun) const {
    dcomplex gtemp;
    long double bres, pres;
    long double bmult;

    Bfunpfun(eta, rho, bres, pres);
    bmult = 2.0 * eta * rho * bres;

    gtemp.re = pres + bmult * (log(fabs(2.0 * eta * rho)) + 2.0 * fEuler - 1.0 + hfun);
    gtemp.im = bmult * Chiim(eta);

    return gtemp;
  }

  void
  FemtoWeightGeneratorKisiel::Getfc(long double kstar, long double eta, long double hfun, dcomplex& fcs, dcomplex& fct) const {
    dcomplex ci;
    dcomplex cia;

    dcomplex fis;
    dcomplex dis;
    dcomplex fcinvs;

    dcomplex fit;
    dcomplex dit;
    dcomplex fcinvt;

    ci.re = hfun;
    ci.im = Chiim(eta);
    cia   = mult(ci, 2.0 / fPionac);

    fis       = invr(fF0s);
    dis       = mult(fD0s, 0.5 * kstar * kstar);
    fcinvs.re = fis.re + dis.re - cia.re;
    fcinvs.im = fis.im + dis.im - cia.im;
    fcs       = invr(fcinvs);

    fit       = invr(fF0t);
    dit       = mult(fD0t, 0.5 * kstar * kstar);
    fcinvt.re = fit.re + dit.re - cia.re;
    fcinvt.im = fit.im + dit.im - cia.im;
    fct       = invr(fcinvt);
  }

  void FemtoWeightGeneratorKisiel::Bfunpfun(long double eta, long double rho, long double& bret, long double& pret) const {
    long double b0   = 1;
    long double b1   = eta * rho;
    long double bsum = b0 + b1;
    long double bnpu, bn, bnmu;
    long double p0   = 1.0;
    long double p1   = 0.0;
    long double psum = p0 + p1;
    long double pnpu, pn, pnmu;

    if (rho > TMath::TwoPi() * 2.0) {
      bret = sin(rho) / rho;
      pret = cos(rho);
      return;
    }


    bn   = b1;
    bnmu = b0;
    pn   = p1;
    pnmu = p0;
    for (int iter = 1; iter < 100000; iter++) {
      bnpu = 2 * eta * rho * bn - rho * rho * bnmu;
      bnpu /= (1.0 * iter + 1.0) * (1.0 * iter + 2.0);
      bsum += bnpu;
      //    std::cout << "B E " << iter << " " << bnpu << std::endl;

      pnpu = 2 * eta * rho * pn - rho * rho * pnmu - (2.0 * iter + 1.0) * 2.0 * eta * rho * bn;
      pnpu /= (1.0 * iter) * (1.0 * iter + 1.0);
      psum += pnpu;
      //    std::cout << "P E " << iter << " " << pnpu << std::endl;

      bnmu = bn;
      bn   = bnpu;

      pnmu = pn;
      pn   = pnpu;
      if ((fabs(pnmu) + fabs(bnmu) + fabs(bnpu) + fabs(pnpu)) < 1.0e-20) {
        //    std::cout << "iter " << iter << std::endl;
        break;
      }
    }


    bret = bsum;
    pret = psum;
  }

  long double FemtoWeightGeneratorKisiel::GetH(long double eta) const {
    long double etasum = log(1.0 / eta) - fEuler;
    long double series = 0.0;
    long double x2inv  = (eta * eta);
    long double element;
    long double save;
    for (int iter = 1; iter < 1000000; iter++) {
      element = ((1.0 * iter) * (1.0 * iter) + x2inv) * (1.0 * iter);
      //    std::cout << "Element " << iter << " is " << element << std::endl;
      element = 1.0 / element;
      if (iter == 1) save = 1.0e-10 * element;
      //    std::cout << "Element " << iter << " is " << element << std::endl;

      series += element;
      if (element < save) break;
    }
    series *= x2inv;
    etasum += series;

    return etasum;
  }

  double FemtoWeightGeneratorKisiel::GetCoulomb() {
    double kstrst = fKStarOut * fRStarOutS + fKStarSide * fRStarSideS + fKStarLong * fRStarLongS;

    // Classical limit - if distance is larger than Coulomb radius,
    // the interaction does not matter
    if (fabs(fRStarS) > fabs(fPionac)) return (1.0);
    if (fabs(fRStarS) == 0.0) return (Gamow(fabs(fKStar)));

    // Classical limit - in the case of large k*r* product we go to
    // classical coulomb interaction
    if (fabs(fKStar) * fRStarS * (1.0 + kstrst / (fRStarS * fabs(fKStar))) > 15.0)
      return (1.0 - 1.0 / (fRStarS * fPionac * fKStar * fKStar));

    // Calculate the F function
    dcomplex ffplus;
    GetFFsingle(ffplus);

    if (!std::isfinite(ffplus.re)) {
      std::cout << "FFPlus Re not a number !"
                << " " << std::endl;
      std::cout << fRStarS << " " << fKStar << " " << fRStarOutS << " " << fRStarSideS << " " << fRStarLongS << std::endl;
    }

    if (!std::isfinite(ffplus.im))
      std::cout << "FFPlus Im not a number !"
                << " " << std::endl;


    return (Gamow(fabs(fKStar)) * modl2(ffplus));
  }

  double FemtoWeightGeneratorKisiel::GetQuantumCoulomb() {
    if (fRStarS < 0.0000000001) return 1.0;

    double kstrst = fKStarOut * fRStarOutS + fKStarSide * fRStarSideS + fKStarLong * fRStarLongS;
    int ccase     = 0;
    int wavesign  = 1;

    if (fTwospin == 1) {
      if (fPcount == 3)
        wavesign = 1;
      else
        wavesign = -1;
      fPcount++;
      if (fPcount == 4) fPcount = 0;
    }

    // Classical limit - if distance is larger than Coulomb radius,
    // the interaction does not matter
    if (fabs(fRStarS) > fabs(fPionac)) return (1.0 + wavesign * cos(2 * kstrst));

    // Classical limit - in the case of large k* we go to
    // classical coulomb interaction
    long double testp = fabs(fKStar) * fRStarS * (1.0 + kstrst / (fRStarS * fabs(fKStar)));
    long double testm = fabs(fKStar) * fRStarS * (1.0 - kstrst / (fRStarS * fabs(fKStar)));

    if ((testp > 15.0) && (testm > 15.0)) {
      double fasymplus  = (1.0 - 1.0 / ((fRStarS + kstrst) * fPionac * fKStar * fKStar));
      double fasymminus = (1.0 - 1.0 / ((fRStarS - kstrst) * fPionac * fKStar * fKStar));
      return 0.5 * ((fasymplus + fasymminus) * cos(2 * kstrst) + (2.0 * sqrt(fasymplus * fasymminus)));
    }
    //    return (1.0 - 1.0/(fRStarS*pionac*fKStar*fKStar))*(1.0+wavesign*cos(2*kstrst));

    dcomplex ffplus, ffminus;
    // Check for the classical limit in both functions separately
    if (((testp < 15.0) && (testm < 15.0)))  // ||
                                             //       ((testp< 15.0) && (testm> 15.0) && (fabs(testp-testm < 1.0))) ||
                                             //       ((testp> 15.0) && (testm< 15.0) && (fabs(testp-testm < 1.0))))
    {
      // Calculate the F function
      GetFFdouble(ffplus, ffminus);
      ccase = 1;
    } else if (testp < 15.0) {
      double asym;
      GetFFsingle(ffplus);
      //      GetFFsingle(&ffminus,-1);
      asym =
        (1.0 - 1.0 / (fRStarS * (1.0 - kstrst / (fRStarS * fabs(fKStar)) * fPionac * fKStar * fKStar))) / Gamow(fabs(fKStar));
      asym = sqrt(asym);
      if (asym < 1.0)
        ffminus.re = 1.0 + (asym - 1.0) * 2.0;
      else
        ffminus.re = 1.0 + (asym - 1.0) / 2.0;
      ffminus.im = sqrt(asym * asym - ffminus.re * ffminus.re);
      ccase      = 2;
    } else {
      double asym;
      //      GetFFsingle(&ffplus);
      GetFFsingle(ffminus, -1);
      asym =
        (1.0 - 1.0 / (fRStarS * (1.0 + kstrst / (fRStarS * fabs(fKStar)) * fPionac * fKStar * fKStar))) / Gamow(fabs(fKStar));
      asym = sqrt(asym);
      if (asym < 1.0)
        ffplus.re = 1.0 + (asym - 1.0) * 2.0;
      else
        ffplus.re = 1.0 + (asym - 1.0) / 2.0;
      ffplus.im = sqrt(asym * asym - ffplus.re * ffplus.re);
      ccase     = 3;
    }

    dcomplex expikr;
    expikr.re         = cos(kstrst);
    expikr.im         = sin(kstrst);
    dcomplex expikrc  = conj(expikr);
    dcomplex ffplusc  = conj(ffplus);
    dcomplex ffminusc = conj(ffminus);

    dcomplex expikr2  = mult(expikr, expikr);
    dcomplex expikrc2 = conj(expikr2);
    dcomplex sterm    = mult(expikr2, mult(ffplus, ffminusc));
    dcomplex tterm    = mult(expikrc2, mult(ffminus, ffplusc));


    if (!std::isfinite(ffminus.re))
      std::cout << "FFMinus Re not a number !"
                << " " << ccase << std::endl;

    if (!std::isfinite(ffminus.im))
      std::cout << "FFMinus Im not a number !"
                << " " << ccase << std::endl;

    if ((ffplus.re > 2.0) || (ffplus.re < -2.0)) std::cout << std::endl << "FFplus Re wild !" << ffplus.re << std::endl;

    if ((ffplus.im > 2.0) || (ffplus.im < -2.0)) std::cout << "FFplus Im wild !" << ffplus.im << std::endl;

    if ((ffminus.re > 2.0) || (ffminus.re < -2.0))
      std::cout << std::endl << "FFminus Re wild !" << ffminus.re << " " << ccase << std::endl;

    if ((ffminus.im > 2.0) || (ffminus.im < -2.0)) std::cout << "FFminus Im wild !" << ffminus.im << " " << ccase << std::endl;

    fCoulqscpart = 0.5 * Gamow(fabs(fKStar)) * (modl2(ffplus) + modl2(ffminus));

    return (0.5 * Gamow(fabs(fKStar)) * (modl2(ffplus) + wavesign * sterm.re + wavesign * tterm.re + modl2(ffminus)));
  }

  void FemtoWeightGeneratorKisiel::GetFFdouble(dcomplex& ffp, dcomplex& ffm) const {
    long double comprep[COULOMBSTEPS];
    long double compimp[COULOMBSTEPS];
    long double comprem[COULOMBSTEPS];
    long double compimm[COULOMBSTEPS];
    long double eta, ksip, ksim;
    dcomplex alfa, zetp, zetm;

    int nsteps;

    long double kstar  = fabs(fKStar);
    long double kstrst = fKStarOut * fRStarOutS + fKStarSide * fRStarSideS + fKStarLong * fRStarLongS;
    long double coskr  = kstrst / (fabs(fKStar) * fRStarS);

    if ((kstar * fRStarS * (1 + coskr) < 5.0) && (kstar * fRStarS * (1 - coskr) < 5.0))
      nsteps = 25;
    else if ((kstar * fRStarS * (1 + coskr) < 10.0) && (kstar * fRStarS * (1 - coskr) < 10.0))
      nsteps = 45;
    else if ((kstar * fRStarS * (1 + coskr) < 15.0) && (kstar * fRStarS * (1 - coskr) < 15.0))
      nsteps = 110;
    else
      nsteps = 150;

    eta     = 1.0 / (kstar * fPionac);
    alfa.re = 0.0;
    alfa.im = -eta;

    dcomplex fcomp, scompp, scompm;
    long double tcomp;
    dcomplex sump, summ;
    dcomplex fcmult;

    long double rad = fRStarS;

    ksip = kstar * rad * (1 + coskr);
    ksim = kstar * rad * (1 - coskr);

    zetp.re = 0.0;
    zetp.im = ksip;

    zetm.re = 0.0;
    zetm.im = ksim;

    fcomp.re  = 1.0;
    fcomp.im  = 0.0;
    scompp.re = 1.0;
    scompp.im = 0.0;
    scompm.re = 1.0;
    scompm.im = 0.0;
    tcomp     = 1.0;

    for (int istep = 0; istep < nsteps; istep++) {
      sump = mult(fcomp, scompp);
      summ = mult(fcomp, scompm);

      sump = mult(sump, 1.0 / (tcomp * tcomp));
      summ = mult(summ, 1.0 / (tcomp * tcomp));


      if (istep == 0) {
        comprep[istep] = sump.re;
        compimp[istep] = sump.im;

        comprem[istep] = summ.re;
        compimm[istep] = summ.im;
      } else {
        comprep[istep] = comprep[istep - 1] + sump.re;
        compimp[istep] = compimp[istep - 1] + sump.im;

        comprem[istep] = comprem[istep - 1] + summ.re;
        compimm[istep] = compimm[istep - 1] + summ.im;
      }

      fcmult.re = alfa.re + istep;
      fcmult.im = alfa.im;

      fcomp  = mult(fcomp, fcmult);
      scompp = mult(scompp, zetp);
      scompm = mult(scompm, zetm);
      tcomp *= (istep + 1);
    }

    ffp.re = comprep[nsteps - 1];
    ffp.im = compimp[nsteps - 1];

    ffm.re = comprem[nsteps - 1];
    ffm.im = compimm[nsteps - 1];
  }

  void FemtoWeightGeneratorKisiel::GetFFsingle(dcomplex& ffp, int sign) const {
    double comprep[COULOMBSTEPS];
    double compimp[COULOMBSTEPS];
    double eta, ksip;
    dcomplex alfa, zetp;

    int nsteps;

    double kstar  = fabs(fKStar);
    double kstrst = fKStarOut * fRStarOutS + fKStarSide * fRStarSideS + fKStarLong * fRStarLongS;
    double coskr  = sign * kstrst / (fabs(fKStar) * fRStarS);

    if (kstar * fRStarS * (1 + coskr) > 15.0)
      nsteps = 170;
    else if (kstar * fRStarS * (1 + coskr) > 10.0)
      nsteps = 45;
    else if (kstar * fRStarS * (1 + coskr) > 5.0)
      nsteps = 35;
    else
      nsteps = 15;

    eta     = 1.0 / (kstar * fPionac);
    alfa.re = 0.0;
    alfa.im = -eta;

    dcomplex fcomp, scompp;
    double tcomp;
    dcomplex sump;
    dcomplex fcmult;

    double rad = fRStarS;

    ksip = kstar * rad * (1 + coskr);

    zetp.re = 0.0;
    zetp.im = ksip;

    fcomp.re  = 1.0;
    fcomp.im  = 0.0;
    scompp.re = 1.0;
    scompp.im = 0.0;
    tcomp     = 1.0;

    for (int istep = 0; istep < nsteps; istep++) {
      sump = mult(fcomp, scompp);

      sump = mult(sump, 1.0 / (tcomp * tcomp));

      if (istep == 0) {
        comprep[istep] = sump.re;
        compimp[istep] = sump.im;
      } else {
        comprep[istep] = comprep[istep - 1] + sump.re;
        compimp[istep] = compimp[istep - 1] + sump.im;
      }

      fcmult.re = alfa.re + istep;
      fcmult.im = alfa.im;

      fcomp  = mult(fcomp, fcmult);
      scompp = mult(scompp, zetp);
      tcomp *= (istep + 1);

      if ((sump.re * sump.re + sump.im * sump.im) < 1.0e-14) {
        nsteps = istep;
        break;
      }
    }

    ffp.re = comprep[nsteps - 1];
    ffp.im = compimp[nsteps - 1];
  }

  FemtoWeightGeneratorKisiel::dcomplex FemtoWeightGeneratorKisiel::conj(const dcomplex& arg) const {
    dcomplex res;

    res.re = arg.re;
    res.im = -arg.im;

    return res;
  }

  FemtoWeightGeneratorKisiel::dcomplex FemtoWeightGeneratorKisiel::mult(const dcomplex& arga, long double argb) const {
    dcomplex res;

    res.re = arga.re * argb;
    res.im = arga.im * argb;

    return res;
  }

  FemtoWeightGeneratorKisiel::dcomplex FemtoWeightGeneratorKisiel::mult(const dcomplex& arga, const dcomplex& argb) const {
    dcomplex res;

    res.re = arga.re * argb.re - arga.im * argb.im;
    res.im = arga.re * argb.im + argb.re * arga.im;

    return res;
  }

  Package* FemtoWeightGeneratorKisiel::Report() const {
    Package* pack = FemtoWeightGenerator::Report();
    pack->AddObject(new ParameterString("PairType", Femto::PairTypeToString(fPairType)));
    return pack;
  }

  Bool_t FemtoWeightGeneratorKisiel::IsPairSupported(Femto::EPairType type) const {
    if (static_cast<int>(type) >= 100) return kTRUE;
    return kFALSE;
  }

  FemtoWeightGeneratorKisiel::~FemtoWeightGeneratorKisiel() {}
}  // namespace Hal
