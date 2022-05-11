/*
 * TrackBoosteMomentaCuts.cxx
 *
 *  Created on: 2 sie 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackBoostedMomentaCuts.h"

#include "McTrack.h"
#include "ComplexTrack.h"
#include "Track.h"
#include <TLorentzVector.h>


namespace Hal {
  TrackBoostedPCut::TrackBoostedPCut() : TrackCut(1) { SetUnitName("P_{boosted} [GeV/c]"); }

  Bool_t TrackBoostedPCut::Pass(Track* track) {
    TLorentzVector mom = track->GetMomentum();
    mom.SetXYZM(track->GetPx(), track->GetPy(), track->GetPz(), track->GetMass());
    mom.Boost(fBoost);
    SetValue(mom.P());
    return Validate();
  }

  TrackBoostedDeltaPCut::TrackBoostedDeltaPCut() : TrackCut(1) { SetUnitName("#DeltaP_{boosted} [GeV/c]"); }

  Bool_t TrackBoostedDeltaPCut::Init(Int_t format_id) {
    fInit = kTRUE;
    return FormatInhertis("ComplexEvent", format_id);
  }

  Bool_t TrackBoostedDeltaPCut::Pass(Track* track) {
    ComplexTrack* tr = (ComplexTrack*) track;
    Track* re_tr     = tr->GetRealTrack();
    Track* im_tr     = tr->GetImgTrack();
    TLorentzVector re_mom, im_mom;
    im_mom = im_tr->GetMomentum();
    re_mom.SetXYZM(re_tr->GetPx(), re_tr->GetPy(), re_tr->GetPz(), im_tr->GetMass());
    re_mom.Boost(fBoost);
    im_mom.Boost(fBoost);
    SetValue(re_mom.P() - im_mom.P());
    return Validate();
  }

  TrackBoostedKinematcisCut::TrackBoostedKinematcisCut() : TrackCut(7) {
    SetUnitName("Px_{boosted} [GeV/c]", Px());
    SetUnitName("Py_{boosted} [GeV/c]", Py());
    SetUnitName("Pz_{boosted} [GeV/c]", Pz());
    SetUnitName("Pt_{boosted} [GeV/c]", Pt());
    SetUnitName("P_{boosted} [GeV/c]", P());
    SetUnitName("#eta_{boosted} []", Eta());
    SetUnitName("Y_{boosted} []", Rapidity());
  }

  Bool_t TrackBoostedKinematcisCut::Pass(Track* track) {
    McTrack* mc        = (McTrack*) track;
    TLorentzVector vec = mc->GetMomentum();
    vec.Boost(fBoost);
    SetValue(vec.Px(), Px());
    SetValue(vec.Py(), Py());
    SetValue(vec.Pz(), Pz());
    SetValue(vec.Pt(), Pt());
    SetValue(vec.P(), P());
    SetValue(vec.Eta(), Eta());
    SetValue(vec.Rapidity(), Rapidity());
    return Validate();
  }

  Bool_t TrackBoostedKinematcisCut::Init(Int_t format_id) {
    fInit = kTRUE;
    if (FormatInhertis("MCEvent", format_id)) { return kTRUE; }
    return kFALSE;
  }

  TrackBoostedKinematcisCut::~TrackBoostedKinematcisCut() {}
}  // namespace Hal
