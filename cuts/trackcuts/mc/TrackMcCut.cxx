/*
 * TrackMCCut.cxx
 *
 *  Created on: 5 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackMcCut.h"

#include "Cut.h"
#include "DataFormatManager.h"
#include "McEvent.h"
#include "Std.h"

#include <RtypesCore.h>

namespace Hal {

  TrackMcCut::TrackMcCut(const Int_t size) : TrackCut(size) {}

  Bool_t TrackMcCut::Init(Int_t task_id) {
    if (TrackCut::Init() == kFALSE) return kFALSE;
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (dynamic_cast<const Hal::McEvent*>(ev)) return kTRUE;
    return kFALSE;
  }

  TrackMcCut::~TrackMcCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
