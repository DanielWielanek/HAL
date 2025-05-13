/*
 * EventRotate.cxx
 *
 *  Created on: 18 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventRotateTask.h"

#include "Cout.h"
#include "CutContainer.h"
#include "Event.h"
#include "EventVirtualCut.h"
#include "Package.h"
#include "Parameter.h"

#include <TRandom.h>
namespace Hal {
  EventRotateTask::EventRotateTask() : fPhiMin(-TMath::Pi()), fPhiMax(TMath::Pi()) {}

  EventRotateTask& EventRotateTask::operator=(const EventRotateTask& other) {
    if (this != &other) {
      EventAna::operator=(other);
      fPhiMin = other.fPhiMin;
      fPhiMax = other.fPhiMax;
    }
    return *this;
  }

  EventRotateTask::~EventRotateTask() {}

  EventRotateTask::EventRotateTask(const EventRotateTask& other) : EventAna(other) {
    fPhiMin = other.fPhiMin;
    fPhiMax = other.fPhiMax;
  }

  void Hal::EventRotateTask::ProcessEvent() { fCurrentEvent->RotateZ(gRandom->Uniform(fPhiMin, fPhiMax)); }

  Hal::Package* EventRotateTask::Report() const {
    auto report = EventAna::Report();
    report->AddObject(new Hal::ParameterDouble("PhiMin", fPhiMin));
    report->AddObject(new Hal::ParameterDouble("PhiMax", fPhiMax));
    return report;
  }

  void EventRotateTask::CheckCutContainerCollections() {
    EventAna::CheckCutContainerCollections();
    int no = fCutContainer->GetEventCollectionsNo();
    if (no > 1) { Cout::PrintInfo("EventRotateTask - more than one event collection!", EInfo::kWarning); }
    int cutNo = fCutContainer->GetEventCollection(0)->GetCutNo();
    if (cutNo > 1) { Cout::PrintInfo("EventRotateTask - more than one event cut!", EInfo::kWarning); }
    auto evcut = dynamic_cast<Hal::EventVirtualCut*>(fCutContainer->GetEventCollection(0)->GetCut(0));
    if (evcut == nullptr) { Cout::PrintInfo("EventRotateTask - no virtual event cut", EInfo::kWarning); }
  }

}  // namespace Hal
