/*
 * PairDeltaQinvCut.cxx
 *
 *  Created on: 12 lis 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "PairDeltaQinvCut.h"

#include "ComplexTrack.h"
#include "Track.h"
#include "TwoTrack.h"

namespace Hal {
  PairDeltaQinvCut::PairDeltaQinvCut() : TwoTrackCut(2) {
    SetUnitName("#DeltaQ_{inv} [GeV.c]", Absolute());
    SetUnitName("#DeltaQ_{inv} [%]", Relative());
    SetMinMax(-1E+9, 1E+9, Absolute());
    SetMinMax(-1000, 1000, Relative());
    fMass1 = fMass2 = 0.139;
  }

  Bool_t PairDeltaQinvCut::Pass(TwoTrack* pair) {
    ComplexTrack* z1 = (ComplexTrack*) pair->GetTrack1();
    ComplexTrack* z2 = (ComplexTrack*) pair->GetTrack2();
    Track* re1       = z1->GetRealTrack();
    Track* re2       = z2->GetRealTrack();
    Track* mc1       = z1->GetImgTrack();
    Track* mc2       = z2->GetImgTrack();
    if (mc1 == NULL || mc2 == NULL) {
      SetValue(1E+9, Absolute());
      SetValue(1E+9, Relative());
      return ForcedUpdate(kFALSE);
    }
    Double_t q_im  = Q(mc1, mc2);
    Double_t q_re  = Q(re1, re2);
    Double_t delta = q_re - q_im;
    SetValue(delta, Absolute());
    SetValue(delta / q_im * 100.0, Relative());
    return Validate();
  }

  Double_t PairDeltaQinvCut::Q(Track* tr1, Track* tr2) const {
    Double_t px = tr1->GetPx() - tr2->GetPx();
    Double_t py = tr1->GetPy() - tr2->GetPy();
    Double_t pz = tr1->GetPz() - tr2->GetPz();
    Double_t p1 = tr1->GetPx() * tr1->GetPx() + tr1->GetPy() * tr1->GetPy() + tr1->GetPz() * tr1->GetPz();
    Double_t p2 = tr2->GetPx() * tr2->GetPx() + tr2->GetPy() * tr2->GetPy() + tr2->GetPz() * tr2->GetPz();
    Double_t e1 = TMath::Sqrt(p1 + fMass1);
    Double_t e2 = TMath::Sqrt(p2 + fMass2);
    Double_t e  = e1 - e2;
    return TMath::Sqrt(TMath::Abs((e * e - px * px - py * py - pz * pz)));
  }

  Bool_t PairDeltaQinvCut::Init(Int_t task_id) {
    fMass1 = fMass1 * fMass1;
    fMass2 = fMass2 * fMass2;
    return TwoTrackCut::Init(task_id);
  }

  PairDeltaQinvCut::~PairDeltaQinvCut() {}
}  // namespace Hal
