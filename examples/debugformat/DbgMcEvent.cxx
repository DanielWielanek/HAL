/*
 * HalOnTheFlyMcEvent.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "DbgMcEvent.h"
#include "DbgData.h"
#include "DbgMcEventInterface.h"

#include "DataManager.h"

#include <RtypesCore.h>
#include <TClonesArray.h>
#include <TObjArray.h>
#include <TObject.h>
#include <iostream>

namespace HalDbg {
  McEvent::McEvent() : Hal::McEvent("HalDbg::McTrack") {}

  Hal::EventInterface* McEvent::CreateInterface() const { return new McEventInterface(); }

  void McEvent::Update(Hal::EventInterface* interface) {
    McEventInterface* s = (McEventInterface*) interface;
    fTotalTracksNo      = s->GetMcEvent()->GetNTracks();
    fEventId            = s->GetMcEvent()->GetEventId();
    fTracks->Clear();
    fTracks->ExpandCreateFast(fTotalTracksNo);
    Dbg::McEvent* ev = s->GetMcEvent();
    fB               = ev->GetB();
    for (int i = 0; i < fTotalTracksNo; i++) {
      McTrack* track   = (McTrack*) fTracks->UncheckedAt(i);
      Dbg::McTrack* mc = (Dbg::McTrack*) ev->GetTrack(i);
      track->ResetTrack(i, this);
      track->SetMomentum(mc->GetMomentum().Px(), mc->GetMomentum().Py(), mc->GetMomentum().Pz(), mc->GetMomentum().E());
      track->SetFreezeoutPosition(mc->GetFreezeout().X(), mc->GetFreezeout().Y(), mc->GetFreezeout().Z(), mc->GetFreezeout().T());
      track->SetMotherIndex(-1);
      track->SetPdg(mc->GetPdgCode());
    }
  }

  Bool_t McEvent::ExistInTree() const {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    if (manager->CheckBranch("Dbg::McEvent.")) { return kTRUE; }
    return kFALSE;
  }

  McEvent::~McEvent() {}
}  // namespace HalDbg
