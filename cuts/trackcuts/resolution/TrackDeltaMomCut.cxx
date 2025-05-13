/*
 * TrackDeltaMomCut.cxx
 *
 *  Created on: 28 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackDeltaMomCut.h"

#include "Track.h"

#include <TLorentzVector.h>

namespace Hal {
  TrackDeltaPtCut::TrackDeltaPtCut() : TrackDeltaMomCut(1) { SetUnitName("#Deltap_{T} [GeV/c]"); }

  Bool_t TrackDeltaPtCut::Pass(Track* track) {
    if (!GetMom(track)) {
      SetValue(-1E+9);
      return ForcedUpdate(kFALSE);
    }
    Double_t pT_reco = fTrackRe->GetMomentum().Pt();
    Double_t pT_mc   = fTrackIm->GetMomentum().Pt();
    SetValue(pT_mc - pT_reco);
    return Validate();
  }

  TrackDeltaPtCut::~TrackDeltaPtCut() {}

  /*-----------------------------------------------------------------------------------------------------------*/

  TrackDeltaPCut::TrackDeltaPCut() : TrackDeltaMomCut(1) { SetUnitName("#Deltap [GeV/c]"); }

  Bool_t TrackDeltaPCut::Pass(Track* track) {
    if (!GetMom(track)) {
      SetValue(-1E+9);
      return ForcedUpdate(kFALSE);
    }
    Double_t p_mc   = fTrackIm->GetMomentum().P();
    Double_t p_reco = fTrackRe->GetMomentum().P();
    SetValue(p_mc - p_reco);
    return Validate();
  }

  TrackDeltaPCut::~TrackDeltaPCut() {}

  /*-----------------------------------------------------------------------------------------------------------*/

  TrackDeltaPzCut::TrackDeltaPzCut() : TrackDeltaMomCut() { SetUnitName("#Deltap_{z} [GeV/c]"); }

  Bool_t TrackDeltaPzCut::Pass(Track* track) {
    if (!GetMom(track)) {
      SetValue(-1E+9);
      return ForcedUpdate(kFALSE);
    }
    Double_t p_mc   = fTrackIm->GetMomentum().Pz();
    Double_t p_reco = fTrackRe->GetMomentum().Pz();
    SetValue(p_mc - p_reco);
    return Validate();
  }

  TrackDeltaPzCut::~TrackDeltaPzCut() {}

  /*-----------------------------------------------------------------------------------------------------------*/

  TrackDeltaPhiCut::TrackDeltaPhiCut() : TrackDeltaMomCut(1) {
    SetUnitName("#Delta#phi");
    SetMinMax(-TMath::Pi(), TMath::Pi());
  }

  Bool_t TrackDeltaPhiCut::Pass(Track* track) {
    if (!GetMom(track)) {
      SetValue(-1E+9);
      return ForcedUpdate(kFALSE);
    }
    Double_t p_mc   = fTrackIm->GetMomentum().Phi();
    Double_t p_reco = fTrackRe->GetMomentum().Phi();
    SetValue(TVector2::Phi_mpi_pi(p_mc - p_reco));
    return Validate();
  }

  TrackDeltaPhiCut::~TrackDeltaPhiCut() {}

  /*-----------------------------------------------------------------------------------------------------------*/

  TrackDeltaThetaCut::TrackDeltaThetaCut() : TrackDeltaMomCut(1) {
    SetUnitName("#Delta#theta");
    SetMinMax(-TMath::PiOver2(), TMath::PiOver2());
  }

  Bool_t TrackDeltaThetaCut::Pass(Track* track) {
    if (!GetMom(track)) {
      SetValue(-1E+9);
      return ForcedUpdate(kFALSE);
    }
    Double_t p_mc   = fTrackIm->GetMomentum().Theta();
    Double_t p_reco = fTrackRe->GetMomentum().Theta();
    SetValue(TVector2::Phi_mpi_pi(p_mc - p_reco));
    return Validate();
  }

  TrackDeltaThetaCut::~TrackDeltaThetaCut() {}

  /*-----------------------------------------------------------------------------------------------------------*/

  TrackDeltaVectPtCut::TrackDeltaVectPtCut() { SetUnitName("#Deltap_{T} [GeV/c]"); }

  Bool_t TrackDeltaVectPtCut::Pass(Track* track) {
    if (!GetMom(track)) {
      SetValue(-1E+9);
      return ForcedUpdate(kFALSE);
    }
    SetValue(fDiff.Pt());
    return Validate();
  }

  TrackDeltaVectPtCut::~TrackDeltaVectPtCut() {}

  /*-----------------------------------------------------------------------------------------------------------*/

  TrackDeltaVectPCut::TrackDeltaVectPCut() { SetUnitName("#Deltap [GeV/c]"); }

  Bool_t TrackDeltaVectPCut::Pass(Track* track) {
    if (!GetMom(track)) {
      SetValue(-1E+9);
      return ForcedUpdate(kFALSE);
    }
    SetValue(fDiff.P());
    return Validate();
  }

  TrackDeltaVectPCut::~TrackDeltaVectPCut() {}

  /*-----------------------------------------------------------------------------------------------------------*/

  TrackDeltaVectPhiCut::TrackDeltaVectPhiCut() {
    SetUnitName("#Delta#phi");
    SetMinMax(-TMath::Pi(), TMath::Pi());
  }

  Bool_t TrackDeltaVectPhiCut::Pass(Track* track) {
    if (!GetMom(track)) {
      SetValue(-1E+9);
      return ForcedUpdate(kFALSE);
    }
    SetValue(fDiff.Phi());
    return Validate();
  }

  TrackDeltaVectPhiCut::~TrackDeltaVectPhiCut() {}

  /*-----------------------------------------------------------------------------------------------------------*/

  TrackDeltaVectThetaCut::TrackDeltaVectThetaCut() {
    SetUnitName("#Delta#theta");
    SetMinMax(-TMath::PiOver2(), TMath::PiOver2());
  }

  Bool_t TrackDeltaVectThetaCut::Pass(Track* track) {
    if (!GetMom(track)) {
      SetValue(-1E+9);
      return ForcedUpdate(kFALSE);
    }
    SetValue(fDiff.Theta());
    return Validate();
  }

  TrackDeltaVectThetaCut::~TrackDeltaVectThetaCut() {}
}  // namespace Hal
