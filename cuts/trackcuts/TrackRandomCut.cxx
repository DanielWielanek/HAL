/*
 * TrackRandomCut.cxx
 *
 *  Created on: 26 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackRandomCut.h"

#include "Track.h"

#include <TRandom.h>

namespace Hal {
  TrackRandomCut::TrackRandomCut() : TrackCut(1) { SetUnitName("Rndm [Rand]", 0); }

  TrackRandomCut::~TrackRandomCut() {}

  Bool_t TrackRandomCut::Pass(Track* track) {
    Double_t z = gRandom->Rndm();
    SetValue(z);
    if (z < GetMax()) return kTRUE;
    return kFALSE;
  }
}  // namespace Hal
