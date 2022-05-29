/*
 * OTFRecoEventInterface.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "OTFRecoEventInterface.h"

#include "DataManager.h"

#include <RtypesCore.h>

namespace HalOTF {
  RecoEventInterface::RecoEventInterface() : fEvent(nullptr) {}

  void RecoEventInterface::ConnectToTree() {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    fEvent                    = (OTF::RecoEvent*) manager->GetObject("OTF::RecoEvent.");
  }

  void RecoEventInterface::Register(Bool_t write) {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    manager->Register("OTF::RecoEvent.", "RecoEvent", fEvent, write);
  }

  RecoEventInterface::~RecoEventInterface() {}
}  // namespace HalOTF
