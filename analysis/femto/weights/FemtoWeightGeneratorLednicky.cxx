/*
 * FemtoWeightGeneratorLednicky.cxx
 *
 *  Created on: 21-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoWeightGeneratorLednicky.h"


#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <utility>

#include "Cout.h"
#include "FemtoConst.h"
#include "FemtoPair.h"
#include "Package.h"
#include "Parameter.h"
#include <iostream>

#ifdef SOLARIS
#ifndef false
typedef int bool;
#define false 0
#define true 1
#endif
#endif

#ifdef WIN32
#ifdef CERNLIB_MSSTDCALL
#define F77_UCASE
#define type_of_call _stdcall
#ifndef CERNLIB_QXCAPT
#define CERNLIB_QXCAPT
#endif
#else
#define F77_LCASE
#ifndef CERNLIB_QXNO_SC
#define CERNLIB_QXNO_SC
#endif
#endif
#define type_of_call _stdcall
#define DEFCHARD const char*, const int
#define DEFCHARL
#define PASSCHARD(string) string, strlen(string)
#define PASSCHARL(string)
#else
#define DEFCHARD const char*
#define DEFCHARL , const int
#define PASSCHARD(string) string
#define PASSCHARL(string) , strlen(string)
#endif
#ifdef CERNLIB_QXCAPT
#define F77_NAME(name, NAME) NAME
#else
#if defined(CERNLIB_QXNO_SC)
#define F77_NAME(name, NAME) name
#else
#define F77_NAME(name, NAME) name##_
#endif
#endif
#ifndef type_of_call
#define type_of_call
#endif

//#define LEDNICKY_CHEATING
#define OLD_LEDNICKY  // keep compatibility with old lednicky code

// --- Prototype of the function used in the weight calculator
//     (in FsiWeightLedinicky.F)

#ifdef OLD_LEDNICKY
#define fsiin F77_NAME(fsiin, FSIIN)
extern "C" {
void type_of_call F77_NAME(fsiin, FSIIN)(const int& itest, const int& ich, const int& iqs, const int& isi, const int& i3c);
}

#else
#define fsiini F77_NAME(fsiini, FSIINI)
extern "C" {
void type_of_call F77_NAME(
  fsiini,
  FSIINI)(const int& itest, const int& ill, const int& ins, const int& ich, const int& iqs, const int& isi, const int& i3c);
}
#endif
#define llini F77_NAME(llini, LLINI)
extern "C" {
void type_of_call F77_NAME(llini, LLINI)(const int& lll, const int& ns, const int& itest);
}

#define fsinucl F77_NAME(fsinucl, FSINUCL)
extern "C" {
void type_of_call F77_NAME(fsinucl, FSINUCL)(const double& mn, const double& cn);
}
#define fsimomentum F77_NAME(fsimomentum, FSIMOMENTUM)
extern "C" {
void type_of_call F77_NAME(fsimomentum, FSIMOMENTUM)(double& p1, double& p2);
}
#define fsiposition F77_NAME(fsiposition, FSIPOSITION)
extern "C" {
void type_of_call F77_NAME(fsiposition, FSIPOSITION)(double& x1, double& x2);
}
#define fsiw F77_NAME(fsiw, FSIW)
extern "C" {
void type_of_call F77_NAME(fsiw, FSIW)(const int& i, double& weif, double& wei, double& wein);
}
#define ltran12 F77_NAME(ltran12, LTRAN12)
extern "C" {
void type_of_call ltran12_();
}

// K+K- model type
/*
#define setkpkmmodel F77_NAME(setkpkmmodel, SETKPKMMODEL)
extern "C" {
void type_of_call F77_NAME(setkpkmmodel, SETKPKMMODEL)(const int& i_model, const int& i_PhiOffOn);
}
*/
// Test function for Lambda potential
//#define printlam F77_NAME(printlam,PRINTLAM)
// extern "C" {void type_of_call printlam_();}
// there is not PRINTLAM in *.F file

// --- Additional prototyping of some CERN functions (in FsiTool.F)
typedef float REAL;
typedef struct {
  REAL re;
  REAL im;
} COMPLEX;
#define cgamma F77_NAME(cgamma, CGAMMA)
extern "C" {
COMPLEX type_of_call cgamma_(COMPLEX*);
}
#ifdef __ROOT__
ClassImp(FemtoWeightGeneratorLednicky)
#endif
  namespace Hal {
  FemtoWeightGeneratorLednicky::FemtoWeightGeneratorLednicky() :
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
    fNS(4),
    fNumProcessPair(0),
    fNumbNonId(0),
    fKpKmModel(14),
    fPhi_OffOn(1),
    fNS_4(0) {
    fNumProcessPair = new int[fLLMax + 1];
    int i;
    for (i = 1; i <= fLLMax; i++) {
      fNumProcessPair[i] = 0;
    }
    FsiInit();
    FsiNucl();
  }

  void FemtoWeightGeneratorLednicky::FsiSetLL() {
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
    if (fNS_4 == 4) tNS = 4;  // K+K- analisys
                              /*  std::cout << "fLL dans FsiSetLL() = " << fLL << std::endl;
                                std::cout << "tNS dans FsiSetLL() = " << tNS << std::endl;
                                std::cout << "fItest dans FsiSetLL() = " << fItest << std::endl;*/
    llini(fLL, tNS, fItest);
  }

  void FemtoWeightGeneratorLednicky::FsiNucl() {
    // initialize weight generation taking into account the residual charge
    //   cout << "*******************AliFemtoModelWeightGeneratorLednicky check
    //   FsiNucl ************" << endl; cout <<"fNuclMass dans FsiNucl() = " <<
    //   fNuclMass << endl; cout <<"fNuclCharge dans FsiNucl() = " << fNuclCharge
    //   << endl; cout <<"fNuclChargeSign dans FsiNucl() = " << fNuclChargeSign <<
    //   endl;
    fsinucl(fNuclMass, fNuclCharge * fNuclChargeSign);
  }

  bool FemtoWeightGeneratorLednicky::SetPid(const int aPid1, const int aPid2) {
    // set calculated system for basing on particles' pids
    static const int ksPi0Pid  = 111;
    static const int ksPionPid = 211;
    static const int ksK0Pid   = 130;
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


    fPairType = Femto::PidToPairType(aPid1, aPid2);

    PairIDToLL(fPairType, fLL, tChargeFactor);

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

  FemtoWeightGeneratorLednicky::FemtoWeightGeneratorLednicky(const FemtoWeightGeneratorLednicky& aWeight) :
    FemtoWeightGenerator(aWeight),
    fWei(aWeight.fWei),
    fWein(aWeight.fWein),
    fWeif(aWeight.fWeif),
    fWeightDen(aWeight.fWeightDen),
    fItest(aWeight.fItest),
    fIch(aWeight.fIch),
    fIqs(aWeight.fIqs),
    fIsi(aWeight.fIsi),
    fI3c(aWeight.fI3c),
    fNuclMass(aWeight.fNuclMass),
    fNuclCharge(aWeight.fNuclCharge),
    fSphereApp(aWeight.fSphereApp),
    fT0App(aWeight.fT0App),
    fLL(aWeight.fLL),
    fNuclChargeSign(aWeight.fNuclChargeSign),
    fSwap(aWeight.fSwap),
    fLLMax(aWeight.fLLMax),
    fNS(aWeight.fNS),
    fNumProcessPair(nullptr),
    fNumbNonId(aWeight.fNumbNonId),
    fKpKmModel(aWeight.fKpKmModel),
    fPhi_OffOn(aWeight.fPhi_OffOn),
    fNS_4(aWeight.fNS_4) {
    // copy constructor

    fNumProcessPair = new int[fLLMax + 1];
    int i;
    for (i = 1; i <= fLLMax; i++) {
      fNumProcessPair[i] = 0;
    }

    FsiInit();
    FsiNucl();
  }

  FemtoWeightGeneratorLednicky& FemtoWeightGeneratorLednicky::operator=(const FemtoWeightGeneratorLednicky& aWeight) {
    if (this == &aWeight) return *this;
    FemtoWeightGenerator::operator=(aWeight);
    fWei                          = aWeight.fWei;
    fWein                         = aWeight.fWein;
    fWeif                         = aWeight.fWeif;
    fWeightDen                    = aWeight.fWeightDen;

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
    fNS             = aWeight.fNS;
    fKpKmModel      = aWeight.fKpKmModel;
    fPhi_OffOn      = aWeight.fPhi_OffOn;
    fNS_4           = aWeight.fNS_4;
    if (fNumProcessPair) free(fNumProcessPair);
    fNumProcessPair = new int[fLLMax + 1];
    int i;
    for (i = 1; i <= fLLMax; i++) {
      fNumProcessPair[i] = 0;
    }
    FsiInit();
    FsiNucl();

    return *this;
  }

  FemtoWeightGeneratorLednicky::~FemtoWeightGeneratorLednicky() {
    if (fNumProcessPair) delete[] fNumProcessPair;
  }

  Double_t FemtoWeightGeneratorLednicky::GenerateWeight(FemtoPair * pair) {
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
    SetPid(pair->GetPdg1(), pair->GetPdg2());
    // std::cout << Femto::PairTypeToString(fPairType) << " " << fLL << std::endl;
    if (fPairType == Femto::EPairType::kUnknown) { return 1; }

    // AliFemtoThreeVector*  p;
    // p=(track1->GetTrueMomentum());
    double p1[] = {pair->TruePx1(), pair->TruePy1(), pair->TruePz1()};
    // p=(track2->GetTrueMomentum());
    double p2[] = {pair->TruePx2(), pair->TruePy2(), pair->TruePz2()};
    if ((p1[0] == p2[0]) && (p1[1] == p2[1]) && (p1[2] == p2[2])) {
      fWeightDen = 0.;
      return 0;
    }

    double x1[] = {pair->GetX1(), pair->GetY1(), pair->GetZ1(), pair->GetT1()};
    // tPoint=(track2->GetEmissionPoint());
    double x2[] = {pair->GetX2(), pair->GetY2(), pair->GetZ2(), pair->GetT2()};
    if ((x1[0] == x2[0]) && (x1[1] == x2[1]) && (x1[2] == x2[2]) && (x1[3] == x2[3])) {
      fWeightDen = 0.;
      return 0;
    }
    if (fSwap) {
      fsimomentum(*p2, *p1);
      fsiposition(*x2, *x1);
    } else {
      fsiposition(*x1, *x2);
      fsimomentum(*p1, *p2);
    }
    FsiSetLL();
#ifndef OLD_LEDNICKY
    FsiInit();
#endif
    ltran12();
    fsiw(1, fWeif, fWei, fWein);

    if (fI3c == 0) return fWein;
    fWeightDen = fWeif;

    return fWei;
  }

  void FemtoWeightGeneratorLednicky::SetPairType(Femto::EPairType aPairType) {
    this->fPairType              = aPairType;
    std::pair<Int_t, Int_t> pair = Femto::PairTypeToPid(fPairType);
    SetPid(pair.first, pair.second);
  }

  void FemtoWeightGeneratorLednicky::SetPairTypeFromPairAndConfigureFSI(FemtoPair * pair) {
    const Int_t ktPid1 = pair->GetPdg1();
    const Int_t ktPid2 = pair->GetPdg2();
    fIch               = 0;
    fIqs               = 0;
    fIsi               = 0;

    fPairType = Femto::PidToPairType(ktPid1, ktPid2);
    switch (fPairType) {
        // clang-format off
  //quantum strong coulomb
    case Femto::EPairType::kPionPlusPionPlus: {SetFlags(1, 0, 1);} break;
    case Femto::EPairType::kPionPlusPionMinus: {SetFlags(0, 0, 1);} break;
    case Femto::EPairType::kKaonPlusKaonPlus: {SetFlags(1, 0, 1);} break;
    case Femto::EPairType::kKaonPlusKaonMinus: {SetFlags(0, 0, 1);} break;

    case Femto::EPairType::kProtonProton: {SetFlags(1, 1, 1);} break;
    case Femto::EPairType::kProtonAntiproton: {SetFlags(0, 1, 1);} break;
    case Femto::EPairType::kPionPlusKaonPlus: {SetFlags(0, 1, 1);} break;
    case Femto::EPairType::kPionPlusKaonMinus: {SetFlags(0, 1, 1);} break;

    case Femto::EPairType::kPionPlusProton: {SetFlags(0, 1, 1);} break;
    case Femto::EPairType::kPionPlusAntiproton: {SetFlags(0, 1, 1);} break;
    case Femto::EPairType::kKaonPlusProton: {SetFlags(0, 1, 1);} break;
    case Femto::EPairType::kKaonPlusAntiproton: {SetFlags(0, 1, 1);} break;

    case Femto::EPairType::kProtonLambda: {SetFlags(0, 1, 0);} break;
    case Femto::EPairType::kLambdaLambda: {SetFlags(1, 1, 0);} break;
    case Femto::EPairType::kKaonZeroKaonZero: {SetFlags(1, 1, 0);} break;
    case Femto::EPairType::kKaonZeroKaonZeroBar: {SetFlags(0, 1, 0);} break;

    case Femto::EPairType::kNeutronNeutron: {SetFlags(1, 1, 0);} break;
    case Femto::EPairType::kNeutronProton: {SetFlags(0, 1, 0);} break;
    case Femto::EPairType::kPionZeroPionZero: {SetFlags(1, 0, 0);} break;
    case Femto::EPairType::kNeutronLambda: {SetFlags(0, 1, 0);} break;
        // clang-format on
    }
    FsiInit();
    SetPid(ktPid1, ktPid2);
  }


  void FemtoWeightGeneratorLednicky::SetPairTypeFromPair(FemtoPair * pair) {
    const Int_t ktPid1 = pair->GetPdg1();
    const Int_t ktPid2 = pair->GetPdg2();
    SetPid(ktPid1, ktPid2);
  }

  Femto::EPairType FemtoWeightGeneratorLednicky::GetPairType() const { return this->fPairType; }

  Double_t FemtoWeightGeneratorLednicky::GetKStar() const { return FemtoWeightGenerator::GetKStar(); }

  Double_t FemtoWeightGeneratorLednicky::GetKStarOut() const { return FemtoWeightGenerator::GetKStarOut(); }

  Double_t FemtoWeightGeneratorLednicky::GetKStarSide() const { return FemtoWeightGenerator::GetKStarSide(); }

  Double_t FemtoWeightGeneratorLednicky::GetKStarLong() const { return FemtoWeightGenerator::GetKStarLong(); }

  Double_t FemtoWeightGeneratorLednicky::GetRStar() const { return FemtoWeightGenerator::GetRStar(); }

  Double_t FemtoWeightGeneratorLednicky::GetRStarOut() const { return FemtoWeightGenerator::GetRStarOut(); }

  Double_t FemtoWeightGeneratorLednicky::GetRStarSide() const { return FemtoWeightGenerator::GetRStarSide(); }

  Double_t FemtoWeightGeneratorLednicky::GetRStarLong() const { return FemtoWeightGenerator::GetRStarLong(); }

  FemtoWeightGenerator* FemtoWeightGeneratorLednicky::Clone(const char* /*name*/) const {
    FemtoWeightGenerator* tmp = new FemtoWeightGeneratorLednicky(*this);
    return tmp;
  }

  void FemtoWeightGeneratorLednicky::SetDefaultCalcPar() {
    fItest = 1;
    fIqs   = 1;
    fIsi   = 1;
    fI3c   = 0;
    fIch   = 1;
    FsiInit();
    fSphereApp = false;
    fT0App     = false;
  }

  void FemtoWeightGeneratorLednicky::SetCoulOn() {
    fItest = 1;
    fIch   = 1;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky::SetCoulOff() {
    fItest = 1;
    fIch   = 0;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky::SetQuantumOn() {
    fItest = 1;
    fIqs   = 1;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky::SetQuantumOff() {
    fItest = 1;
    fIqs   = 0;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky::SetStrongOn() {
    fItest = 1;
    fIsi   = 1;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky::SetStrongOff() {
    fItest = 1;
    fIsi   = 0;
    FsiInit();
  }

  void FemtoWeightGeneratorLednicky::Set3BodyOn() {
    fItest = 1;
    fI3c   = 1;
    FsiInit();
    FsiNucl();
  }

  void FemtoWeightGeneratorLednicky::Set3BodyOff() {
    fItest = 1;
    fI3c   = 0;
    FsiInit();
    fWeightDen = 1.;
    FsiNucl();
  }

  void FemtoWeightGeneratorLednicky::SetSphere() { fSphereApp = true; }

  void FemtoWeightGeneratorLednicky::SetSquare() { fSphereApp = false; }

  void FemtoWeightGeneratorLednicky::SetT0ApproxOff() { fT0App = false; }

  void FemtoWeightGeneratorLednicky::SetT0ApproxOn() { fT0App = true; }

  void FemtoWeightGeneratorLednicky::SetNuclCharge(const double aNuclCharge) {
    fNuclCharge = aNuclCharge;
    FsiNucl();
  }

  void FemtoWeightGeneratorLednicky::SetNuclMass(const double aNuclMass) {
    fNuclMass = aNuclMass;
    FsiNucl();
  }

  void FemtoWeightGeneratorLednicky::FsiInit() {
    // Initialize weight generation module
    //   cout << "*******************AliFemtoModelWeightGeneratorLednicky check
    //   FsiInit ************" << endl; cout <<"mItest dans FsiInit() = " <<
    //   fItest << endl; cout <<"mIch dans FsiInit() = " << fIch << endl; cout
    //   <<"mIqs dans FsiInit() = " << fIqs << endl; cout <<"mIsi dans FsiInit() =
    //   " << fIsi << endl; cout <<"mI3c dans FsiInit() = " << fI3c << endl;
#ifdef OLD_LEDNICKY
    fsiin(fItest, fIch, fIqs, fIsi, fI3c);
#else
    fsiini(fItest, fLL, fNS, fIch, fIqs, fIsi, fI3c);
#endif
  }

  Bool_t FemtoWeightGeneratorLednicky::Init(Int_t task_id, FemtoPair * /*pair*/) {
    FsiInit();
    fTaskID = task_id;
    return kTRUE;
  }

  Package* FemtoWeightGeneratorLednicky::Report() const {
    Package* report = FemtoWeightGenerator::Report();
    report->AddObject(new ParameterString("PairType", Femto::PairTypeToString(fPairType)));
    report->AddObject(new ParameterBool("Coulomb", fIch));
    report->AddObject(new ParameterBool("Qstat.", fIqs));
    report->AddObject(new ParameterBool("Strong", fIsi));
    report->AddObject(new ParameterBool("3-body", fI3c));
    report->AddObject(new ParameterDouble("Nucl.Charge", fNuclCharge));
    report->AddObject(new ParameterDouble("Nucl.Mass", fNuclMass));
    report->AddObject(new ParameterInt("Nucl.ChargeSign", fNuclChargeSign));
    report->AddObject(new ParameterBool("SphereApprox.", fSphereApp));
    report->AddObject(new ParameterBool("T0Approx.", fT0App));
    return report;
  }

  void FemtoWeightGeneratorLednicky::PairIDToLL(Femto::EPairType pair_pid, Int_t & LL, Int_t & tChargeFactor) const {

    switch (pair_pid) {
      case Femto::EPairType::kPionPlusPionPlus: {
        LL = 7;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kPionPlusPionMinus: {
        LL = 5;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kKaonPlusKaonPlus: {
        LL = 15;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kKaonPlusKaonMinus: {
        LL = 14;
        tChargeFactor *= 1;
      } break;

      case Femto::EPairType::kProtonProton: {
        LL = 2;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kProtonAntiproton: {
        LL = 30;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kPionPlusKaonPlus: {
        LL = 11;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kPionPlusKaonMinus: {
        LL = 10;
        tChargeFactor *= 1;
      } break;

      case Femto::EPairType::kPionPlusProton: {
        LL = 12;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kPionPlusAntiproton: {
        LL = 13;
        tChargeFactor *= -1;
      } break;
      case Femto::EPairType::kKaonPlusProton: {
        LL = 16;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kKaonPlusAntiproton: {
        LL = 17;
        tChargeFactor *= -1;
      } break;

      case Femto::EPairType::kProtonLambda: {
        LL = 27;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kLambdaLambda: {
        LL = 29;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kKaonZeroKaonZero: {
        LL = 22;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kKaonZeroKaonZeroBar: {
        LL = 23;
        tChargeFactor *= 1;
      } break;

      case Femto::EPairType::kNeutronNeutron: {
        LL = 1;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kNeutronProton: {
        LL = 3;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kPionZeroPionZero: {
        LL = 6;
        tChargeFactor *= 1;
      } break;
      case Femto::EPairType::kNeutronLambda: {
        LL = 28;
        tChargeFactor *= 1;
      } break;


      default: LL = 0; break;
    }
  }

  Femto::EPairType FemtoWeightGeneratorLednicky::LLToPairID(Int_t ll) const {
    switch (ll) {
      case 7: {
        return Femto::EPairType::kPionPlusPionPlus;
      } break;
      case 5: {
        return Femto::EPairType::kPionPlusPionMinus;
      } break;
      case 15: {
        return Femto::EPairType::kKaonPlusKaonPlus;
      } break;
      case 14: {
        return Femto::EPairType::kKaonPlusKaonMinus;
      } break;

      case 2: {
        return Femto::EPairType::kProtonProton;
      } break;
      case 30: {
        return Femto::EPairType::kProtonAntiproton;
      } break;
      case 11: {
        return Femto::EPairType::kPionPlusKaonPlus;
      } break;
      case 10: {
        return Femto::EPairType::kPionPlusKaonMinus;
      } break;

      case 12: {
        return Femto::EPairType::kPionPlusProton;
      } break;
      case 13: {
        return Femto::EPairType::kPionPlusAntiproton;
      } break;
      case 16: {
        return Femto::EPairType::kKaonPlusProton;
      } break;
      case 17: {
        return Femto::EPairType::kKaonPlusAntiproton;
      } break;

      case 27: {
        return Femto::EPairType::kProtonLambda;
      } break;
      case 29: {
        return Femto::EPairType::kLambdaLambda;
      } break;
      case 22: {
        return Femto::EPairType::kKaonZeroKaonZero;
      } break;
      case 23: {
        return Femto::EPairType::kKaonZeroKaonZeroBar;
      } break;

      case 1: {
        return Femto::EPairType::kNeutronNeutron;
      } break;
      case 3: {
        return Femto::EPairType::kNeutronProton;
      } break;
      case 6: {
        return Femto::EPairType::kPionZeroPionZero;
      } break;
      case 28: {
        return Femto::EPairType::kNeutronLambda;  //
      } break;

      default: break;
    }
    return Femto::EPairType::kUnknown;
  }

  void FemtoWeightGeneratorLednicky::Print(Option_t * option) const {
    Cout::Text(ClassName(), "L");
    Cout::Text(Femto::PairTypeToString(fPairType), "L");
    if (fIqs == 1) {
      Cout::FailSucced("Quantum Stat.", "ENABLED ", kGreen);
    } else {
      Cout::FailSucced("Quantum Stat.", "DISABLED", kRed);
    }
    if (fIch == 1) {
      Cout::FailSucced("Coulomb FSI", "ENABLED ", kGreen);
    } else {
      Cout::FailSucced("Coulomb FSI", "DISABLED", kRed);
    }
    if (fIsi == 1) {
      Cout::FailSucced("Strong FSI", "ENABLED ", kGreen);
    } else {
      Cout::FailSucced("Strong FSI", "DISABLED", kRed);
    }
    if (fI3c == 1) {
      Cout::FailSucced("3-body", "ENABLED ", kGreen);
    } else {
      Cout::FailSucced("3-body", "DISABLED", kRed);
    }
    Cout::Text("--", "L");
  }

  void FemtoWeightGeneratorLednicky::FsiSetKpKmModelType(const int aModelType, const int aPhi_OffOn) {
    fKpKmModel = aModelType;
    fPhi_OffOn = aPhi_OffOn;
    fNS_4      = 4;
    FsiSetKpKmModelType();
  }

  void FemtoWeightGeneratorLednicky::FsiSetKpKmModelType() {
    //    setkpkmmodel(fKpKmModel, fPhi_OffOn);
  };

  Bool_t FemtoWeightGeneratorLednicky::IsPairSupported(Femto::EPairType type) const {
    if (static_cast<int>(type) <= 200) return kTRUE;
    return kFALSE;
  }
}  // namespace Hal
