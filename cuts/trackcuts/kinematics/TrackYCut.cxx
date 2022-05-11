/*
 * TrackYCut.cpp
 *
 *  Created on: 03-11-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackYCut.h"

#include "Track.h"

namespace Hal {

  TrackYCut::TrackYCut() : TrackCut(1) { SetUnitName("y"); }

  Bool_t TrackYCut::Pass(Track* track) {
    Double_t pz = track->GetPz();
    Double_t e  = track->GetE();
    Double_t y  = 0.5 * TMath::Log((e + pz) / (e - pz));
    SetValue(y);
    return Validate();
  }

  TrackYCut::~TrackYCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
