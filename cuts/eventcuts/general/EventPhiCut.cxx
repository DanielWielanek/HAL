/*
 * EventPhiCut.cxx
 *
 *  Created on: 18-03-2014
 *      Author: wielanek
 */

#include "EventPhiCut.h"

#include "Event.h"

namespace Hal {
  EventPhiCut::EventPhiCut() : EventCut(1) { SetUnitName("Event #phi [radians]"); }

  Bool_t EventPhiCut::Pass(Event* event) {
    SetValue(event->GetPhi());
    return Validate();
  }

  EventPhiCut::~EventPhiCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
