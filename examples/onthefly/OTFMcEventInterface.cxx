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
  McEventInterface::McEventInterface() : fEvent(nullptr), fCanDelete(kFALSE) {}

  void McEventInterface::ConnectToTreeInternal(Hal::EventInterface::eMode mode) {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    switch (mode) {
      case Hal::EventInterface::eMode::kRead: {
        fEvent = (OTF::McEvent*) manager->GetObject("OTF::McEvent.");
      } break;
      case Hal::EventInterface::eMode::kWrite: {
        fEvent = new OTF::McEvent();
        manager->Register("OTF::McEvent.", "OTF", fEvent, kTRUE);
      } break;
      case Hal::EventInterface::eMode::kWriteVirtual: {
        fEvent = new OTF::McEvent();
        manager->Register("OTF::McEvent.", "OTF", fEvent, kFALSE);
        fCanDelete = kTRUE;
      } break;
    }
  }

  McEventInterface::~McEventInterface() {
    if (fCanDelete && fEvent) delete fEvent;
  }

}  // namespace HalOTF
