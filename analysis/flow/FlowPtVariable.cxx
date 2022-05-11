/*
 * FlowPtVariable.cxx
 *
 *  Created on: 19-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FlowPtVariable.h"

#include "Track.h"

namespace Hal {
  FlowPtVariable::FlowPtVariable() : FlowVariable("P_{t} [GeV/c]", "dP_{t}") {}

  Double_t FlowPtVariable::GetVariable(Track* p) {
    Double_t px = p->GetPx();
    Double_t py = p->GetPy();
    return TMath::Sqrt(px * px + py * py);
  }

  FlowPtVariable::~FlowPtVariable() {}
}  // namespace Hal
