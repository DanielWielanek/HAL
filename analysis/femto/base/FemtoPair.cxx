/*
 * FemtoPair.cpp
 *
 *  Created on: 29-05-2014
 *      Author: wielanek
 */

#include "FemtoPair.h"

#include "ComplexTrack.h"
#include "Cout.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "FemtoMiniPair.h"
#include "McTrack.h"
#include "Track.h"
#include "TwoTrack.h"

#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TParticlePDG.h>
#include <cstdio>
#include <iostream>


namespace Hal {
  FemtoPair::FemtoPair(Bool_t use_fake) :
    fTrack1(NULL),
    fTrack2(NULL),
    fPx1(0.0),
    fPy1(0.0),
    fPz1(0.0),
    fE1(0.0),
    fPx2(0.0),
    fPy2(0.0),
    fPz2(0.0),
    fE2(0.0),
    fpx1(0.0),
    fpy1(0.0),
    fpz1(0.0),
    fe1(0.0),
    fpx2(0.0),
    fpy2(0.0),
    fpz2(0.0),
    fe2(0.0),
    fX1(0.0),
    fY1(0.0),
    fZ1(0.0),
    fT1(0.0),
    fX2(0.0),
    fY2(0.0),
    fZ2(0.0),
    fT2(0.0),
    fPdg1(0),
    fPdg2(0),
    fM12(-1.0),
    fM22(-1.0),
    fUseImgMomenta(use_fake),
    fX(0),
    fY(0),
    fZ(0),
    fT(0),
    fWeight(1.0),
    fMode(kNoMC),
    fUseAbs(kFALSE) {}

  void FemtoPair::SetTrueMomenta1(Double_t px, Double_t py, Double_t pz, Double_t e) {
    fPx1 = px;
    fPy1 = py;
    fPz1 = pz;
    fE1  = e;
  }

  void FemtoPair::SetTrueMomenta2(Double_t px, Double_t py, Double_t pz, Double_t e) {
    fPx2 = px;
    fPy2 = py;
    fPz2 = pz;
    fE2  = e;
  }

  Bool_t FemtoPair::Init(Int_t task_id) {
    TDatabasePDG* pdg   = TDatabasePDG::Instance();
    TParticlePDG* part1 = pdg->GetParticle(fPdg1);
    TParticlePDG* part2 = pdg->GetParticle(fPdg2);
    if (part1) {
      Double_t m1 = part1->Mass();
      fM12        = m1 * m1;
    }
    if (part2) {
      Double_t m1 = part2->Mass();
      fM22        = m1 * m1;
    }
    if (task_id < 0) {
      fMode = kNoMC;
      return kTRUE;
    }
    DataFormatManager* mngr = DataFormatManager::Instance();
    const Event* event      = mngr->GetFormat(task_id);
    Track* tr               = event->GetNewTrack();
    Bool_t use_mc           = kFALSE;
    if (tr->InheritsFrom("Hal::MCTrack")) { use_mc = kTRUE; }
    Bool_t complex = kFALSE;
    if (tr->InheritsFrom("Hal::ComplexTrack")) { complex = kTRUE; }
    delete tr;
    if (use_mc) {
      fMode = kMC;
    } else if (complex) {
      if (fUseImgMomenta) {
        fMode = kComplexIm;
      } else {
        fMode = kComplexRe;
      }
    }
    if (fUseImgMomenta) {
      if (event->InheritsFrom("Hal::ComplexEvent")) return kTRUE;
      Cout::PrintInfo("Cannot use fake momenta without combiend event!", EInfo::kLessWarning);
      return kFALSE;
    } else {
      return kTRUE;
    }
  }

  void FemtoPair::SetMass(Double_t m1, Double_t m2) {
    if (m1 > 0) fM12 = m1 * m1;
    if (m2 > 0) fM22 = m2 * m2;
  }

  FemtoPair& FemtoPair::operator=(const FemtoMiniPair& other) {
    fPx1  = other.TruePx1();
    fPx2  = other.TruePx2();
    fPy1  = other.TruePy1();
    fPy2  = other.TruePy2();
    fPz1  = other.TruePz1();
    fPz2  = other.TruePz2();
    fE1   = other.TrueE1();
    fE2   = other.TrueE2();
    fpx1  = other.GetPx1();
    fpx2  = other.GetPx2();
    fpy1  = other.GetPy1();
    fpy2  = other.GetPy2();
    fpz1  = other.GetPz1();
    fpz2  = other.GetPz2();
    fe1   = other.TrueE1();
    fe2   = other.TrueE2();
    fPdg1 = other.GetPdg1();
    fPdg2 = other.GetPdg2();
    fX1   = other.GetX1();
    fX2   = other.GetX2();
    fY1   = other.GetY1();
    fY2   = other.GetY2();
    fZ1   = other.GetZ1();
    fZ2   = other.GetZ2();
    fT1   = other.GetT1();
    fT2   = other.GetT2();
    return *this;
  }

  void FemtoPair::BuildMomenta(TwoTrack* tracks) {
    fTrack1 = tracks->GetTrack1();
    fTrack2 = tracks->GetTrack2();
    switch (fMode) {
      case kMC: {
        McTrack* track1           = (McTrack*) fTrack1;
        McTrack* track2           = (McTrack*) fTrack2;
        const TLorentzVector& p1  = track1->GetMomentum();
        const TLorentzVector& p2  = track2->GetMomentum();
        const TLorentzVector& pt1 = track1->GetMomentum();
        const TLorentzVector& pt2 = track2->GetMomentum();
        fPdg1                     = track1->GetPdg();
        fPdg2                     = track2->GetPdg();
        const TLorentzVector& x1  = track1->GetFreezoutPosition();
        const TLorentzVector& x2  = track2->GetFreezoutPosition();
        SetFreezoutCoord1(x1.X(), x1.Y(), x1.Z(), x1.T());
        SetFreezoutCoord2(x2.X(), x2.Y(), x2.Z(), x2.T());
        SetTrueMomenta1(pt1.Px(), pt1.Py(), pt1.Pz(), pt1.E());
        SetTrueMomenta2(pt2.Px(), pt2.Py(), pt2.Pz(), pt2.E());
        SetMomenta1(p1.Px(), p1.Py(), p1.Pz());
        SetMomenta2(p2.Px(), p2.Py(), p2.Pz());

      } break;
      case kComplexIm: {
        McTrack* imtr1 = (McTrack*) ((ComplexTrack*) fTrack1)->GetImgTrack();
        McTrack* imtr2 = (McTrack*) ((ComplexTrack*) fTrack2)->GetImgTrack();
        if (imtr1 && imtr2) {
          const TLorentzVector& pt1 = imtr1->GetMomentum();
          const TLorentzVector& pt2 = imtr2->GetMomentum();
          const TLorentzVector& x1  = imtr1->GetFreezoutPosition();
          const TLorentzVector& x2  = imtr2->GetFreezoutPosition();
          fPdg1                     = imtr1->GetPdg();
          fPdg2                     = imtr2->GetPdg();
          SetFreezoutCoord1(x1.X(), x1.Y(), x1.Z(), x1.T());
          SetFreezoutCoord2(x2.X(), x2.Y(), x2.Z(), x2.T());
          SetTrueMomenta1(pt1.Px(), pt1.Py(), pt1.Pz(), pt1.E());
          SetTrueMomenta2(pt2.Px(), pt2.Py(), pt2.Pz(), pt2.E());

        } else {
          const TLorentzVector& p1  = fTrack1->GetMomentum();
          const TLorentzVector& p2  = fTrack2->GetMomentum();
          const TLorentzVector& pt1 = p1;
          const TLorentzVector& pt2 = p2;
          SetTrueMomenta1(pt1.Px(), pt1.Py(), pt1.Pz());
          SetTrueMomenta2(pt2.Px(), pt2.Py(), pt2.Pz());
        }
        const TLorentzVector& p1 = imtr1->GetMomentum();
        const TLorentzVector& p2 = imtr2->GetMomentum();
        SetMomenta1(p1.Px(), p1.Py(), p1.Pz());
        SetMomenta2(p2.Px(), p2.Py(), p2.Pz());

      } break;
      case kComplexRe: {
        McTrack* imtr1           = (McTrack*) ((ComplexTrack*) fTrack1)->GetImgTrack();
        McTrack* imtr2           = (McTrack*) ((ComplexTrack*) fTrack2)->GetImgTrack();
        const TLorentzVector& p1 = fTrack1->GetMomentum();
        const TLorentzVector& p2 = fTrack2->GetMomentum();
        if (imtr1 && imtr2) {
          const TLorentzVector& pt1 = imtr1->GetMomentum();
          const TLorentzVector& pt2 = imtr2->GetMomentum();
          const TLorentzVector& x1  = imtr1->GetFreezoutPosition();
          const TLorentzVector& x2  = imtr2->GetFreezoutPosition();
          fPdg1                     = imtr1->GetPdg();
          fPdg2                     = imtr2->GetPdg();
          SetFreezoutCoord1(x1.X(), x1.Y(), x1.Z(), x1.T());
          SetFreezoutCoord2(x2.X(), x2.Y(), x2.Z(), x2.T());
          SetTrueMomenta1(pt1.Px(), pt1.Py(), pt1.Pz(), pt1.E());
          SetTrueMomenta2(pt2.Px(), pt2.Py(), pt2.Pz(), pt2.E());
        } else {
          SetTrueMomenta1(p1.Px(), p1.Py(), p1.Pz());
          SetTrueMomenta2(p2.Px(), p2.Py(), p2.Pz());
        }
        SetMomenta1(p1.Px(), p1.Py(), p1.Pz());
        SetMomenta2(p2.Px(), p2.Py(), p2.Pz());
      } break;
      case kNoMC: {
        const TLorentzVector& p1  = fTrack1->GetMomentum();
        const TLorentzVector& p2  = fTrack2->GetMomentum();
        const TLorentzVector& pt1 = p1;
        const TLorentzVector& pt2 = p2;
        SetTrueMomenta1(pt1.Px(), pt1.Py(), pt1.Pz());
        SetTrueMomenta2(pt2.Px(), pt2.Py(), pt2.Pz());
        SetMomenta1(p1.Px(), p1.Py(), p1.Pz());
        SetMomenta2(p2.Px(), p2.Py(), p2.Pz());
        return;
      } break;
    }
  }

  void FemtoPair::SetMomenta1(Double_t px, Double_t py, Double_t pz, Double_t e) {
    fpx1 = px;
    fpy1 = py;
    fpz1 = pz;
    fe1  = e;
  }

  void FemtoPair::SetMomenta2(Double_t px, Double_t py, Double_t pz, Double_t e) {
    fpx2 = px;
    fpy2 = py;
    fpz2 = pz;
    fe2  = e;
  }

  void FemtoPair::PrintInfo() const {
    std::cout << "pair info" << std::endl;
    Double_t m1 = TMath::Sqrt(fE1 * fE1 - fPx1 * fPx1 - fPy1 * fPy1 - fPz1 * fPz1);
    Double_t m2 = TMath::Sqrt(fE2 * fE2 - fPx2 * fPx2 - fPy2 * fPy2 - fPz2 * fPz2);
    printf("momenta (%.4f %.4f %.4f %.4f) (%.4f %.4f %.4f %.4f) m1 = %.4f m2 = %.4f "
           "\n",
           GetPx1(),
           GetPy1(),
           GetPz1(),
           GetE1(),
           GetPx2(),
           GetPy2(),
           GetPz2(),
           GetE2(),
           m1,
           m2);
    printf("True momenta (%.4f %.4f %.4f %.4f) (%.4f %.4f %.4f %.4f) m1 = %.4f m2 = "
           "%.4f \n",
           TruePx1(),
           TruePy1(),
           TruePz1(),
           TrueE1(),
           TruePx2(),
           TruePy2(),
           TruePz2(),
           TrueE2(),
           m1,
           m2);
    printf("coords (%.4f %.4f %.4f %.4f) (%.4f %.4f %.4f %.4f)\n",
           GetX1(),
           GetY1(),
           GetZ1(),
           GetT1(),
           GetX2(),
           GetY2(),
           GetZ2(),
           GetT2());
  }

  FemtoPair::~FemtoPair() {}

  void FemtoPair::SetTrueMomenta1(Double_t px, Double_t py, Double_t pz) {
    fPx1 = px;
    fPy1 = py;
    fPz1 = pz;
    fE1  = TMath::Sqrt(fPx1 * fPx1 + fPy1 * fPy1 + fPz1 * fPz1 + fM12);
  }

  void FemtoPair::SetTrueMomenta2(Double_t px, Double_t py, Double_t pz) {
    fPx2 = px;
    fPy2 = py;
    fPz2 = pz;
    fE2  = TMath::Sqrt(fPx2 * fPx2 + fPy2 * fPy2 + fPz2 * fPz2 + fM22);
  }

  void FemtoPair::SetMomenta1(Double_t px, Double_t py, Double_t pz) {
    fpx1 = px;
    fpy1 = py;
    fpz1 = pz;
    fe1  = TMath::Sqrt(fpx1 * fpx1 + fpy1 * fpy1 + fpz1 * fpz1 + fM12);
  }

  void FemtoPair::SetMomenta2(Double_t px, Double_t py, Double_t pz) {
    fpx2 = px;
    fpy2 = py;
    fpz2 = pz;
    fe2  = TMath::Sqrt(fpx2 * fpx2 + fpy2 * fpy2 + fpz2 * fpz2 + fM22);
  }

  Double_t FemtoPair::GetKt() const {
    Double_t tPx = fpx1 + fpx2;
    Double_t tPy = fpy1 + fpy2;
    return 0.5 * TMath::Sqrt(tPx * tPx + tPy * tPy);
  }

  Double_t FemtoPair::GetTrueKt() const {
    Double_t tPx = fPx1 + fPx2;
    Double_t tPy = fPy1 + fPy2;
    return 0.5 * TMath::Sqrt(tPx * tPx + tPy * tPy);
  }

  void FemtoPair::SetFreezoutCoord1(Double_t x, Double_t y, Double_t z, Double_t t) {
    fX1 = x;
    fY1 = y;
    fZ1 = z;
    fT1 = t;
  }

  void FemtoPair::SetFreezoutCoord2(Double_t x, Double_t y, Double_t z, Double_t t) {
    fX2 = x;
    fY2 = y;
    fZ2 = z;
    fT2 = t;
  }

  FemtoPair& FemtoPair::operator=(const FemtoPicoPair& other) {
    fPx1 = other.TruePx1();
    fPx2 = other.TruePx2();
    fPy1 = other.TruePy1();
    fPy2 = other.TruePy2();
    fPz1 = other.TruePz1();
    fPz2 = other.TruePz2();
    fE1  = other.TrueE1();
    fE2  = other.TrueE2();
    fpx1 = fPx1;
    fpx2 = fPx2;
    fpy1 = fPy1;
    fpy2 = fPy2;
    fpz1 = fPz1;
    fpz2 = fPz2;
    fe1  = fE1;
    fe2  = fE2;
    return *this;
  }

  FemtoPair& FemtoPair::operator=(const FemtoMicroPair& other) {
    fPx1  = other.TruePx1();
    fPx2  = other.TruePx2();
    fPy1  = other.TruePy1();
    fPy2  = other.TruePy2();
    fPz1  = other.TruePz1();
    fPz2  = other.TruePz2();
    fE1   = other.TrueE1();
    fE2   = other.TrueE2();
    fpx1  = other.GetPx1();
    fpx2  = other.GetPx2();
    fpy1  = other.GetPy1();
    fpy2  = other.GetPy2();
    fpz1  = other.GetPz1();
    fpz2  = other.GetPz2();
    fe1   = other.GetE1();
    fe2   = other.GetE2();
    fPdg1 = other.GetPdg1();
    fPdg2 = other.GetPdg2();
    return *this;
  }
}  // namespace Hal
