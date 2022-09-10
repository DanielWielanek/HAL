/*
 * EventInterfaceTemplate.cxx
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventInterfaceTemplate.h"

#include <Hal/DataManager.h>

namespace MyHal {

  EventInterfaceTemplate::EventInterfaceTemplate() : fEvent(nullptr), fCanDelete(kFALSE) {}

  void EventInterfaceTemplate::ConnectToTreeInternal(eMode mode) {
    Hal::DataManager* manager = Hal::DataManager::Instance();

    switch (mode) {
      case Hal::EventInterface::eMode::kRead: {
        fEvent = (TObject*) manager->GetObject("ObjectName");
      } break;
      case Hal::EventInterface::eMode::kWrite: {
        fEvent = new TObject();
        manager->Register("ObjectName.", "obj", (TNamed*) fEvent, kTRUE);
      } break;
      case Hal::EventInterface::eMode::kWriteVirtual: {
        fEvent = new TObject();
        manager->Register("ObjectName.", "obj", (TNamed*) fEvent, kTRUE);
        fCanDelete = kTRUE;
      } break;
    }
  }

  EventInterfaceTemplate::~EventInterfaceTemplate() {
    if (fEvent && fCanDelete) delete fEvent;
  }

}  // namespace MyHal
