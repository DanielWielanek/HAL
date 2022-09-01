/*
 * EventMcCut.cxx
 *
 *  Created on: 30 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "EventMcCut.h"

#include "Cut.h"
#include "DataFormatManager.h"
#include "McEvent.h"
#include "Std.h"

#include <RtypesCore.h>

namespace Hal {

  EventMcCut::EventMcCut(const Int_t size) : EventCut(size) {}

  Bool_t EventMcCut::Init(Int_t format_id) {
    if (EventCut::Init() == kFALSE) return kFALSE;
    const Event* ev = DataFormatManager::Instance()->GetFormat(format_id, EFormatDepth::kNonBuffered);
    if (dynamic_cast<Hal::McEvent*>(ev)) return kTRUE;
    return kFALSE;
  }
} /* namespace Hal */
