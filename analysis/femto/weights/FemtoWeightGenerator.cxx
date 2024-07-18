/*
 * FemtoWeightGenerator.cxx
 *
 *  Created on: 21-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoWeightGenerator.h"

#include "Cout.h"
#include "FemtoPair.h"

namespace Hal {
  FemtoWeightGenerator::FemtoWeightGenerator() :
    fPairType(Femto::EPairType::kUnknown),
    fTaskID(0),
    fKStarOut(0),
    fKStarSide(0),
    fKStarLong(0),
    fKStar(0),
    fRStarOut(0),
    fRStarSide(0),
    fRStarLong(0),
    fRStar(0) {}

  FemtoWeightGenerator::FemtoWeightGenerator(const FemtoWeightGenerator& aModel) :
    TObject(aModel),
    fPairType(aModel.fPairType),
    fTaskID(aModel.fTaskID),
    fKStarOut(0),
    fKStarSide(0),
    fKStarLong(0),
    fKStar(0),
    fRStarOut(0),
    fRStarSide(0),
    fRStarLong(0),
    fRStar(0) {}

  FemtoWeightGenerator& FemtoWeightGenerator::operator=(const FemtoWeightGenerator& aModel) {
    if (this != &aModel) {
      fTaskID    = aModel.fTaskID;
      fKStarOut  = aModel.fKStarOut;
      fKStarSide = aModel.fKStarSide;
      fKStarLong = aModel.fKStarLong;
      fKStar     = aModel.fKStar;
      fRStarOut  = aModel.fRStarOut;
      fRStarSide = aModel.fRStarSide;
      fRStarLong = aModel.fRStarLong;
      fRStar     = aModel.fRStar;
      fPairType  = aModel.fPairType;
    }
    return *this;
  }

  void FemtoWeightGenerator::SetPairType(Femto::EPairType aPairType) { fPairType = aPairType; }

  Femto::EPairType FemtoWeightGenerator::GetPairType() const { return fPairType; }

  FemtoWeightGenerator::~FemtoWeightGenerator() {
    // TODO Auto-generated destructor stub
  }

  Double_t FemtoWeightGenerator::GenerateWeight(FemtoPair* /*pair*/) { return 1; }

  void FemtoWeightGenerator::SetPairTypeFromPair(FemtoPair* pair) { fPairType = GetPairTypeFromPair(pair); }

  void FemtoWeightGenerator::Print(Option_t* /*option*/) const {
    Cout::Text(ClassName(), "L");
    Cout::Text(Femto::PairTypeToString(fPairType), "L");
  }

  Femto::EPairType FemtoWeightGenerator::GetPairTypeFromPair(FemtoPair* pair) {
    const Int_t ktPid1 = pair->GetPdg1();
    const Int_t ktPid2 = pair->GetPdg2();
    return Femto::PidToPairType(ktPid1, ktPid2);
  }

  Int_t FemtoWeightGenerator::GetPdg1() { return Femto::PairTypeToPid(fPairType).first; }

  Int_t FemtoWeightGenerator::GetPdg2() { return Femto::PairTypeToPid(fPairType).second; }

  void FemtoWeightGenerator::CalculateKinematics(FemtoPair* pair) {
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

    // Rotate in transverse plane
    fKStarOut  = (pair->TruePx1() * tPx + pair->TruePy1() * tPy) / tPt;
    fKStarSide = (-pair->TruePx1() * tPy + pair->TruePy1() * tPx) / tPt;


    // Boost to pair cms
    fKStarOut = tMt / tM * (fKStarOut - tPt / tMt * tE1L);


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

    Double_t tROS = fRStarOut / 0.197327;
    //   Double_t tDTimePairCMS = tGamma*(tDTimePairLCMS - tBeta* tROut);
    fRStar = ::sqrt(fRStarOut * fRStarOut + fRStarSide * fRStarSide + fRStarLong * fRStarLong);
    fKStar = ::sqrt(fKStarOut * fKStarOut + fKStarSide * fKStarSide + fKStarLong * fKStarLong);
    if (fKStarOut < 0) fKStar = -fKStar;
  }
}  // namespace Hal
