/*
 * DbgRecoEventInterface.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "DbgRecoEventInterface.h"

#include "DataManager.h"

#include <RtypesCore.h>

namespace HalDbg {
  RecoEventInterface::RecoEventInterface() : fEvent(nullptr), fCanDelete(kFALSE) {}

  void RecoEventInterface::ConnectToTreeInternal(eMode mode) {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    switch (mode) {
      case Hal::EventInterface::eMode::kRead: {
        fEvent = (Dbg::RecoEvent*) manager->GetObject("Dbg::RecoEvent.");
      } break;
      case Hal::EventInterface::eMode::kWrite: {
        fEvent = new Dbg::RecoEvent();
        manager->Register("Dbg::RecoEvent.", "Dbg", fEvent, kTRUE);
      } break;
      case Hal::EventInterface::eMode::kWriteVirtual: {
        fEvent = new Dbg::RecoEvent();
        manager->Register("Dbg::RecoEvent.", "Dbg", fEvent, kFALSE);
        fCanDelete = kTRUE;
      } break;
    }
  }

  RecoEventInterface::~RecoEventInterface() {
    if (fCanDelete && fEvent) delete fEvent;
  }
}  // namespace HalDbg
