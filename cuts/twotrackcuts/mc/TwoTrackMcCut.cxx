/*
 * TwoTrackMcCut.cxx
 *
 *  Created on: 27 mar 2026
 *      Author: daniel
 */

#include "TwoTrackMcCut.h"

#include "Cut.h"
#include "DataFormatManager.h"
#include "McEvent.h"
#include "StdTypes.h"

namespace Hal {

  TwoTrackMcCut::TwoTrackMcCut(Int_t size) : TwoTrackCut(size) {}

  Bool_t TwoTrackMcCut::Init(Int_t task_id) {
    if (TwoTrackCut::Init() == kFALSE) return kFALSE;
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kBuffered);
    if (dynamic_cast<const Hal::McEvent*>(ev)) return kTRUE;
    return kFALSE;
  }

} /* namespace Hal */
