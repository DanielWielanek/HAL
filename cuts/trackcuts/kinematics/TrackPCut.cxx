/*
 * TrackPCut.cxx
 *
 *  Created on: 28 mar 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackPCut.h"

#include "Track.h"

namespace Hal {

  TrackPCut::TrackPCut() : TrackCut(1) { SetUnitName("p [GeV/c]"); }

  Bool_t TrackPCut::Pass(Track* track) {
    SetValue(track->GetMomentum().P());
    return Validate();
  }

  TrackPCut::~TrackPCut() {}

  TrackPSignedCut::TrackPSignedCut() { SetUnitName("p_{signed} [GeV/c]"); }

  Bool_t TrackPSignedCut::Pass(Track* track) {
    if (track->GetCharge() >= 0)
      SetValue(track->GetMomentum().P());
    else
      SetValue(-track->GetMomentum().P());
    return Validate();
  }

  TrackPSignedCut::~TrackPSignedCut() {}
}  // namespace Hal
