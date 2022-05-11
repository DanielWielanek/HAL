/*
 * TrackChargeCut.cxx
 *
 *  Created on: 21 maj 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackChargeCut.h"

#include "Track.h"

namespace Hal {

  TrackChargeCut::TrackChargeCut() : TrackCut(1) { SetUnitName("Charge [e]"); }

  Bool_t TrackChargeCut::Pass(Track* tr) {
    SetValue(tr->GetCharge());
    return Validate();
  }

  TrackChargeCut::~TrackChargeCut() {}
}  // namespace Hal
