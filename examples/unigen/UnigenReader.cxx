/*
 * UnigenReader.cxx
 *
 *  Created on: 18 kwi 2024
 *      Author: daniel
 */

#include "UnigenReader.h"

#include "DataManager.h"
#include "UnigenEvent.h"
#include "UnigenEventInterface.h"

namespace HalUni {

  UnigenReader::UnigenReader() {}

  void UnigenReader::Exec(Option_t* opt) { fUniEvent->Update(fInterface); }

  Hal::Task::EInitFlag UnigenReader::Init() {

    fUniEvent  = new HalUni::UnigenEvent();
    fInterface = (UnigenEventInterface*) fUniEvent->CreateInterface();
    fInterface->ConnectToTree(Hal::EventInterface::eMode::kRead);
    Hal::DataManager* mng = Hal::DataManager::Instance();
    mng->Register("HalEvent.", "HalEvents", fUniEvent, kFALSE);
    if (!fUniEvent->ExistInTree()) { return Hal::Task::EInitFlag::kFATAL; }

    return Hal::Task::EInitFlag::kSUCCESS;
  }

  UnigenReader::~UnigenReader() {
    if (fInterface) delete fInterface;
  }

} /* namespace HalUni */
