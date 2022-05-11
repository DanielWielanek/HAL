/*
 * TrackOnlyPrimariesCut.cxx
 *
 *  Created on: 07-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackOnlyPrimariesCut.h"

#include "McTrack.h"

namespace Hal {
  TrackOnlyPrimariesCut::TrackOnlyPrimariesCut() : TrackCut(1) {
    SetUnitName("IsPrimary");
    SetMinAndMax(1);
  }

  Bool_t TrackOnlyPrimariesCut::Pass(Track* track) {
    if (track->IsPrimary()) {
      SetValue(1);
      return Validate();
    } else {
      SetValue(0);
      return Validate();
    }
  }

  TrackOnlyPrimariesCut::~TrackOnlyPrimariesCut() {}
}  // namespace Hal
