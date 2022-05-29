/*
 * OTFMcEventInterface.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "OTFMcEventInterface.h"

#include "DataManager.h"

#include <RtypesCore.h>
#include <TCollection.h>
#include <TNamed.h>
#include <iostream>

namespace HalOTF {
  McEventInterface::McEventInterface() : fEvent(nullptr) {}

  void McEventInterface::ConnectToTree() {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    fEvent                    = (OTF::McEvent*) manager->GetObject("OTF::McEvent.");
    std::cout << "CONNECT " << fEvent << std::endl;
  }

  void McEventInterface::Register(Bool_t write) {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    manager->Register("OTF::McEvent.", "McEvent", fEvent, write);
  }

  McEventInterface::~McEventInterface() {
    // TODO Auto-generated destructor stub
  }
}  // namespace HalOTF
