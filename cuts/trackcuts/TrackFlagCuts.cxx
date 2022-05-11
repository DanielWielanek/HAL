/*
 * TrackFlagCuts.cxx
 *
 *  Created on: 2 sty 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackFlagCuts.h"

#include "Track.h"
namespace Hal {
  Bool_t TrackPrimaryFlagCut::Pass(Track* track) {
    if (track->IsPrimary()) {
      SetValue(Primary());
    } else {
      SetValue(NonPrimary());
    }
    return Validate();
  }

  Bool_t TrackV0FlagCut::Pass(Track* track) {
    if (track->IsGoodV0()) {
      SetValue(GoodV0());
    } else if (track->IsV0()) {
      SetValue(BadV0());
    } else {
      SetValue(NonV0());
    }
    return Validate();
  }

  Bool_t TrackGoodSecondaryFlagCut::Pass(Track* track) {
    if (track->IsGoodSecondary()) {
      SetValue(HasMother());
    } else {
      SetValue(IsOrphan());
    }
    return Validate();
  }

  Bool_t TrackGlobalFlagCut::Pass(Track* track) {
    if (track->IsGlobal()) {
      SetValue(GlobalTrack());
    } else {
      SetValue(NonGlobalTrack());
    }
    return Validate();
  }
}  // namespace Hal
