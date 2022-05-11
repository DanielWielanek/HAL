/*
 * TrackMassCut.cxx
 *
 *  Created on: 26-03-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackMassCut.h"

#include "Track.h"

namespace Hal {
  TrackMassCut::TrackMassCut() : TrackCut(1) { SetUnitName("m [GeV/c^{2}]"); }

  Bool_t TrackMassCut::Pass(Track* track) {
    Double_t mass = track->GetMass();
    SetValue(mass);
    return Validate();
  }

  TrackMassCut::~TrackMassCut() {}
}  // namespace Hal
