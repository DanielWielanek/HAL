/*
 * TrackFreezoutCut.cxx
 *
 *  Created on: 05-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackFreezoutCut.h"

#include "McTrack.h"
#include "DataFormatManager.h"

namespace Hal{

TrackFreezoutCut::TrackFreezoutCut() : TrackMCCut(4) {
  SetUnitName("X [fm]", 0);
  SetUnitName("Y [fm]", 1);
  SetUnitName("Z [fm]", 2);
  SetUnitName("T [fm/c]", 3);
  SetMinMax(-1000, 1000, 0);
  SetMinMax(-1000, 1000, 1);
  SetMinMax(-1000, 1000, 2);
  SetMinMax(-1000, 1000, 3);
}

Bool_t TrackFreezoutCut::Pass(Track* track) {
  const TLorentzVector& fr = ((McTrack*) track)->GetFreezoutPosition();
  SetValue(fr.X(), 0);
  SetValue(fr.Y(), 1);
  SetValue(fr.Z(), 2);
  SetValue(fr.T(), 3);
  return Validate();
}

TrackFreezoutCut::~TrackFreezoutCut() {}

TrackTFreezCut::TrackTFreezCut() : TrackMCCut(1) { SetUnitName("T_{freez} [fm]"); }

Bool_t TrackTFreezCut::Pass(Track* track) {
  SetValue(((McTrack*) track)->GetFreezoutPosition().T());
  return Validate();
}

TrackTFreezCut::~TrackTFreezCut() {}

TrackTauCut::TrackTauCut() : TrackMCCut(1) { SetUnitName("#tau [fm/c]"); }

Bool_t TrackTauCut::Pass(Track* track) {
  Double_t z, t;
  z = ((McTrack*) track)->GetFreezoutPosition().Z();
  t = ((McTrack*) track)->GetFreezoutPosition().T();
  if (TMath::Abs(z) > TMath::Abs(t)) {
    SetValue(TMath::Sqrt(-t * t + z * z));
    ForcedUpdate(kFALSE);
    return kFALSE;
  }
  Double_t tau = TMath::Sqrt(t * t - z * z);
  SetValue(tau);
  return Validate();
}

TrackTauCut::~TrackTauCut() {}
}
