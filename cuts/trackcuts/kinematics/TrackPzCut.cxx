/*
 * TrackPzCut.cxx
 *
 *  Created on: 5 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackPzCut.h"

#include "Track.h"

namespace Hal{

TrackPzCut::TrackPzCut() : TrackCut(1) { SetUnitName("p_{z} [GeV/c]"); }

Bool_t TrackPzCut::Pass(Track* track) {
  SetValue(track->GetPz());
  return Validate();
}

TrackPzCut::~TrackPzCut() {
  // TODO Auto-generated destructor stub
}
}
