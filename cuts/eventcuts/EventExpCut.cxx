/*
 * EventExpCut.cxx
 *
 *  Created on: 30 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventExpCut.h"

namespace Hal {

  EventExpCut::EventExpCut(const Int_t size) : EventCut(size) {}

  Bool_t EventExpCut::Init(Int_t format_id) {
    if (FormatInhertis("Hal::ExpEvent", format_id, EFormatDepth::kNonBuffered)) return kTRUE;
    return kFALSE;
  }

} /* namespace Hal */
