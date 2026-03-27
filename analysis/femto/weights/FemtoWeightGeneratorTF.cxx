/*
 * FemtoWeightGeneratorTF.cxx
 *
 *  Created on: 26 mar 2026
 *      Author: daniel
 */

#include "FemtoWeightGeneratorTF.h"

#include "FemtoConst.h"
#include "FemtoPair.h"
#include <TF1.h>
#include <TF3.h>

namespace Hal {

  FemtoWeightGeneratorTF1::FemtoWeightGeneratorTF1() {
    for (int i = 0; i < fMaxPairsNo; i++) {
      fWeights.push_back(nullptr);
    }
    fWeights[0] = new TF1("f", "1", -1E+3, 1E+3, 0);
  }

  FemtoWeightGeneratorTF1::~FemtoWeightGeneratorTF1() {
    for (auto x : fWeights)
      if (x) delete x;
  }

  Double_t FemtoWeightGeneratorTF1::GenerateWeight(Hal::FemtoPair* pair) {
    auto type     = GetPairTypeFromPair(pair);
    auto weightNo = Hal::Femto::PairTypeToNextInt(type);
    if (weightNo >= fMaxPairsNo) { return 1; }
    TF1* wcalc = fWeights[weightNo];
    if (wcalc == nullptr) return 1;
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

    // Boost to pair cms
    fKStarOut = tMt / tM * (fKStarOut - tPt / tMt * tE1L);

    fKStar = ::sqrt(fKStarOut * fKStarOut + fKStarSide * fKStarSide + fKStarLong * fKStarLong);
    return wcalc->Eval(fKStar);
  }

  void FemtoWeightGeneratorTF1::SetWeightFunc(const TF1& func, Femto::EPairType type) {
    int pos  = Hal::Femto::PairTypeToNextInt(type);
    auto old = fWeights[pos];
    if (old) delete old;
    fWeights[pos] = (TF1*) func.Clone();
  }

  FemtoWeightGeneratorTF1::FemtoWeightGeneratorTF1(const FemtoWeightGeneratorTF1& other) {
    for (int i = 0; i < fMaxPairsNo; i++) {
      fWeights.push_back((TF1*) other.fWeights[i]->Clone());
    }
  }

  Hal::FemtoWeightGenerator* FemtoWeightGeneratorTF1::MakeCopy() const { return new FemtoWeightGeneratorTF1(*this); }
  //==================================================================================================================
  FemtoWeightGeneratorTF3::FemtoWeightGeneratorTF3() {
    for (int i = 0; i < fMaxPairsNo; i++) {
      fWeights.push_back(nullptr);
    }
    fWeights[0] = new TF3("f", "1", -1E+3, 1E+3, 0);
  }

  FemtoWeightGeneratorTF3::~FemtoWeightGeneratorTF3() {
    for (auto x : fWeights)
      if (x) delete x;
  }

  Double_t FemtoWeightGeneratorTF3::GenerateWeight(Hal::FemtoPair* pair) {
    auto type     = GetPairTypeFromPair(pair);
    auto weightNo = Hal::Femto::PairTypeToNextInt(type);
    if (weightNo >= fMaxPairsNo) { return 1; }
    TF1* wcalc = fWeights[weightNo];
    if (wcalc == nullptr) return 1;
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

    // Boost to pair cms
    fKStarOut = tMt / tM * (fKStarOut - tPt / tMt * tE1L);

    fKStar = ::sqrt(fKStarOut * fKStarOut + fKStarSide * fKStarSide + fKStarLong * fKStarLong);
    return wcalc->Eval(fKStarOut, fKStarSide, fKStarLong);
  }

  void FemtoWeightGeneratorTF3::SetWeightFunc(const TF3& func, Femto::EPairType type) {
    int pos  = Hal::Femto::PairTypeToNextInt(type);
    auto old = fWeights[pos];
    if (old) delete old;
    fWeights[pos] = (TF3*) func.Clone();
  }

  FemtoWeightGeneratorTF3::FemtoWeightGeneratorTF3(const FemtoWeightGeneratorTF3& other) {
    for (int i = 0; i < fMaxPairsNo; i++) {
      fWeights.push_back((TF3*) other.fWeights[i]->Clone());
    }
  }

  Hal::FemtoWeightGenerator* FemtoWeightGeneratorTF3::MakeCopy() const { return new FemtoWeightGeneratorTF3(*this); }

} /* namespace Hal */
