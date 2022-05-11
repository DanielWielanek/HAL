/*
 * EventCut.cpp
 *
 *  Created on: 20-06-2013
 *      Author: Daniel Wielanek
 */

#include "EventCut.h"

#include "HalStd.h"
namespace Hal {
  EventCut::EventCut(const Int_t i) : Cut(i, ECutUpdate::kEventUpdate, "N_{events}") { fName = "EventCut"; }

  EventCut::~EventCut() {
    // TODO Auto-generated destructor stub
  }

  Package* EventCut::Report() const {
    Package* pack = Cut::Report();
    return pack;
  }
}  // namespace Hal
