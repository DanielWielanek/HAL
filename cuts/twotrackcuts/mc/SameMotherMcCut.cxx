/*
 * SameMotherCut.cxx
 *
 *  Created on: 2 kwi 2024
 *      Author: daniel
 */

#include "SameMotherMcCut.h"

#include "DataFormatManager.h"
#include "Event.h"
#include "McTrack.h"
#include "TwoTrack.h"

namespace Hal {

  SameMotherMcCut::SameMotherMcCut() : TwoTrackCut(1) {
    SetUnitName("SameMother", 0);
    SetMinAndMax(1, 0);
  }

  Bool_t SameMotherMcCut::Pass(TwoTrack* pair) {
    auto tr1 = (Hal::McTrack*) pair->GetTrack1();
    auto tr2 = (Hal::McTrack*) pair->GetTrack2();
    if (tr1->GetMotherIndex() < 0) { return ForcedUpdate(0); }
    if (tr2->GetMotherIndex() < 0) { return ForcedUpdate(0); }
    if (tr1->GetMotherIndex() == tr2->GetMotherIndex()) return ForcedUpdate(1);
    return ForcedUpdate(0);
  }

  Bool_t SameMotherMcCut::Init(Int_t task_id) {
    if (TwoTrackCut::Init() == kFALSE) return kFALSE;
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kBuffered);
    if (ev->InheritsFrom("Hal::McEvent")) {
      return kTRUE;
    } else {
      return kFALSE;
    }
  }

} /* namespace Hal */
