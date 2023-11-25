/*
 * DbgMcEventInterface.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "DbgMcEventInterface.h"

#include "DataManager.h"

#include <RtypesCore.h>
#include <TCollection.h>
#include <TNamed.h>
#include <iostream>

namespace HalDbg {
  McEventInterface::McEventInterface() : fEvent(nullptr), fCanDelete(kFALSE) {}

  void McEventInterface::ConnectToTreeInternal(Hal::EventInterface::eMode mode) {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    switch (mode) {
      case Hal::EventInterface::eMode::kRead: {
        fEvent = (Dbg::McEvent*) manager->GetObject("Dbg::McEvent.");
      } break;
      case Hal::EventInterface::eMode::kWrite: {
        fEvent = new Dbg::McEvent();
        manager->Register("Dbg::McEvent.", "Dbg", fEvent, kTRUE);
      } break;
      case Hal::EventInterface::eMode::kWriteVirtual: {
        fEvent = new Dbg::McEvent();
        manager->Register("Dbg::McEvent.", "Dbg", fEvent, kFALSE);
        fCanDelete = kTRUE;
      } break;
    }
  }

  McEventInterface::~McEventInterface() {
    if (fCanDelete && fEvent) delete fEvent;
  }

}  // namespace HalDbg
