/*
 * TrackV0ArmengerosCut.cxx
 *
 *  Created on: 10 cze 2024
 *      Author: daniel
 */

#include "TrackV0ArmengerosCut.h"

#include "V0Track.h"

namespace Hal {

  Bool_t TrackV0ArmengerosCut::PassV0(V0Track* tr) {
    SetValue(tr->GetPtArm(), 1);
    SetValue(tr->GetAlphaArm(), 0);
    return Validate();
  }

  TrackV0ArmengerosCut::TrackV0ArmengerosCut() : TrackV0Cut(2) {
    SetUnitName("#alpha", 0);
    SetUnitName("p_{T}", 1);
  }

} /* namespace Hal */
