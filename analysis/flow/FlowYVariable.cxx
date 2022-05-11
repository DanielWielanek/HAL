/*
 * FlowYVariable.cxx
 *
 *  Created on: 05-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FlowYVariable.h"

#include "Track.h"

#include <TMath.h>

namespace Hal {
  FlowYVariable::FlowYVariable() : FlowVariable("y", "y") {}

  Double_t FlowYVariable::GetVariable(Track* p) {
    Double_t E  = p->GetE();
    Double_t pz = p->GetPz();
    return 0.5 * TMath::Log((E + pz) / (E - pz));
  }

  FlowYVariable::~FlowYVariable() {}
}  // namespace Hal
