/*
 * FemtoWeightGeneratorBasic.cxx
 *
 *  Created on: 21-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoWeightGeneratorBasic.h"


#include <RtypesCore.h>
#include <TString.h>
#include <cmath>

#include "FemtoConst.h"
#include "FemtoPair.h"
#include "Package.h"
#include "Parameter.h"

namespace Hal {
  FemtoWeightGeneratorBasic::FemtoWeightGeneratorBasic() {}

  FemtoWeightGenerator* FemtoWeightGeneratorBasic::GetGenerator() const {
    FemtoWeightGeneratorBasic* tGen = new FemtoWeightGeneratorBasic(*this);
    return tGen;
  }

  FemtoWeightGeneratorBasic::FemtoWeightGeneratorBasic(const FemtoWeightGeneratorBasic& aModel) : FemtoWeightGenerator(aModel) {}

  FemtoWeightGeneratorBasic& FemtoWeightGeneratorBasic::operator=(const FemtoWeightGeneratorBasic& aModel) {
    if (this != &aModel) { FemtoWeightGenerator::operator=(aModel); }

    return *this;
  }

  Double_t FemtoWeightGeneratorBasic::GenerateWeight(FemtoPair* pair) {
    // Generate a simple femtoscopic weight coming only from
    // quantum statistics - symmetrization or anti-symmetrization
    // of the pair wave function

    // Get hidden information pointers
    // return 1;
    // Calculate pair variables
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

    // Transform positions to LCMS
    //   Double_t tP1zl = tGamma * (track1->GetEmissionPoint()->z() - tBeta *
    //   track1->GetEmissionPoint()->t()); Double_t tP1tl = tGamma *
    //   (track1->GetEmissionPoint()->t() - tBeta *
    //   track1->GetEmissionPoint()->z());

    //   Double_t tP2zl = tGamma * (track2->GetEmissionPoint()->z() - tBeta *
    //   track2->GetEmissionPoint()->t()); Double_t tP2tl = tGamma *
    //   (track2->GetEmissionPoint()->t() - tBeta *
    //   track2->GetEmissionPoint()->z());

    //   Double_t tP1pzl = tGamma * (track1->GetTrueMomentum()->z() - tBeta *
    //   tE1); Double_t tP1el  = tGamma * (tE1  - tBeta *
    //   track1->GetTrueMomentum()->z());

    //   Double_t tP2pzl = tGamma * (track2->GetTrueMomentum()->z() - tBeta *
    //   tE2); Double_t tP2el  = tGamma * (tE2  - tBeta *
    //   track2->GetTrueMomentum()->z());

    // Rotate in transverse plane
    fKStarOut  = (pair->TruePx1() * tPx + pair->TruePy1() * tPy) / tPt;
    fKStarSide = (-pair->TruePx1() * tPy + pair->TruePy1() * tPx) / tPt;

    //   Double_t tP1pxl = fKStarOut;
    //   Double_t tP1pyl = fKStarSide;

    //   Double_t tP2pxl = (track2->GetTrueMomentum()->x()*tPx +
    //   track2->GetTrueMomentum()->y()*tPy)/tPt; Double_t tP2pyl =
    //   (track2->GetTrueMomentum()->y()*tPx -
    //   track2->GetTrueMomentum()->x()*tPy)/tPt;;

    //   Double_t tKO = tP1pxl - tP2pxl;
    //   Double_t tKS = tP1pyl - tP2pyl;
    //   Double_t tKL = tP1pzl - tP2pzl;
    //   Double_t tDE = tP1el  - tP2el;

    // save the rotated coordinates in LCMS variables
    //   Double_t tP1xl = ( track1->GetEmissionPoint()->x()*tPx +
    //   track1->GetEmissionPoint()->y()*tPy)/tPt; Double_t tP1yl =
    //   (-track1->GetEmissionPoint()->x()*tPy +
    //   track1->GetEmissionPoint()->y()*tPx)/tPt;

    //   Double_t tP2xl = ( track2->GetEmissionPoint()->x()*tPx +
    //   track2->GetEmissionPoint()->y()*tPy)/tPt; Double_t tP2yl =
    //   (-track2->GetEmissionPoint()->x()*tPy +
    //   track2->GetEmissionPoint()->y()*tPx)/tPt;

    // Boost to pair cms
    fKStarOut = tMt / tM * (fKStarOut - tPt / tMt * tE1L);

    // tBetat = tPt/tMt;
    //   Double_t tGammat = 1.0/sqrt(1.0-tBetat*tBetat);

    //   Double_t tP1xp = tGammat*(tP1xl - tBetat*tP1tl);
    //   Double_t tP1tp = tGammat*(tP1tl - tBetat*tP1xl);

    //   Double_t tP2xp = tGammat*(tP2xl - tBetat*tP2tl);
    //   Double_t tP2tp = tGammat*(tP2tl - tBetat*tP2xl);

    //   Double_t tRO = (tP1xl - tP2xl)/0.197327;
    //   Double_t tRS = (tP1yl - tP2yl)/0.197327;
    //   Double_t tRL = (tP1zl - tP2zl)/0.197327;
    //   Double_t tDT = (tP1tl - tP2tl)/0.197327;

    Double_t tDX    = pair->GetX1() - pair->GetX2();
    Double_t tDY    = pair->GetY1() - pair->GetY2();
    Double_t tRLong = pair->GetZ1() - pair->GetZ2();
    Double_t tDTime = pair->GetT1() - pair->GetT2();

    Double_t tROut  = (tDX * tPx + tDY * tPy) / tPt;
    Double_t tRSide = (-tDX * tPy + tDY * tPx) / tPt;

    fRStarSide    = tRSide;
    Double_t tRSS = fRStarSide / 0.197327;

    fRStarLong              = tGamma * (tRLong - tBeta * tDTime);
    Double_t tDTimePairLCMS = tGamma * (tDTime - tBeta * tRLong);

    Double_t tRLS = fRStarLong / 0.197327;
    // 1/.1973
    tBeta  = tPt / tMt;
    tGamma = tMt / tM;
    // std::cout<<tROut<<" x "<<fRStarSide<<" "<<fRStarLong<<std::endl;
    fRStarOut = tGamma * (tROut - tBeta * tDTimePairLCMS);
    // TLorentzVector TX;
    // TX.SetXYZT(-pair->X2(),-pair->Y2(),-pair->Z2(),-pair->T2());
    // TX.Boost(0,0,-tPz/tE);
    // TX.RotateZ(-TMath::ATan2(tPy,tPx));
    // std::cout<<" contorl of weight "<<TX.X()<<" "<<TX.Y()<<"
    // "<<TX.Z()<<std::endl;
    Double_t tROS = fRStarOut / 0.197327;
    //   Double_t tDTimePairCMS = tGamma*(tDTimePairLCMS - tBeta* tROut);
    fRStar = ::sqrt(fRStarOut * fRStarOut + fRStarSide * fRStarSide + fRStarLong * fRStarLong);
    fKStar = ::sqrt(fKStarOut * fKStarOut + fKStarSide * fKStarSide + fKStarLong * fKStarLong);

    //   Double_t tRSt = fRStar/0.197327;
#ifdef _kinetic_debug

    Double_t tpx = pair->GetPx1() - pair->GetPx2();
    Double_t tpy = pair->GetPy1() - pair->y2();
    Double_t tpz = pair->GetPz1() - pair->GetPz2();
    Double_t tpe = pair->GetE1() - pair->GetE2();
    TLorentzVector p1, p2;
    p1.SetPxPyPzE(pair->GetPx1(), pair->GetPy1(), pair->GetPz1(), pair->GetE1());
    p2.SetPxPyPzE(pair->GetPx2(), pair->GetPy2(), pair->GetPz2(), pair->GetE2());
    TLorentzVector x1, x2;
    x1.SetPxPyPzE(pair->GetX1(), pair->GetY1(), pair->GetZ1(), pair->GetT1());
    x2.SetPxPyPzE(pair->GetX2(), pair->GetY2(), pair->GetZ2(), pair->GetT2());
    TLorentzVector sum = p1 + p2;
    Double_t vz        = sum.Pz() / sum.E();
    p1.Boost(0, 0, -vz);
    p2.Boost(0, 0, -vz);
    x1.Boost(0, 0, -vz);
    x2.Boost(0, 0, -vz);
    Double_t phi = sum.Phi();
    p1.RotateZ(-phi);
    p2.RotateZ(-phi);
    x1.RotateZ(-phi);
    x2.RotateZ(-phi);
    sum.Boost(0, 0, -vz);
    Double_t vx = sum.Pt() / sum.E();
    p1.Boost(-vx, 0, 0);
    p2.Boost(-vx, 0, 0);
    x1.Boost(-vx, 0, 0);
    x2.Boost(-vx, 0, 0);
    TLorentzVector R = x1 - x2;
    std::cout << " ---WEIGHT--PRF-------- " << std::endl;

    std::cout << " out " << 2 * fKStarOut << " " << p1.Px() << std::endl;
    std::cout << " side " << 2 * fKStarSide << " " << p1.Py() << std::endl;
    std::cout << " long " << 2 * fKStarLong << " " << p1.Pz() << std::endl;
    std::cout << " star " << 2 * fKStar << " " << (p1.Rho()) << std::endl;

    std::cout << " Rlong " << fRStarLong << " " << R.Pz() << std::endl;
    std::cout << " Rout " << fRStarOut << " " << R.Px() << std::endl;
    std::cout << " Rside " << fRStarSide << " " << R.Py() << std::endl;

    pair->PrintInfo();
    std::cout << " ---END WEIGHT-------- " << std::endl;
    // std::cout<<" Rstar "<<fKStar <<" " <<(p1.Rho())<<std::endl;
    std::cout << " WAGA BASIC " << 2.0 * (fKStarOut * tROS) << " " << 2.0 * (fKStarSide * tRSS) << " "
              << 2.0 * (fKStarLong * tRLS) << std::endl;
    std::cout << "WAGA BASIC " << 2.0 * (fKStarOut * tROS + fKStarSide * tRSS + fKStarLong * tRLS) << std::endl;
#endif
    ///=====================================================
    if (fPairType != Femto::EPairType::kUnknown) {
      if ((fPairType == Femto::EPairType::kPionPlusPionPlus) || (fPairType == Femto::EPairType::kKaonPlusKaonPlus)) {
        return 1.0 + cos(2.0 * (fKStarOut * tROS + fKStarSide * tRSS + fKStarLong * tRLS));
      } else if (fPairType == Femto::EPairType::kProtonProton || fPairType == Femto::EPairType::kLambdaLambda)
        return 1.0 - 0.5 * cos(2.0 * (fKStarOut * tROS + fKStarSide * tRSS + fKStarLong * tRLS));
      else
        return 1.0;
    } else {
      Femto::EPairType tPairType = GetPairTypeFromPair(pair);
      if ((tPairType == Femto::EPairType::kPionPlusPionPlus) || (tPairType == Femto::EPairType::kKaonPlusKaonPlus)) {
        return 1.0 + cos(2.0 * (fKStarOut * tROS + fKStarSide * tRSS + fKStarLong * tRLS));
      } else if (tPairType == Femto::EPairType::kProtonProton || tPairType == Femto::EPairType::kLambdaLambda)
        return 1.0 - 0.5 * cos(2.0 * (fKStarOut * tROS + fKStarSide * tRSS + fKStarLong * tRLS));
      else
        return 1.0;
    }
  }

  FemtoWeightGenerator* FemtoWeightGeneratorBasic::Clone(const char* /*newname*/) const { return GetGenerator(); }

  FemtoWeightGeneratorBasic::~FemtoWeightGeneratorBasic() {}

  Package* FemtoWeightGeneratorBasic::Report() const {
    Package* report = FemtoWeightGenerator::Report();
    report->AddObject(new ParameterString("PairType", Femto::PairTypeToString(fPairType)));
    return report;
  }
}  // namespace Hal
