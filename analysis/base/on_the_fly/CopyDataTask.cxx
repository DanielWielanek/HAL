/*
 * CopyDataTask.cxx
 *
 *  Created on: 06-11-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CopyDataTask.h"

#include "Cout.h"
#include "Event.h"
#include "Task.h"


namespace Hal {
  CopyDataTask::CopyDataTask() : fInputData(NULL) {}

  void CopyDataTask::Exec(Option_t* /*opt*/) {}

  Task::EInitFlag CopyDataTask::Init() {
    if (fInputData == nullptr) return Task::EInitFlag::kFATAL;
    Hal::EventInterface* interface = fInputData->CreateInterface();
    interface->ConnectToTree(Hal::EventInterface::eMode::kRead);
    interface->ConnectToTree(Hal::EventInterface::eMode::kWrite);
    return Task::EInitFlag::kSUCCESS;
  }

  void CopyDataTask::SetFormat(Event* event) {
    if (fInputData) {
      Cout::PrintInfo("Overwriting format in CopyDataTask", EInfo::kLowWarning);
      fInputData = event;
    }
  }

  CopyDataTask::~CopyDataTask() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
