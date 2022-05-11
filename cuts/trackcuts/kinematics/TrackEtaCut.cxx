/*
 * TrackEtaCut.cxx
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackEtaCut.h"

#include "Track.h"

namespace Hal {
  TrackEtaCut::TrackEtaCut() : TrackCut(1) { SetUnitName("#eta"); }

  TrackEtaCut::~TrackEtaCut() {}

  Bool_t TrackEtaCut::Pass(Track* track) {
    Double_t px = track->GetPx();
    Double_t py = track->GetPy();
    Double_t pz = track->GetPz();
    Double_t p  = TMath::Sqrt(px * px + py * py + pz * pz);
    if (p == pz) {
      SetValue(kMaxInt);
      return Validate();
    }
    Double_t eta = 0.5 * TMath::Log((p + pz) / (p - pz));
    SetValue(eta);
    return Validate();
  }

  //- Eta abs ------------------

  TrackEtaAbsCut::TrackEtaAbsCut() : TrackCut(1) { SetUnitName("|#eta|"); }

  Bool_t TrackEtaAbsCut::Pass(Track* track) {
    Double_t px = track->GetPx();
    Double_t py = track->GetPy();
    Double_t pz = track->GetPz();
    Double_t p  = TMath::Sqrt(px * px + py * py + pz * pz);
    if (p == pz) {
      SetValue(kMaxInt);
      return ValidateAbs();
    }
    if (p == -pz) {
      SetValue(kMaxInt);
      return ValidateAbs();
    }
    Double_t eta = 0.5 * TMath::Log((p + pz) / (p - pz));
    SetValue(TMath::Abs(eta), 0);
    return Validate();
  }

  TrackEtaAbsCut::~TrackEtaAbsCut() {}

  Bool_t TrackEtaAbsCut::Init(Int_t task_id) {
    if (TrackCut::Init(task_id) == kFALSE) return kFALSE;
    return kTRUE;
  }
}  // namespace Hal
