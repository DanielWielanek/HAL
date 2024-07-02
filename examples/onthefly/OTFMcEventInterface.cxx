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
  McEventInterface::McEventInterface() : fEventPointer(nullptr), fCanDelete(kFALSE) {}

  void McEventInterface::ConnectToTreeInternal(Hal::EventInterface::eMode mode) {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    switch (mode) {
      case Hal::EventInterface::eMode::kRead: {
        fEventPointer = (OTF::McEvent*) manager->GetDoublePointer("OTF::McEvent.");
      } break;
      case Hal::EventInterface::eMode::kWrite: {
        manager->Register("OTF::McEvent.", "OTF", new OTF::McEvent(), kTRUE);
        fEventPointer = manager->GetTObject("OTF::McEvent.");
      } break;
      case Hal::EventInterface::eMode::kWriteVirtual: {
        manager->Register("OTF::McEvent.", "OTF", new OTF::McEvent(), kFALSE);
        fEventPointer = manager->GetTObject("OTF::McEvent.");
        fCanDelete    = kTRUE;
      } break;
    }
  }

  McEventInterface::~McEventInterface() {
    if (fCanDelete && fEventPointer && fEventPointer->GetPointer()) fEventPointer->DeletePointer();
    if (fEventPointer) delete fEventPointer;
  }

}  // namespace HalOTF
