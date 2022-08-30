/*
 * EventMcCut.cxx
 *
 *  Created on: 30 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventMcCut.h"

namespace Hal {

  EventMcCut::EventMcCut(const Int_t size) : EventCut(size) {}

  Bool_t EventMcCut::Init(Int_t format_id) {
    if (FormatInhertis("Hal::McEvent", format_id, EFormatDepth::kNonBuffered)) { return kTRUE; }
    return kFALSE;
  }
} /* namespace Hal */
