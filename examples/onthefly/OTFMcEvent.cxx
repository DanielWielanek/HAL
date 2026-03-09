/*
 * HalOnTheFlyMcEvent.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "OTFMcEvent.h"
#include "OTFData.h"
#include "OTFMcEventInterface.h"

#include "DataManager.h"

#include <RtypesCore.h>
#include <TClonesArray.h>
#include <TObjArray.h>
#include <TObject.h>
#include <iostream>

namespace HalOTF {
  McEvent::McEvent() : Hal::McEvent("HalOTF::McTrack") {}

  Hal::EventInterface* McEvent::CreateInterface() const { return new McEventInterface(); }

  void McEvent::Update(Hal::EventInterface* interface) {
    Clear();
    McEventInterface* s = (McEventInterface*) interface;
    fTotalTracksNo      = s->GetMcEvent()->GetNTracks();
    fMultiplicity       = fTotalTracksNo;
    fTracks->ExpandCreateFast(fTotalTracksNo);
    OTF::McEvent* ev = s->GetMcEvent();
    fB               = ev->GetB();
    fPhi             = ev->GetPhi();
    for (int i = 0; i < fTotalTracksNo; i++) {
      McTrack* track   = (McTrack*) fTracks->UncheckedAt(i);
      OTF::McTrack* mc = (OTF::McTrack*) ev->GetTrack(i);
      track->ResetTrack(i, this);
      *track = *mc;
    }
  }

  Bool_t McEvent::ExistInTree() const {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    if (manager->CheckBranch("OTF::McEvent.")) { return kTRUE; }
    return kFALSE;
  }

  McEvent::~McEvent() {}

  McTrack& McTrack::operator=(const OTF::McTrack& other) {
    SetMomentum(other.GetMomentum());
    SetFreezeoutPosition(other.GetFreezeout().X(), other.GetFreezeout().Y(), other.GetFreezeout().Z(), other.GetFreezeout().T());
    SetPdg(other.GetPdgCode());
    SetMotherIndex(other.GetMotherId());
    SetGeneratorId(other.GetGeneratorId());
    return *this;
  }
}  // namespace HalOTF
