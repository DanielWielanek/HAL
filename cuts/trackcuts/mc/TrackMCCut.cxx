/*
 * TrackMCCut.cxx
 *
 *  Created on: 5 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackMCCut.h"

#include "Cut.h"
#include "DataFormatManager.h"
#include "McEvent.h"
#include "Std.h"

#include <RtypesCore.h>

namespace Hal {

  TrackMCCut::TrackMCCut(const Int_t size) : TrackCut(size) {}

  Bool_t TrackMCCut::Init(Int_t task_id) {
    if (TrackCut::Init() == kFALSE) return kFALSE;
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (dynamic_cast<const Hal::McEvent*>(ev)) return kTRUE;
    return kFALSE;
  }

  TrackMCCut::~TrackMCCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
