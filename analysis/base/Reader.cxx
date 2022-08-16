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

  Reader::Reader() : fProcessedEvents(0), fEvent(nullptr) {}

  Reader::~Reader() {}

  Task::EInitFlag Reader::Init() {
    if (fEvent == nullptr) {
      Cout::PrintInfo("Lack format in Reader!", EInfo::kError);
      return Task::EInitFlag::kFATAL;
    }
    if (fEvent->GetSource() != nullptr) {
      Cout::PrintInfo("Source for reader created", EInfo::kError);
      return Task::EInitFlag::kFATAL;
    }
    fEvent->CreateSource();
    fEvent->LinkWithTree();
    DataManager* manager = DataManager::Instance();
    manager->Register(fEvent->ClassName(), "HalEvents", fEvent, kFALSE);
    Cout::PrintInfo(Form("Register branch from reader called %s", fEvent->ClassName()), EInfo::kInfo);
    return Task::EInitFlag::kSUCCESS;
  }

  void Reader::Exec(Option_t* /*opt*/) {
    fProcessedEvents++;
    fEvent->Update();
  }

  void Reader::SetFormat(Event* format) { fEvent = format; }

  void Reader::FinishTask() {}
} /* namespace Hal */
