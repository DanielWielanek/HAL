/*
 * TrackThetaCut.cxx
 *
 *  Created on: 10 cze 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackThetaCut.h"

#include "Track.h"

#include <TLorentzVector.h>


namespace Hal {

  TrackThetaCut::TrackThetaCut() : TrackCut(1) { SetUnitName("#theta [rad]"); }

  Bool_t TrackThetaCut::Pass(Track* track) {
    Double_t theta = track->GetMomentum().Theta();
    SetValue(theta);
    return Validate();
  }

  TrackThetaCut::~TrackThetaCut() {}

}  // namespace Hal
