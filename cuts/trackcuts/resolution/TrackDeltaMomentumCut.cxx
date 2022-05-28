/*
 * TrackDeltaCut.cxx
 *
 *  Created on: 20 gru 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackDeltaMomentumCut.h"

#include "ComplexTrack.h"
#include "DataFormatManager.h"
#include "Track.h"

#include <iostream>

namespace Hal {
  TrackDeltaMomentumCut::TrackDeltaMomentumCut(Int_t params) : TrackCut(params), fTrackRe(nullptr), fTrackIm(nullptr) {
    if (params == 5) {
      SetUnitName("#Deltap_{T} [GeV/c]", Pt());
      SetUnitName("#Delta#phi [rad]", Phi());
      SetUnitName("#Delta#theta [GeV/c]", Theta());
      SetUnitName("#Deltap_{z} [GeV/c]", Pz());
      SetUnitName("#Deltap [GeV/c]", P());
      SetMinMax(-TMath::Pi(), TMath::Pi(), Phi());
      SetMinMax(-TMath::PiOver2(), TMath::PiOver2(), Theta());
      SetMinMax(-1E+6, 1E+6, P());
      SetMinMax(-1E+6, 1E+6, Pt());
      SetMinMax(-1E+6, 1E+6, Pz());
    }
  }

  Bool_t TrackDeltaMomentumCut::Init(Int_t format_id) {
    if (FormatInhertis("Hal::ComplexEvent", format_id)) { return kTRUE; }
    return kFALSE;
  }

  TrackDeltaMomentumCut::~TrackDeltaMomentumCut() {}

  /*-----------------------------------------------------------------------------------------------------------*/

  TrackDeltaMomCut::TrackDeltaMomCut(Int_t params) : TrackDeltaMomentumCut(params) {}

  Bool_t TrackDeltaMomCut::Pass(Track* track) {
    if (!GetMom(track)) {
      SetValue(-1E+9);
      return ForcedUpdate(kFALSE);
    }
    SetValue(fTrackIm->GetMomentum().P() - fTrackRe->GetMomentum().P(), P());
    SetValue(fTrackIm->GetMomentum().Pt() - fTrackRe->GetMomentum().Pt(), Pt());
    SetValue(fTrackIm->GetMomentum().Pz() - fTrackRe->GetMomentum().Pz(), Pz());
    SetValue(TVector2::Phi_mpi_pi(fTrackIm->GetMomentum().Phi() - fTrackRe->GetMomentum().Phi()), Phi());
    SetValue(fTrackIm->GetMomentum().Theta() - fTrackRe->GetMomentum().Theta(), Theta());
    Double_t p_mc   = fTrackIm->GetMomentum().P();
    Double_t p_reco = fTrackRe->GetMomentum().P();
    SetValue(p_mc - p_reco);
    return Validate();
  }

  Bool_t TrackDeltaMomCut::GetMom(Track* track) {
    ComplexTrack* tr = (ComplexTrack*) track;
    fTrackRe         = tr->GetRealTrack();
    fTrackIm         = tr->GetImgTrack();
    if (fTrackIm == NULL) return kFALSE;
    return kTRUE;
  }

  TrackDeltaMomCut::~TrackDeltaMomCut() {}

  /*-----------------------------------------------------------------------------------------------------------*/

  TrackDeltaVectMomCut::TrackDeltaVectMomCut(Int_t params) : TrackDeltaMomentumCut(params) {}

  Bool_t TrackDeltaVectMomCut::GetMom(Track* track) {
    ComplexTrack* tr = (ComplexTrack*) track;
    Track* real      = tr->GetRealTrack();
    Track* im        = tr->GetImgTrack();
    if (im == NULL) return kFALSE;
    fDiff = im->GetMomentum() - real->GetMomentum();
    return kTRUE;
  }

  Bool_t TrackDeltaVectMomCut::Pass(Track* track) {
    if (!GetMom(track)) {
      SetValue(-1E+9);
      return ForcedUpdate(kFALSE);
    }
    SetValue(fDiff.P(), P());
    SetValue(fDiff.Pt(), Pt());
    SetValue(fDiff.Pz(), Pz());
    SetValue(fDiff.Phi(), Phi());
    SetValue(fDiff.Theta(), Theta());
    return Validate();
  }

  TrackDeltaVectMomCut::~TrackDeltaVectMomCut() {}
}  // namespace Hal
