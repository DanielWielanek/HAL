/*
 * BoostTask.cxx
 *
 *  Created on: 23 lip 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "BoostTask.h"

#include "Cout.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "EventInterface.h"
#include "EventInterfaceAdvanced.h"
#include "MemoryMapManager.h"
#include "Package.h"
#include "Parameter.h"
#include "TrackInterface.h"


namespace Hal {
  BoostTask::BoostTask() : BoostTask(0, 0, 0) {}

  BoostTask::BoostTask(Double_t vx, Double_t vy, Double_t vz) :
    EventAna(), fBoostVx(vx), fBoostVy(vy), fBoostVz(vz), fEventInterface(nullptr), fTrackInterface(nullptr) {}

  BoostTask::BoostTask(Double_t vz) : BoostTask(0, 0, vz) {}

  Task::EInitFlag BoostTask::Init() {
    Task::EInitFlag stat = EventAna::Init();
    return stat;
  }

  void BoostTask::Exec(Option_t* /*opt*/) { fCurrentEvent->Boost(fBoostVx, fBoostVy, fBoostVz); }

  void BoostTask::SetBoost(Double_t vx, Double_t vy, Double_t vz, Option_t* /*opt*/) {
    fBoostVx = vx;
    fBoostVy = vy;
    fBoostVz = vz;
  }

  void BoostTask::SetBoost(Double_t vz, Option_t* /*opt*/) { fBoostVz = vz; }

  Package* BoostTask::Report() const {
    Package* pack = EventAna::Report();
    pack->RemoveObjectByName("CutContainer");
    AddToAnaMetadata(pack, new ParameterString("Boost option", "velocity"));
    AddToAnaMetadata(pack, new ParameterDouble("Vx", fBoostVx));
    AddToAnaMetadata(pack, new ParameterDouble("Vy", fBoostVy));
    AddToAnaMetadata(pack, new ParameterDouble("Vz", fBoostVz));
    return pack;
  }

  BoostTask::~BoostTask() {
    if (fTrackInterface) delete fTrackInterface;
    if (fEventInterface) delete fEventInterface;
  }
}  // namespace Hal
