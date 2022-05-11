/*
 * TwoTrackSailorCowboyCut.cxx
 *
 *  Created on: 15 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackSailorCowboyCut.h"

#include "HelixBase.h"
#include "Package.h"
#include "Parameter.h"

#include <RtypesCore.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TVector2.h>
#include <TVector3.h>

#include "Cut.h"
#include "Event.h"
#include "ExpTrack.h"
#include "Track.h"
#include "TwoTrack.h"

namespace Hal {
  TwoTrackSailorCowboyCut::TwoTrackSailorCowboyCut() : TwoTrackCut(1) {
    SetUnitName("SC [AU]", 0);
    SetMinMax(-1, 1);
    fThreshold = 0;
    // TODO check for fixed target
  }

  Bool_t TwoTrackSailorCowboyCut::Pass(TwoTrack* pair) {
    ExpTrackHelix* tr1 = (ExpTrackHelix*) pair->GetTrack1();
    ExpTrackHelix* tr2 = (ExpTrackHelix*) pair->GetTrack2();
    const Helix& h1    = tr1->GetHelix();
    const Helix& h2    = tr2->GetHelix();
    TVector3 X1        = h1.GetStartPoint() - tr1->GetEvent()->GetVertex()->Vect();
    TVector3 P1        = tr1->GetMomentum().Vect();
    TVector3 X2        = h2.GetStartPoint() - tr2->GetEvent()->GetVertex()->Vect();
    TVector3 P2        = tr2->GetMomentum().Vect();
#ifdef HALHELIX_X
    X1.SetXYZ(X1.Y(), X1.Z(), Z1.X());
    X2.SetXYZ(X2.Y(), X2.Z(), Z2.X());
    P1.SetXYZ(P1.Y(), P1.Z(), P1.X());
    P2.SetXYZ(P2.Y(), P2.Z(), P2.X());
#else
#ifdef HALHELIX_Y
    X1.SetXYZ(X1.Z(), X1.X(), Z1.Y());
    X2.SetXYZ(X2.Z(), X2.X(), Z2.Y());
    P1.SetXYZ(P1.Z(), P1.X(), P1.Y());
    P2.SetXYZ(P2.Z(), P2.X(), P2.Y());
#endif
#endif
    switch (pair->GetPairType()) {
      case TwoTrack::kRotated: {
        X2.SetX(-X2.X());
        X2.SetY(-X2.Y());
        P2.SetX(-P2.X());
        P2.SetY(-P2.Y());
      } break;
      case TwoTrack::kHemishpere: {
        X2.SetX(-X2.X());
        X2.SetY(-X2.Y());
        X2.SetZ(-X2.Z());
        P2.SetX(-P2.X());
        P2.SetY(-P2.Y());
        P2.SetZ(-P2.Z());
      } break;
      default: break;
    }
    fHz1 = HelixZ(X1, P1, tr1->GetCharge());
    fHz2 = HelixZ(X2, P2, tr2->GetCharge());
    return Check();
  }

  TwoTrackSailorCowboyCut::~TwoTrackSailorCowboyCut() {}

  Double_t TwoTrackSailorCowboyCut::NormalizeAngle(const TVector3 pos, Double_t x, Double_t y) const {
    Double_t dx = pos.X() - x;
    Double_t dy = pos.Y() - y;
    return TMath::ATan2(dy, dx);
  }

  Bool_t TwoTrackSailorCowboyCut::AreBetween(Double_t phi, Double_t sign) const {
    if (sign < 0) {
      if (phi > 0) return kFALSE;
    } else {
      if (phi < 0) return kFALSE;
    }
    return kTRUE;
  }

  Bool_t TwoTrackSailorCowboyCut::Check() {
    Double_t sign1 = fHz1.GetRotationDirection();
    Double_t sign2 = fHz2.GetRotationDirection();
    Double_t r2    = 1.0 / fHz2.GetCurv();
    Double_t x2    = fHz2.GetXcenter();
    Double_t y2    = fHz2.GetYcenter();
    Double_t x1    = fHz1.GetXcenter();
    Double_t y1    = fHz1.GetYcenter();
    Double_t phi1  = fHz1.GetPhi0();
    Double_t phi2  = fHz2.GetPhi0();
    Double_t s1, s2;
    fHz1.PathLength(r2, x2, y2, s1, s2);

    TVector3 pos1 = fHz1.EvalPos(s1);
    TVector3 pos2 = fHz1.EvalPos(s2);
    if (s1 == s2 && s1 == Helix::MaxPath()) {  // sailor
      SetValue(Sailor());
      return Validate();
    }
    Double_t phi1_1 = TVector2::Phi_mpi_pi(NormalizeAngle(pos1, x1, y1) - phi1);
    Double_t phi2_1 = TVector2::Phi_mpi_pi(NormalizeAngle(pos2, x1, y1) - phi1);
    Double_t phi1_2 = TVector2::Phi_mpi_pi(NormalizeAngle(pos1, x2, y2) - phi2);
    Double_t phi2_2 = TVector2::Phi_mpi_pi(NormalizeAngle(pos2, x2, y2) - phi2);
    if (AreBetween(phi1_1, sign1) && AreBetween(phi1_2, sign2)) {  // first point is overlap
      if (pos1.Pt() >= fThreshold) {
        SetValue(Cowboy());
        return Validate();
      }
    }
    if (AreBetween(phi2_1, sign1) && AreBetween(phi2_2, sign2)) {  // first point is overlap
      if (pos2.Pt() >= fThreshold) {
        SetValue(Cowboy());
        return Validate();
      }
    }
    SetValue(Sailor());
    return Validate();
  }

  Package* TwoTrackSailorCowboyCut::Report() const {
    Package* report = TwoTrackCut::Report();
    report->AddObject(new ParameterDouble("Threshold", fThreshold));
    return report;
  }

  TwoTrackSailorCowboyCut::TwoTrackSailorCowboyCut(const TwoTrackSailorCowboyCut& other) : TwoTrackCut(other) {
    fHz1       = other.fHz1;
    fHz2       = other.fHz2;
    fThreshold = other.fThreshold;
  }

  TwoTrackSailorCowboyCut& TwoTrackSailorCowboyCut::operator=(const TwoTrackSailorCowboyCut& other) {
    if (&other != this) {
      fHz1       = other.fHz1;
      fHz2       = other.fHz2;
      fThreshold = other.fThreshold;
    }
    return *this;
  }

  Bool_t TwoTrackSailorCowboyCut::Init(Int_t formatId) { return FormatInhertis("ExpEventHelix", formatId); }
}  // namespace Hal
