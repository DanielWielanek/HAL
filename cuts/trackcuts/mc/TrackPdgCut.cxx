/*
 * TrackPdgCut.cxx
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackPdgCut.h"

#include "DataFormatManager.h"
#include "McTrack.h"

namespace Hal {

  TrackPdgCut::TrackPdgCut() : TrackMCCut(1) { SetUnitName("Pdg code"); }

  TrackPdgCut::~TrackPdgCut() {
    // TODO Auto-generated destructor stub
  }

  Bool_t TrackPdgCut::Pass(Track* track) {
    SetValue(((McTrack*) track)->GetPdg());
    return Validate();
  }

  TrackPdgAbsCut::TrackPdgAbsCut() : TrackMCCut(1) { SetUnitName("Pdg code"); }

  Bool_t TrackPdgAbsCut::Pass(Track* track) {
    SetValue(((McTrack*) track)->GetPdg());
    return ValidateAbs();
  }

  TrackPdgAbsCut::~TrackPdgAbsCut() {}
}  // namespace Hal
