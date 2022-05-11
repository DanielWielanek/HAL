/*
 * SmothAlgorithm.cxx
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "SmearAlgorithm.h"

#include "Parameter.h"
#include "Package.h"

namespace Hal {

  Package* SmearAlgorithm::Report() const {
    Package* pack = new Package(this);
    pack->AddObject(new ParameterString("Smear code", this->ClassName()));
    return pack;
  }
  EventSmear::EventSmear() {}

  EventSmear::~EventSmear() {}

  TrackSmear::TrackSmear() {}

  TrackSmear::~TrackSmear() {}

  EventSmearVirtual::EventSmearVirtual() : EventSmear() {}

  void EventSmearVirtual::ModifyEvent(SmearedEvent* /*mod*/) {}

  SmearAlgorithm* EventSmearVirtual::MakeCopy() const { return new EventSmearVirtual(*this); }

  EventSmearVirtual::~EventSmearVirtual() {}

  TrackSmearVirtual::TrackSmearVirtual() {}

  void TrackSmearVirtual::ModifyTrack(SmearedTrack* /*mod*/) {}

  SmearAlgorithm* TrackSmearVirtual::MakeCopy() const { return new TrackSmearVirtual(*this); }

  TrackSmearVirtual::~TrackSmearVirtual() {}

  Package* EventSmear::Report() const {
    Package* pack = SmearAlgorithm::Report();
    pack->AddObject(new ParameterString("Smearing type", "Event"));
    return pack;
  }

  Package* EventSmearVirtual::Report() const {
    Package* pack = SmearAlgorithm::Report();
    pack->AddObject(new ParameterString("Smearing type", "Track"));
    return pack;
  }
}  // namespace Hal
