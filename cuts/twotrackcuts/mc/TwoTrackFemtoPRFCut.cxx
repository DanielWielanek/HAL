/*
 * TwoTrackFemtoPRFCut.cxx
 *
 *  Created on: 25 lis 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TwoTrackFemtoPRFCut.h"

#include "DataFormatManager.h"
#include "Event.h"
#include "McTrack.h"
#include "TwoTrack.h"
namespace Hal {
  TwoTrackFemtoPRFCut::TwoTrackFemtoPRFCut() : TwoTrackCut(2) {
    SetUnitName("k*", 0);
    SetUnitName("r*", 1);
  }

  Bool_t TwoTrackFemtoPRFCut::Pass(TwoTrack* pair) {
    McTrack* tr1 = (McTrack*) pair->GetTrack1();
    McTrack* tr2 = (McTrack*) pair->GetTrack2();
    Double_t x, y, z;
    fP1.SetPxPyPzE(tr1->GetPx(), tr1->GetPy(), tr1->GetPz(), tr1->GetE());
    const TLorentzVector& fr1 = tr1->GetFreezeoutPosition();
    fX1.SetXYZT(fr1.X(), fr1.Y(), fr1.Z(), fr1.T());
    switch (pair->GetPairType()) {
      case TwoTrack::kHemishpere: {
        const TLorentzVector& fr2 = tr2->GetFreezeoutPosition();
        fX2.SetXYZT(-fr2.X(), -fr2.Y(), fr2.Z(), fr2.T());
        fP2.SetXYZT(-tr2->GetPx(), -tr2->GetPy(), tr2->GetPz(), tr2->GetE());
      } break;
      case TwoTrack::kRotated: {
        const TLorentzVector& fr2 = tr2->GetFreezeoutPosition();
        fX2.SetXYZT(-fr2.X(), -fr2.Y(), -fr2.Z(), fr2.T());
        fP2.SetXYZT(-tr2->GetPx(), -tr2->GetPy(), -tr2->GetPz(), tr2->GetE());
      } break;
      default: {
        const TLorentzVector& fr2 = tr2->GetFreezeoutPosition();
        fX2.SetXYZT(fr2.X(), fr2.Y(), fr2.Z(), fr2.T());
        fP2.SetXYZT(tr2->GetPx(), tr2->GetPy(), tr2->GetPz(), tr2->GetE());
      } break;
    }
    Compute();

    x = fX1.X() - fX2.X();
    y = fX1.Y() - fX2.Y();
    z = fX1.Z() - fX2.Z();

    Double_t r = TMath::Sqrt(x * x + y * y + z * z);
    Double_t k = TMath::Abs(fP1.P());
    // if(fP1.X()<0) k = -k;

    SetValue(k, 0);
    SetValue(r, 1);
    return Validate();
  }

  void TwoTrackFemtoPRFCut::Compute() {
    TLorentzVector P = fP1 + fP2;
    Double_t vz      = P.Pz() / P.E();
    P.Boost(0, 0, -vz);  //
    Double_t phi = P.Phi();
    // bost in out direction
    Double_t beta = P.Pt() / P.E();
    fP1.Boost(0, 0, -vz);
    fP1.RotateZ(-phi);
    fP1.Boost(-beta, 0, 0);
    // bost in out direction
    fX1.Boost(0, 0, -vz);
    fX2.Boost(0, 0, -vz);
    // rotation
    fX1.RotateZ(-phi);
    fX2.RotateZ(-phi);
    fX1.Boost(-beta, 0, 0);
    fX2.Boost(-beta, 0, 0);
  }

  Bool_t TwoTrackFemtoPRFCut::Init(Int_t task_id) {
    if (TwoTrackCut::Init() == kFALSE) return kFALSE;
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kBuffered);
    if (ev->InheritsFrom("Hal::McEvent")) {
      return kTRUE;
    } else {
      return kFALSE;
    }
  }

  TwoTrackFemtoPRFCut::~TwoTrackFemtoPRFCut() {}


  TwoTrackFemtoPRFCutFull::TwoTrackFemtoPRFCutFull() : TwoTrackCut(8) {
    SetUnitName("k^{*}_{out} [GeV/c]", 0);
    SetUnitName("k^{*}_{side} [GeV/c]", 1);
    SetUnitName("k^{*}_{long} [GeV/c]", 2);
    SetUnitName("k^{*}_{en} [GeV/c]", 3);

    SetUnitName("r^{*}_{out} [fm/c]", 4);
    SetUnitName("r^{*}_{side} [fm/c]", 5);
    SetUnitName("r^{*}_{long} [fm/c]", 6);
    SetUnitName("t^{*} [fm/c]", 7);
  }

  Bool_t TwoTrackFemtoPRFCutFull::Pass(TwoTrack* pair) {
    McTrack* tr1 = (McTrack*) pair->GetTrack1();
    McTrack* tr2 = (McTrack*) pair->GetTrack2();
    Double_t x, y, z, t;
    fP1.SetPxPyPzE(tr1->GetPx(), tr1->GetPy(), tr1->GetPz(), tr1->GetE());
    const TLorentzVector& fr1 = tr1->GetFreezeoutPosition();
    fX1.SetXYZT(fr1.X(), fr1.Y(), fr1.Z(), fr1.T());
    switch (pair->GetPairType()) {
      case TwoTrack::kHemishpere: {
        const TLorentzVector& fr2 = tr2->GetFreezeoutPosition();
        fX2.SetXYZT(-fr2.X(), -fr2.Y(), fr2.Z(), fr2.T());
        fP2.SetXYZT(-tr2->GetPx(), -tr2->GetPy(), tr2->GetPz(), tr2->GetE());
      } break;
      case TwoTrack::kRotated: {
        const TLorentzVector& fr2 = tr2->GetFreezeoutPosition();
        fX2.SetXYZT(-fr2.X(), -fr2.Y(), -fr2.Z(), fr2.T());
        fP2.SetXYZT(-tr2->GetPx(), -tr2->GetPy(), -tr2->GetPz(), tr2->GetE());
      } break;
      default: {
        const TLorentzVector& fr2 = tr2->GetFreezeoutPosition();
        fX2.SetXYZT(fr2.X(), fr2.Y(), fr2.Z(), fr2.T());
        fP2.SetXYZT(tr2->GetPx(), tr2->GetPy(), tr2->GetPz(), tr2->GetE());
      } break;
    }
    Compute();

    x = fX1.X() - fX2.X();
    y = fX1.Y() - fX2.Y();
    z = fX1.Z() - fX2.Z();
    t = fX1.T() - fX2.T();

    SetValue(x, RStarOut());
    SetValue(y, RStarSide());
    SetValue(z, RStarLong());
    SetValue(t, TStar());

    SetValue(fP1.X(), KStarOut());
    SetValue(fP1.Y(), KStarSide());
    SetValue(fP1.Z(), KStarLong());
    SetValue(fP1.T(), KStarEn());
    return Validate();
  }

  void TwoTrackFemtoPRFCutFull::Compute() {
    TLorentzVector P = fP1 + fP2;
    Double_t vz      = P.Pz() / P.E();
    P.Boost(0, 0, -vz);  //
    Double_t phi = P.Phi();
    // bost in out direction
    Double_t beta = P.Pt() / P.E();
    fP1.Boost(0, 0, -vz);
    fP1.RotateZ(-phi);
    fP1.Boost(-beta, 0, 0);
    // bost in out direction
    fX1.Boost(0, 0, -vz);
    fX2.Boost(0, 0, -vz);
    // rotation
    fX1.RotateZ(-phi);
    fX2.RotateZ(-phi);
    fX1.Boost(-beta, 0, 0);
    fX2.Boost(-beta, 0, 0);
  }

  Bool_t TwoTrackFemtoPRFCutFull::Init(Int_t task_id) {
    if (TwoTrackCut::Init() == kFALSE) return kFALSE;
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kBuffered);
    if (ev->InheritsFrom("Hal::McEvent")) {
      return kTRUE;
    } else {
      return kFALSE;
    }
  }

  TwoTrackFemtoPRFCutFull::~TwoTrackFemtoPRFCutFull() {}

}  // namespace Hal
