/*
 * TrackV0Cut.cxx
 *
 *  Created on: 10 cze 2024
 *      Author: daniel
 */

#include "TrackV0Cut.h"

#include "Track.h"
#include "V0Track.h"

namespace Hal {

  Bool_t TrackV0Cut::Pass(Hal::Track* track) {
    auto v0 = track->GetV0Info();
    if (!v0) {
      for (int i = 0; i < GetCutSize(); i++) {
        SetValue(-1E+9, i);
      }
      return ForcedUpdate(kFALSE);
    }
    return PassV0(v0);
  }

} /* namespace Hal */
