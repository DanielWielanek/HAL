/*
 * EventPrimaryNoCut.cxx
 *
 *  Created on: 15 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventPrimaryTracksNoCut.h"

#include "Event.h"
#include "McTrack.h"

namespace Hal {
  EventPrimaryTracksNoCut::EventPrimaryTracksNoCut() : EventCut(1) { SetUnitName("N_{primaries}", 0); }

  Bool_t EventPrimaryTracksNoCut::Pass(Event* event) {
    Int_t multi = 0;
    for (int i = 0; i < event->GetTotalTrackNo(); i++) {
      Track* track = event->GetTrack(i);
      if (track->IsPrimary()) { multi++; }
    }
    SetValue(multi);
    return Validate();
  }

  EventPrimaryTracksNoCut::~EventPrimaryTracksNoCut() {}
}  // namespace Hal
