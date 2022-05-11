/*
 * FemtoWeightGeneratorLednicky2.cxx
 *
 *  Created on: 30 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */


#include "FemtoWeightGeneratorLednicky2.h"

#include "FemtoFsiParsed.h"
#include "FemtoPair.h"

#include <RtypesCore.h>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <utility>

namespace Hal {
  FemtoWeightGeneratorLednicky2::FemtoWeightGeneratorLednicky2() :
    FemtoWeightGenerator(),
    fWei(0),
    fWein(0),
    fWeif(0),
    fWeightDen(0),
    fItest(0),
    fIch(1),
    fIqs(1),
    fIsi(1),
    fI3c(0),
    fNuclMass(1.),
    fNuclCharge(0.),
    fSphereApp(false),
    fT0App(false),
    fLL(0),
    fNuclChargeSign(1),
    fSwap(0),
    fLLMax(30),
    fLLName(0),
    fNumProcessPair(0),
    fNumbNonId(0) {
    fParser         = new FemtoFsiParsed();
    fLLName         = new char*[fLLMax + 1];
    fNumProcessPair = new int[fLLMax + 1];
    int i;
    for (i = 1; i <= fLLMax; i++) {
      fLLName[i]         = new char[40];
      fNumProcessPair[i] = 0;
    }
    strncpy(fLLName[1], "neutron neutron", 40);
    strncpy(fLLName[2], "proton proton", 40);
    strncpy(fLLName[3], "neutron proton", 40);
    strncpy(fLLName[4], "alpha alpha", 40);
    strncpy(fLLName[5], "pi+ pi-", 40);
    strncpy(fLLName[6], "pi0 pi0", 40);
    strncpy(fLLName[7], "pi+ pi+", 40);
    strncpy(fLLName[8], "neutron deuteron", 40);
    strncpy(fLLName[9], "proton deuteron", 40);
    strncpy(fLLName[10], "pi+ K-", 40);
    strncpy(fLLName[11], "pi+ K+", 40);
    strncpy(fLLName[12], "pi+ proton", 40);
    strncpy(fLLName[13], "pi- proton", 40);
    strncpy(fLLName[14], "K+ K-", 40);
    strncpy(fLLName[15], "K+ K+", 40);
    strncpy(fLLName[16], "K+ proton", 40);
    strncpy(fLLName[17], "K- proton", 40);
    strncpy(fLLName[18], "deuteron deuteron", 40);
    strncpy(fLLName[19], "deuton alpha", 40);
    strncpy(fLLName[20], "triton triton", 40);
    strncpy(fLLName[21], "triton alpha", 40);
    strncpy(fLLName[22], "K0 K0", 40);
    strncpy(fLLName[23], "K0 K0b", 40);
    strncpy(fLLName[24], "deuteron triton", 40);
    strncpy(fLLName[25], "proton triton", 40);
    strncpy(fLLName[26], "proton alpha", 40);
    strncpy(fLLName[27], "proton lambda", 40);
    strncpy(fLLName[28], "neutron lambda", 40);
    strncpy(fLLName[29], "Lambda lambda", 40);       // gael 21May02
    strncpy(fLLName[30], "Proton Anti-proton", 40);  // gael 21May02
    FsiInit();
    FsiNucl();
  }

  void FemtoWeightGeneratorLednicky2::FsiSetLL() {
    int tNS;
    if (fSphereApp || (fLL > 5)) {
      if (fT0App) {
        tNS = 4;
      } else {
        tNS = 2;
      }
    } else {
      tNS = 1;
    }
    // cout <<"fLL dans FsiSetLL() = "<< fLL << endl;
    // cout <<"tNS dans FsiSetLL() = "<< tNS << endl;
    // cout <<"fItest dans FsiSetLL() = "<< fItest << endl;
    fParser->Llini(fLL, tNS, fItest);
  }

  void FemtoWeightGeneratorLednicky2::FsiNucl() {
    // initialize weight generation taking into account the residual charge
    //   cout << "*******************AliFemtoModelWeightGeneratorLednicky check
    //   FsiNucl ************" << endl; cout <<"fNuclMass dans FsiNucl() = " <<
    //   fNuclMass << endl; cout <<"fNuclCharge dans FsiNucl() = " << fNuclCharge
    //   << endl; cout <<"fNuclChargeSign dans FsiNucl() = " << fNuclChargeSign <<
    //   endl;
    fParser->Fsinucl(fNuclMass, fNuclCharge * fNuclChargeSign);
  }

  bool FemtoWeightGeneratorLednicky2::SetPid(const int aPid1, const int aPid2) {
    // set calculated system for basing on particles' pids
    static const int ksPi0Pid  = 111;
    static const int ksPionPid = 211;
    static const int ksK0Pid   = 310;
    static const int ksKPid    = 321;
    static const int ksNeutPid = 2112;
    static const int ksProtPid = 2212;
    static const int ksLamPid  = 3122;
    //  static const int sLamLamPid=3122;

    // cout << "Setting PID to " << aPid1 << " " << aPid2 << endl;

    int tPidl, tPidh;
    int tChargeFactor = 1;

    if (abs(aPid1) < abs(aPid2)) {
      if (aPid1 < 0) tChargeFactor = -1;
      tPidl = aPid1 * tChargeFactor;
      tPidh = aPid2 * tChargeFactor;
      fSwap = false;
    } else {
      if (aPid2 < 0) tChargeFactor = -1;
      tPidl = aPid2 * tChargeFactor;
      tPidh = aPid1 * tChargeFactor;
      fSwap = true;
    }
    switch (tPidl) {
      case ksPionPid:
        switch (tPidh) {
          case -ksPionPid:
            fLL = 5;
            tChargeFactor *= 1;
            break;
          case ksPionPid:
            fLL = 7;
            tChargeFactor *= 1;
            break;
          case -ksKPid:
            fLL = 10;
            tChargeFactor *= 1;
            break;
          case ksKPid:
            fLL = 11;
            tChargeFactor *= 1;
            break;
          case ksProtPid:
            fLL = 12;
            tChargeFactor *= 1;
            break;
          case -ksProtPid:
            fLL = 13;
            tChargeFactor *= -1;
            break;
          default: fLL = 0; break;
        }
        break;
      case ksProtPid:
        switch (tPidh) {
          case ksProtPid:
            fLL = 2;
            tChargeFactor *= 1;
            break;
          case ksLamPid:
            fLL = 27;
            tChargeFactor *= 1;
            break;
          case -ksProtPid:
            fLL = 30;
            tChargeFactor *= 1;
            break;
          default: fLL = 0; break;
        }
        break;
      case ksKPid:
        switch (tPidh) {
          case -ksKPid:
            fLL = 14;
            tChargeFactor *= 1;
            break;
          case ksKPid:
            fLL = 15;
            tChargeFactor *= 1;
            break;
          case ksProtPid:
            fLL = 16;
            tChargeFactor *= 1;
            break;
          case -ksProtPid:
            fLL = 17;
            tChargeFactor *= -1;
            break;
          default: fLL = 0; break;
        }
        break;
      case ksK0Pid:
        switch (tPidh) {
          case ksK0Pid:
            fLL = 22;
            tChargeFactor *= 1;
            break;
          case -ksK0Pid:
            fLL = 23;
            tChargeFactor *= 1;
            break;
          default: fLL = 0; break;
        }
        break;
      case ksPi0Pid:
        switch (tPidh) {
          case ksPi0Pid:
            fLL = 6;
            tChargeFactor *= 1;
            break;
          default: fLL = 0; break;
        }
        break;
      case ksNeutPid:
        switch (tPidh) {
          case ksNeutPid:
            fLL = 1;
            tChargeFactor *= 1;
            break;
          case ksProtPid:
            fLL = 3;
            tChargeFactor *= 1;
            break;
          case ksLamPid:
            fLL = 28;
            tChargeFactor *= 1;
            break;
          default: fLL = 0; break;
        }
        break;            // Gael 21May02
      case ksLamPid:      // Gael 21May02
        switch (tPidh) {  // Gael 21May02
          case ksLamPid:
            fLL = 29;
            tChargeFactor *= 1;
            break;                  // Gael 21May02
          default: fLL = 0; break;  // Gael 21May02
        }                           // Gael 21May02
        break;                      // Gael 21May02
      default: fLL = 0; break;
    }
    if (tChargeFactor != fNuclChargeSign) {
      fNuclChargeSign = tChargeFactor;
      FsiNucl();
    }
    (fNumProcessPair[0])++;
    if (fLL) {
      (fNumProcessPair[fLL])++;
      return true;
    } else {
      fNumbNonId++;
      return false;
    }
    //   cout << "*******************AliFemtoModelWeightGeneratorLednicky check
    //   SetPid ************" << endl; cout << "fLL=="<< fLL << endl; cout <<
    //   "fNuclCharge=="<< fNuclCharge << endl;
  }

  FemtoWeightGeneratorLednicky2::FemtoWeightGeneratorLednicky2(const FemtoWeightGeneratorLednicky2& aWeight) :
    FemtoWeightGenerator(),
    fWei(0),
    fWein(0),
    fWeif(0),
    fWeightDen(0),
    fItest(0),
    fIch(1),
    fIqs(1),
    fIsi(1),
    fI3c(0),
    fNuclMass(1.),
    fNuclCharge(0.),
    fSphereApp(false),
    fT0App(false),
    fLL(0),
    fNuclChargeSign(1),
    fSwap(0),
    fLLMax(30),
    fLLName(0),
    fNumProcessPair(0),
    fNumbNonId(0) {
    // copy constructor
    fWei       = aWeight.fWei;
    fWein      = aWeight.fWein;
    fWeif      = aWeight.fWeif;
    fWeightDen = aWeight.fWeightDen;

    fItest          = aWeight.fItest;
    fIch            = aWeight.fIch;
    fIqs            = aWeight.fIqs;
    fIsi            = aWeight.fIsi;
    fI3c            = aWeight.fI3c;
    fNuclMass       = aWeight.fNuclMass;
    fNuclCharge     = aWeight.fNuclCharge;
    fSphereApp      = aWeight.fSphereApp;
    fT0App          = aWeight.fT0App;
    fLL             = aWeight.fLL;
    fNuclChargeSign = aWeight.fNuclChargeSign;
    fSwap           = aWeight.fSwap;
    fLLName         = aWeight.fLLName;
    fNumProcessPair = aWeight.fNumProcessPair;
    fNumbNonId      = aWeight.fNumbNonId;
    fLLName         = new char*[fLLMax + 1];
    fNumProcessPair = new int[fLLMax + 1];
    int i;
    for (i = 1; i <= fLLMax; i++) {
      fLLName[i]         = new char[40];
      fNumProcessPair[i] = 0;
    }
    strncpy(fLLName[1], "neutron neutron", 40);
    strncpy(fLLName[2], "proton proton", 40);
    strncpy(fLLName[3], "neutron proton", 40);
    strncpy(fLLName[4], "alpha alpha", 40);
    strncpy(fLLName[5], "pi+ pi-", 40);
    strncpy(fLLName[6], "pi0 pi0", 40);
    strncpy(fLLName[7], "pi+ pi+", 40);
    strncpy(fLLName[8], "neutron deuteron", 40);
    strncpy(fLLName[9], "proton deuteron", 40);
    strncpy(fLLName[10], "pi+ K-", 40);
    strncpy(fLLName[11], "pi+ K+", 40);
    strncpy(fLLName[12], "pi+ proton", 40);
    strncpy(fLLName[13], "pi- proton", 40);
    strncpy(fLLName[14], "K+ K-", 40);
    strncpy(fLLName[15], "K+ K+", 40);
    strncpy(fLLName[16], "K+ proton", 40);
    strncpy(fLLName[17], "K- proton", 40);
    strncpy(fLLName[18], "deuteron deuteron", 40);
    strncpy(fLLName[19], "deuton alpha", 40);
    strncpy(fLLName[20], "triton triton", 40);
    strncpy(fLLName[21], "triton alpha", 40);
    strncpy(fLLName[22], "K0 K0", 40);
    strncpy(fLLName[23], "K0 K0b", 40);
    strncpy(fLLName[24], "deuteron triton", 40);
    strncpy(fLLName[25], "proton triton", 40);
    strncpy(fLLName[26], "proton alpha", 40);
    strncpy(fLLName[27], "proton lambda", 40);
    strncpy(fLLName[28], "neutron lambda", 40);
    strncpy(fLLName[29], "Lambda lambda", 40);       // gael 21May02
    strncpy(fLLName[30], "Proton Anti-proton", 40);  // gael 21May02
    FsiInit();
    FsiNucl();
  }

  FemtoWeightGeneratorLednicky2& FemtoWeightGeneratorLednicky2::operator=(const FemtoWeightGeneratorLednicky2& aWeight) {
    if (this == &aWeight) return *this;

    fWei       = aWeight.fWei;
    fWein      = aWeight.fWein;
    fWeif      = aWeight.fWeif;
    fWeightDen = aWeight.fWeightDen;

    fItest          = aWeight.fItest;
    fIch            = aWeight.fIch;
    fIqs            = aWeight.fIqs;
    fIsi            = aWeight.fIsi;
    fI3c            = aWeight.fI3c;
    fNuclMass       = aWeight.fNuclMass;
    fNuclCharge     = aWeight.fNuclCharge;
    fSphereApp      = aWeight.fSphereApp;
    fT0App          = aWeight.fT0App;
    fLL             = aWeight.fLL;
    fNuclChargeSign = aWeight.fNuclChargeSign;
    fSwap           = aWeight.fSwap;
    //  fLLName = aWeight.fLLName;
    fNumProcessPair = aWeight.fNumProcessPair;
    fNumbNonId      = aWeight.fNumbNonId;
    if (fLLName) free(fLLName);
    fLLName = new char*[fLLMax + 1];
    if (fNumProcessPair) free(fNumProcessPair);
    fNumProcessPair = new int[fLLMax + 1];
    int i;
    for (i = 1; i <= fLLMax; i++) {
      fLLName[i]         = new char[40];
      fNumProcessPair[i] = 0;
    }
    strncpy(fLLName[1], "neutron neutron", 40);
    strncpy(fLLName[2], "proton proton", 40);
    strncpy(fLLName[3], "neutron proton", 40);
    strncpy(fLLName[4], "alpha alpha", 40);
    strncpy(fLLName[5], "pi+ pi-", 40);
    strncpy(fLLName[6], "pi0 pi0", 40);
    strncpy(fLLName[7], "pi+ pi+", 40);
    strncpy(fLLName[8], "neutron deuteron", 40);
    strncpy(fLLName[9], "proton deuteron", 40);
    strncpy(fLLName[10], "pi+ K-", 40);
    strncpy(fLLName[11], "pi+ K+", 40);
    strncpy(fLLName[12], "pi+ proton", 40);
    strncpy(fLLName[13], "pi- proton", 40);
    strncpy(fLLName[14], "K+ K-", 40);
    strncpy(fLLName[15], "K+ K+", 40);
    strncpy(fLLName[16], "K+ proton", 40);
    strncpy(fLLName[17], "K- proton", 40);
    strncpy(fLLName[18], "deuteron deuteron", 40);
    strncpy(fLLName[19], "deuton alpha", 40);
    strncpy(fLLName[20], "triton triton", 40);
    strncpy(fLLName[21], "triton alpha", 40);
    strncpy(fLLName[22], "K0 K0", 40);
    strncpy(fLLName[23], "K0 K0b", 40);
    strncpy(fLLName[24], "deuteron triton", 40);
    strncpy(fLLName[25], "proton triton", 40);
    strncpy(fLLName[26], "proton alpha", 40);
    strncpy(fLLName[27], "proton lambda", 40);
    strncpy(fLLName[28], "neutron lambda", 40);
    strncpy(fLLName[29], "Lambda lambda", 40);       // gael 21May02
    strncpy(fLLName[30], "Proton Anti-proton", 40);  // gael 21May02
    FsiInit();
    FsiNucl();

    return *this;
  }

  FemtoWeightGeneratorLednicky2::~FemtoWeightGeneratorLednicky2() {
    if (fLLName) delete[] fLLName;
    if (fNumProcessPair) delete[] fNumProcessPair;
    delete fParser;
  }

  Double_t FemtoWeightGeneratorLednicky2::GenerateWeight(FemtoPair* pair) {
    // AliFemtoModelHiddenInfo *track1 = (AliFemtoModelHiddenInfo *)
    // aPair->Track1()->HiddenInfo();
    // AliFemtoModelHiddenInfo *track2 = (AliFemtoModelHiddenInfo *)
    // aPair->Track2()->HiddenInfo();

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

    // Rotate in transverse plane
    fKStarOut  = (pair->TruePx1() * tPx + pair->TruePy1() * tPy) / tPt;
    fKStarSide = (-pair->TruePx1() * tPy + pair->TruePy1() * tPx) / tPt;
#ifdef _kinetic_debug
    pair->PrintInfo();
    std::cout << "LEDNICKY" << std::endl;
    std::cout << 2.0 * fKStarSide << " " << 2.0 * fKStarOut << " " << fKStarLong * 2.0 << std::endl;
#endif

    // Boost to pair cms
    // Double_t out = this->fKStarOut;
    fKStarOut = tMt / tM * (this->fKStarOut - tPt / tMt * tE1L);

    // tBetat = tPt/tMt;

    Double_t tDX    = pair->GetX1() - pair->GetX2();
    Double_t tDY    = pair->GetY1() - pair->GetY2();
    Double_t tRLong = pair->GetZ1() - pair->GetZ2();
    Double_t tDTime = pair->GetT1() - pair->GetT2();

    Double_t tROut  = (tDX * tPx + tDY * tPy) / tPt;
    Double_t tRSide = (-tDX * tPy + tDY * tPx) / tPt;

    //   cout << "Got points 1 " << track1->GetEmissionPoint()->x() << "  " <<
    //   track1->GetEmissionPoint()->y() << " "  <<
    //   track1->GetEmissionPoint()->z() << "  " <<
    //   track1->GetEmissionPoint()->t() << endl;

    //   cout << "Got points 2 " << track2->GetEmissionPoint()->x() << "  " <<
    //   track2->GetEmissionPoint()->y() << " " << track2->GetEmissionPoint()->z()
    //   << "  " << track2->GetEmissionPoint()->t() << endl;
    this->fRStarSide = tRSide;

    this->fRStarLong        = tGamma * (tRLong - tBeta * tDTime);
    Double_t tDTimePairLCMS = tGamma * (tDTime - tBeta * tRLong);

    tBeta  = tPt / tMt;
    tGamma = tMt / tM;

    this->fRStarOut = tGamma * (tROut - tBeta * tDTimePairLCMS);
    this->fRStar =
      ::sqrt(this->fRStarOut * this->fRStarOut + this->fRStarSide * this->fRStarSide + this->fRStarLong * this->fRStarLong);
    this->fKStar =
      ::sqrt(this->fKStarOut * this->fKStarOut + this->fKStarSide * this->fKStarSide + this->fKStarLong * this->fKStarLong);

    //   cout << "Got out side " << this->fRStarOut << " " << this->fRStarSide <<
    //   endl;

    if (!SetPid(pair->GetPdg1(), pair->GetPdg2())) {
      fWeightDen = 1.;
      return 1;
    } else {  // Good Pid
      // AliFemtoThreeVector*  p;
      // p=(track1->GetTrueMomentum());
      double p1[] = {pair->TruePx1(), pair->TruePy1(), pair->TruePz1()};
      // p=(track2->GetTrueMomentum());
      double p2[] = {pair->TruePx2(), pair->TruePy2(), pair->TruePz2()};
      if ((p1[0] == p2[0]) && (p1[1] == p2[1]) && (p1[2] == p2[2])) {
        fWeightDen = 0.;
        return 0;
      }
      // AliFemtoLorentzVector* tPoint;
      // tPoint=(track1->GetEmissionPoint());
      //     cout << "Pid1:dans GetWeight = " << aThPair->GetPid1() << endl;
      //     cout << "Pid2:dans GetWeight = " << aThPair->GetPid2() << endl;
      //     cout << "LL:in GetWeight = " << mLL << endl;

      double x1[] = {pair->GetX1(), pair->GetY1(), pair->GetZ1(), pair->GetT1()};
      // tPoint=(track2->GetEmissionPoint());
      double x2[] = {pair->GetX2(), pair->GetY2(), pair->GetZ2(), pair->GetT2()};
      if ((x1[0] == x2[0]) && (x1[1] == x2[1]) && (x1[2] == x2[2]) && (x1[3] == x2[3])) {
        fWeightDen = 0.;
        return 0;
      }
      if (fSwap) {
        fParser->SetMomentum(p2, p1);
        fParser->SetPosition(x2, x1);
      } else {
        fParser->SetPosition(x1, x2);
        fParser->SetMomentum(p1, p2);
      }
      FsiSetLL();
      fParser->Ltran12();
      fParser->Fsiw(1, fWeif, fWei, fWein);

      if (fI3c == 0) return fWein;
      fWeightDen = fWeif;
      return fWei;
    }
  }

  void FemtoWeightGeneratorLednicky2::SetPairType(Femto::EPairType aPairType) {
    this->fPairType              = aPairType;
    std::pair<Int_t, Int_t> pair = Femto::PairTypeToPid(fPairType);
    SetPid(pair.first, pair.second);
  }

  void FemtoWeightGeneratorLednicky2::SetPairTypeFromPairAndConfigureFSI(FemtoPair* pair) {
    const Int_t ktPid1 = pair->GetPdg1();
    const Int_t ktPid2 = pair->GetPdg2();
    fIch               = 0;
    fIqs               = 0;
    fIsi               = 0;

    if (((ktPid1 == 211) && (ktPid2 == 211)) || ((ktPid1 == -211) && (ktPid2 == -211))) {
      this->fPairType = Femto::EPairType::kPionPlusPionPlus;
      fIch            = 1;
      fIqs            = 1;
    } else if (((ktPid1 == -211) && (ktPid2 == 211)) || ((ktPid1 == 211) && (ktPid2 == -211))) {
      this->fPairType = Femto::EPairType::kPionPlusPionMinus;
      fIch            = 1;
    } else if (((ktPid1 == 321) && (ktPid2 == 321)) || ((ktPid1 == -321) && (ktPid2 == -321))) {
      this->fPairType = Femto::EPairType::kKaonPlusKaonPlus;
      fIch            = 1;
      fIqs            = 1;
    } else if (((ktPid1 == -321) && (ktPid2 == 321)) || ((ktPid1 == 321) && (ktPid2 == -321))) {
      this->fPairType = Femto::EPairType::kKaonPlusKaonMinus;
      fIch            = 1;
    } else if (((ktPid1 == 2212) && (ktPid2 == 2212)) || ((ktPid1 == -2212) && (ktPid2 == -2212))) {
      this->fPairType = Femto::EPairType::kProtonProton;
      fIch            = 1;
      fIqs            = 1;
      fIsi            = 1;
    } else if (((ktPid1 == -2212) && (ktPid2 == 2212)) || ((ktPid1 == 2212) && (ktPid2 == -2212))) {
      this->fPairType = Femto::EPairType::kProtonAntiproton;
      fIch            = 1;
      fIsi            = 1;
    } else if (((ktPid1 == 211) && (ktPid2 == 321)) || ((ktPid1 == -211) && (ktPid2 == -321))) {
      this->fPairType = Femto::EPairType::kPionPlusKaonPlus;
      fIch            = 1;
    } else if (((ktPid1 == -211) && (ktPid2 == 321)) || ((ktPid1 == 211) && (ktPid2 == -321))) {
      this->fPairType = Femto::EPairType::kPionPlusKaonMinus;
      fIch            = 1;
    } else if (((ktPid1 == 211) && (ktPid2 == 2212)) || ((ktPid1 == -211) && (ktPid2 == -2212))) {
      this->fPairType = Femto::EPairType::kPionPlusProton;
      fIch            = 1;
      fIsi            = 1;
    } else if (((ktPid1 == -211) && (ktPid2 == 2212)) || ((ktPid1 == 211) && (ktPid2 == -2212))) {
      this->fPairType = Femto::EPairType::kPionPlusAntiproton;
      fIch            = 1;
      fIsi            = 1;
    } else if (((ktPid1 == 321) && (ktPid2 == 2212)) || ((ktPid1 == -321) && (ktPid2 == -2212))) {
      this->fPairType = Femto::EPairType::kKaonPlusProton;
      fIch            = 1;
      fIsi            = 1;
    } else if (((ktPid1 == -321) && (ktPid2 == 2212)) || ((ktPid1 == 321) && (ktPid2 == -2212))) {
      this->fPairType = Femto::EPairType::kKaonPlusAntiproton;
      fIch            = 1;
      fIsi            = 1;
    } else if (((ktPid1 == 3122) && (ktPid2 == 2212)) || ((ktPid1 == -3122) && (ktPid2 == -2212))) {
      fIsi = 1;
    } else if (((ktPid1 == 3122) && (ktPid2 == 3122)) || ((ktPid1 == -3122) && (ktPid2 == -3122))) {
      fIqs = 1;
      fIsi = 1;
    }
    FsiInit();
    SetPid(ktPid1, ktPid2);
  }
  void FemtoWeightGeneratorLednicky2::SetPairTypeFromPair(FemtoPair* pair) {
    Femto::EPairType type = Femto::PidToPairType(pair->GetPdg1(), pair->GetPdg2());
    fPairType             = type;
    SetPid(pair->GetPdg1(), pair->GetPdg2());
  }

  Femto::EPairType FemtoWeightGeneratorLednicky2::GetPairType() const { return this->fPairType; }

  Double_t FemtoWeightGeneratorLednicky2::GetKStar() const { return FemtoWeightGenerator::GetKStar(); }

  Double_t FemtoWeightGeneratorLednicky2::GetKStarOut() const { return FemtoWeightGenerator::GetKStarOut(); }

  Double_t FemtoWeightGeneratorLednicky2::GetKStarSide() const { return FemtoWeightGenerator::GetKStarSide(); }

  Double_t FemtoWeightGeneratorLednicky2::GetKStarLong() const { return FemtoWeightGenerator::GetKStarLong(); }

  Double_t FemtoWeightGeneratorLednicky2::GetRStar() const { return FemtoWeightGenerator::GetRStar(); }

  Double_t FemtoWeightGeneratorLednicky2::GetRStarOut() const { return FemtoWeightGenerator::GetRStarOut(); }

  Double_t FemtoWeightGeneratorLednicky2::GetRStarSide() const { return FemtoWeightGenerator::GetRStarSide(); }

  Double_t FemtoWeightGeneratorLednicky2::GetRStarLong() const { return FemtoWeightGenerator::GetRStarLong(); }

  FemtoWeightGenerator* FemtoWeightGeneratorLednicky2::Clone(const char* /*newname*/) const {
    FemtoWeightGenerator* tmp = new FemtoWeightGeneratorLednicky2(*this);
    return tmp;
  }

  void FemtoWeightGeneratorLednicky2::SetDefaultCalcPar() {
    fItest = 1;
    fIqs   = 1;
    fIsi   = 1;
    fI3c   = 0;
    fIch   = 1;
    FsiInit();
    fSphereApp = false;
    fT0App     = false;
  }

  void FemtoWeightGeneratorLednicky2::SetCoulOn() {
    fItest = 1;
    fIch   = 1;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky2::SetCoulOff() {
    fItest = 1;
    fIch   = 0;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky2::SetQuantumOn() {
    fItest = 1;
    fIqs   = 1;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky2::SetQuantumOff() {
    fItest = 1;
    fIqs   = 0;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky2::SetStrongOn() {
    fItest = 1;
    fIsi   = 1;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky2::SetStrongOff() {
    fItest = 1;
    fIsi   = 0;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky2::Set3BodyOn() {
    fItest = 1;
    fI3c   = 1;
    FsiInit();
    FsiNucl();
  }

  void FemtoWeightGeneratorLednicky2::Set3BodyOff() {
    fItest = 1;
    fI3c   = 0;
    FsiInit();
    fWeightDen = 1.;
    FsiNucl();
  }

  void FemtoWeightGeneratorLednicky2::SetSphere() { fSphereApp = true; }

  void FemtoWeightGeneratorLednicky2::SetSquare() { fSphereApp = false; }

  void FemtoWeightGeneratorLednicky2::SetT0ApproxOff() { fT0App = false; }

  void FemtoWeightGeneratorLednicky2::SetT0ApproxOn() { fT0App = true; }

  void FemtoWeightGeneratorLednicky2::SetNuclCharge(const double aNuclCharge) {
    fNuclCharge = aNuclCharge;
    FsiNucl();
  }

  Bool_t FemtoWeightGeneratorLednicky2::IsPairSupported(Femto::EPairType type) const {
    if (static_cast<int>(type) < 200) return kTRUE;
    return kFALSE;
  }

  void FemtoWeightGeneratorLednicky2::SetNuclMass(const double aNuclMass) {
    fNuclMass = aNuclMass;
    FsiNucl();
  }

  void FemtoWeightGeneratorLednicky2::FsiInit() {
    // Initialize weight generation module
    //   cout << "*******************AliFemtoModelWeightGeneratorLednicky check
    //   FsiInit ************" << endl; cout <<"mItest dans FsiInit() = " <<
    //   fItest << endl; cout <<"mIch dans FsiInit() = " << fIch << endl; cout
    //   <<"mIqs dans FsiInit() = " << fIqs << endl; cout <<"mIsi dans FsiInit() =
    //   " << fIsi << endl; cout <<"mI3c dans FsiInit() = " << fI3c << endl;
    fParser->Fsiin(fItest, fIch, fIqs, fIsi, fI3c);
  }

  Bool_t FemtoWeightGeneratorLednicky2::Init(Int_t task_id, FemtoPair* /*pair*/) {
    FsiInit();
    fTaskID = task_id;
    return kTRUE;
  }
}  // namespace Hal
