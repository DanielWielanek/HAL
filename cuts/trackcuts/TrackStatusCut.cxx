/*
 * TrackStatusCut.cxx
 *
 *  Created on: 15-07-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackStatusCut.h"

#include "Track.h"

namespace Hal {
  TrackStatusCut::TrackStatusCut() : TrackCut(1) { SetUnitName("Status"); }

  Bool_t TrackStatusCut::Pass(Track* track) {
    SetValue(track->GetStatus());
    return Validate();
  }

  TrackStatusCut::~TrackStatusCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
