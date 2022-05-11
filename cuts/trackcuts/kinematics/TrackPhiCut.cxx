/*
 * TrackPhiCut.cxx
 *
 *  Created on: 16-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackPhiCut.h"

#include "Track.h"

namespace Hal{

TrackPhiCut::TrackPhiCut() : TrackCut(1), fRound(kFALSE) {
  SetUnitName("#phi");
  SetMinMax(-TMath::Pi(), TMath::Pi());
}

Bool_t TrackPhiCut::Pass(Track* track) {
  Double_t phi = TMath::ATan2(track->GetPy(), track->GetPx());
  SetValue(phi);
  if (fRound) {
    if (phi < GetMax() || phi > GetMin()) {
      ForcedUpdate(kTRUE);
      return kTRUE;
    } else {
      ForcedUpdate(kFALSE);
      return kFALSE;
    }
  } else {
    return Validate();
  }
}

Bool_t TrackPhiCut::Init(Int_t /*task_id*/) {
  fInit = kTRUE;
  while (GetMax(0) > TMath::Pi()) {
    SetMaximum(GetMax() - TMath::Pi() * 2.0);
  }
  while (GetMin(0) > TMath::Pi()) {
    SetMinimum(GetMin() - TMath::Pi() * 2.0);
  }
  while (GetMax(0) < -TMath::Pi()) {
    SetMaximum(GetMax() + TMath::Pi() * 2.0);
  }
  while (GetMin(0) < -TMath::Pi()) {
    SetMinimum(GetMin() + TMath::Pi() * 2.0);
  }
  if (GetMax(0) < GetMin(0)) { fRound = kTRUE; }
  return kTRUE;
}

TrackPhiCut::~TrackPhiCut() {}
}
