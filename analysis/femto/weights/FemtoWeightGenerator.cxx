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

  void FemtoWeightGenerator::Print(Option_t* option) const {
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
}  // namespace Hal
