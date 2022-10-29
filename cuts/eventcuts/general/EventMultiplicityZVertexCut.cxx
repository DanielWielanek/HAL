/*
 * EventMultiplicityVertexCut.cxx
 *
 *  Created on: 29 paź 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventMultiplicityZVertexCut.h"

#include "Event.h"

namespace Hal {

  EventMultiplicityZVertexCut::EventMultiplicityZVertexCut() : EventCut(2) {
    SetUnitName("Multiplicity [N]", 0);
    SetUnitName("V_{z} [cm]", 1);
  }

  Bool_t EventMultiplicityZVertexCut::Pass(Event* event) {
    SetValue(event->GetMutliplicity(), 0);
    SetValue(event->GetVertex()->Z(), 1);
    return Validate();
  }

  EventMultiplicityZVertexCut::~EventMultiplicityZVertexCut() {}

} /* namespace Hal */
