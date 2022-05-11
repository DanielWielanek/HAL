/*
 * V0MassInvCut.cxx
 *
 *  Created on: 26 lut 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "V0MassInvCut.h"

#include "Track.h"
#include "V0Track.h"

namespace Hal {
  V0MassInvCut::V0MassInvCut() : TrackCut(1) { SetUnitName("M_{inv} [GeV/c^2]"); }

  Bool_t V0MassInvCut::Pass(Track* track) {
    if (track->IsV0() == kFALSE) {
      SetValue(-1);
      return ForcedUpdate(kFALSE);
    }
    V0Track* v0 = track->GetV0Info();
    SetValue(v0->GetMom().M());
    return Validate();
  }

  V0MassInvCut::~V0MassInvCut() {}
}  // namespace Hal
