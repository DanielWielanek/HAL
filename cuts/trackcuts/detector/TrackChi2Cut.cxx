/*
 * TrackChi2Cut.cxx
 *
 *  Created on: 13 lis 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackChi2Cut.h"

#include "ExpTrack.h"

namespace Hal {
  TrackChi2Cut::TrackChi2Cut() : TrackExpCut(1) {
    SetUnitName("#chi^{2}");
    SetMinMax(-1E+9, 1E+9);
  }

  Bool_t TrackChi2Cut::Pass(Track* tr) {
    ExpTrack* track = (ExpTrack*) tr;
    SetValue(track->GetChi2());
    return Validate();
  }

  TrackChi2Cut::~TrackChi2Cut() {}
}  // namespace Hal
