/*
 * TrackTpcHitsCut.cxx
 *
 *  Created on: 4 maj 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackTpcHitCut.h"

#include "ExpTrack.h"
namespace Hal {
  TrackTpcHitCut::TrackTpcHitCut() : TrackExpCut(1) { SetUnitName("TpcHits [N]"); }

  Bool_t TrackTpcHitCut::Pass(Track* tr) {
    ExpTrack* track = (ExpTrack*) tr;
    SetValue(track->GetNHits());
    return Validate();
  }

  Bool_t TrackTpcHitCut::Init(Int_t format_id) {
    if (TpcAvaiable(format_id)) return kTRUE;
    return kFALSE;
  }

  TrackTpcHitCut::~TrackTpcHitCut() {}
}  // namespace Hal
