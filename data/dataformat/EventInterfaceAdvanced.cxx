/*
 * AdvancedEventInterface.cxx
 *
 *  Created on: 4 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventInterfaceAdvanced.h"

#include <iostream>
namespace Hal {
  EventInterfaceAdvanced::EventInterfaceAdvanced() : fCanDeleteEvent(kTRUE) {}

  EventInterfaceAdvanced::~EventInterfaceAdvanced() {}

  void EventInterfaceAdvanced::Boost(Double_t /*vx*/, Double_t /*vy*/, Double_t /*vz*/) {
    std::cout << "BOOST not supported for this format" << std::endl;
  }

  void EventInterfaceAdvanced::LinkWithTree(EventInterface::eMode mode) {
    switch (mode) {
      case EventInterface::eMode::kRead:
        ConnectToTree();
        fCanDeleteEvent = kFALSE;
        break;
      case EventInterface::eMode::kWrite:
        Register(kTRUE);
        fCanDeleteEvent = kFALSE;
        break;
      case EventInterface::eMode::kWriteVirtual:
        Register(kFALSE);
        fCanDeleteEvent = kFALSE;
        break;
    }
  }
}  // namespace Hal
