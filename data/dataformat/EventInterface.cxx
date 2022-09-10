/*
 * EventInterface.cxx
 *
 *  Created on: 4 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventInterface.h"

#include <iostream>
namespace Hal {
  EventInterface::EventInterface() : fConnected(kFALSE) {}

  void EventInterface::ConnectToTree(eMode mode) {
    if (fConnected) return;
    ConnectToTreeInternal(mode);
  }

  EventInterface::~EventInterface() {}

}  // namespace Hal
