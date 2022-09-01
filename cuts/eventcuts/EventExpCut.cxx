/*
 * EventExpCut.cxx
 *
 *  Created on: 30 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventExpCut.h"

#include "DataFormatManager.h"
#include "ExpEvent.h"

namespace Hal {

  EventExpCut::EventExpCut(const Int_t size) : EventCut(size) {}

  Bool_t EventExpCut::Init(Int_t format_id) {
    if (EventCut::Init() == kFALSE) return kFALSE;
    const Event* ev = DataFormatManager::Instance()->GetFormat(format_id, EFormatDepth::kNonBuffered);
    if (dynamic_cast<const Hal::ExpEvent*>(ev)) return kTRUE;
    return kFALSE;
  }

} /* namespace Hal */
