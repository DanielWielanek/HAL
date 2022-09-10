/*
 * AdvancedEventInterface.cxx
 *
 *  Created on: 4 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventInterfaceAdvanced.h"

#include "TrackInterface.h"
#include <iostream>
namespace Hal {
  EventInterfaceAdvanced::EventInterfaceAdvanced() : fCanDeleteEvent(kFALSE), fTrInterface(nullptr) {}

  EventInterfaceAdvanced::EventInterfaceAdvanced(TrackInterface* tr) : fCanDeleteEvent(kFALSE), fTrInterface(tr) {}

  EventInterfaceAdvanced::~EventInterfaceAdvanced() {
    if (fTrInterface) delete fTrInterface;
  }

  void EventInterfaceAdvanced::Boost(Double_t /*vx*/, Double_t /*vy*/, Double_t /*vz*/) {
    std::cout << "BOOST not supported for this format" << std::endl;
  }

  void EventInterfaceAdvanced::ConnectToTreeInternal(EventInterface::eMode mode) {
    if (EventInterface::eMode::kWriteVirtual == mode) { fCanDeleteEvent = kTRUE; }
  }
}  // namespace Hal
