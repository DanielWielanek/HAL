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
  EventInterface::EventInterface() {
    // TODO Auto-generated constructor stub
  }

  EventInterface::~EventInterface() {
    // TODO Auto-generated destructor stub
  }

  void EventInterface::LinkWithTree(eMode mode) {
    if (mode == eMode::kRead) {
      ConnectToTree();
    } else {
      std::cout << "Cannot use read/write virtual for pure EventInterface" << std::endl;
    }
  }
}  // namespace Hal
