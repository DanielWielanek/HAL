/*
 * TrackMcParentCut.cxx
 *
 *  Created on: 15 gru 2025
 *      Author: daniel
 */

#include "TrackMcParentCut.h"

#include <RtypesCore.h>

#include "Const.h"
#include "Cut.h"
#include "Event.h"
#include "McEvent.h"
#include "McTrack.h"
#include "Track.h"

#include <iostream>

namespace Hal {

  TrackMcParentCut::TrackMcParentCut(Int_t size) : TrackMcCut(size) {}

  Bool_t TrackMcParentCut::Pass(Track* track) {
    auto mcTrack = (Hal::McTrack*) track;
    if (mcTrack->GetMotherIndex() < 0) {
      for (int i = 0; i < GetCutSize(); i++)
        SetValue(Hal::Const::DummyVal(), i);
      return ForcedUpdate(kFALSE);
    }
    return InnerPass(mcTrack);
  }

  Bool_t TrackMcParentPdgCut::InnerPass(McTrack* track) {
    auto event  = (Hal::McEvent*) track->GetEvent();
    auto mother = (Hal::McTrack*) event->GetTrack(track->GetMotherIndex());
    SetValue(mother->GetPdg());
    return Validate();
  }

  TrackMcParentPdgCut::TrackMcParentPdgCut() : TrackMcParentCut(1) { SetUnitName("Mother PDG [AU]", 0); }
}  // namespace Hal
