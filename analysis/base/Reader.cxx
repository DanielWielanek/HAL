/*
 * Reader.cxx
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Reader.h"

#include "Cout.h"
#include "DataManager.h"
#include "Event.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"

#include <RtypesCore.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TString.h>


namespace Hal {

  Reader::Reader() : fProcessedEvents(0), fEvent(nullptr), fInterface(nullptr) {}

  Reader::~Reader() {
    if (fInterface) delete fInterface;
  }

  Task::EInitFlag Reader::Init() {
    if (fEvent == nullptr) {
      Cout::PrintInfo("Lack format in Reader!", EInfo::kError);
      return Task::EInitFlag::kFATAL;
    }
    fInterface = fEvent->CreateSource();
    fInterface->ConnectToTree(EventInterface::eMode::kRead);
    DataManager* manager = DataManager::Instance();
    manager->Register("HalEvent.", "HalEvents", fEvent, kFALSE);
    Cout::PrintInfo(Form("Register branch from reader called %s", fEvent->ClassName()), EInfo::kInfo);
    return Task::EInitFlag::kSUCCESS;
  }

  void Reader::Exec(Option_t* /*opt*/) {
    fProcessedEvents++;
    fEvent->Update(fInterface);
  }

  void Reader::SetFormat(Event* format) { fEvent = format; }

  void Reader::FinishTask() {}
} /* namespace Hal */
