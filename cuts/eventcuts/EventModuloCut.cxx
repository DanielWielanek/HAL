/*
 * EventModuloCut.cxx
 *
 *  Created on: 8 lut 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventModuloCut.h"

#include "Event.h"
#include "Package.h"
#include "Parameter.h"

namespace Hal {
  EventModuloCut::EventModuloCut(Int_t modulo) : EventCut(1), fCount(-1), fModulo(modulo) { SetUnitName("Modulo"); }

  Bool_t EventModuloCut::Pass(Event* /*event*/) {
    fCount++;
    Int_t modulo = fCount % fModulo;
    SetValue(modulo);
    return Validate();
  }

  EventModuloCut::~EventModuloCut() {}

  Package* EventModuloCut::Report() const {
    Package* pack = EventCut::Report();
    pack->AddObject(new ParameterInt("Modulo", fModulo));
    return pack;
  }
}  // namespace Hal
