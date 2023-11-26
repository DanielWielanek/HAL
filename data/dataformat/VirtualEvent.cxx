/*
 * EventVirtual.cxx
 *
 *  Created on: 4 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "VirtualEvent.h"

#include "DataManager.h"

namespace Hal {

  VirtualEvent::VirtualEvent() : Event("VirtualTrack"), fVirtualEvent(NULL) {}

  void VirtualEvent::Update(Hal::EventInterface* /*interface*/) {
    if (fVirtualEvent) {
      ShallowCopyEvent(fVirtualEvent);
      fTotalTracksNo = fVirtualEvent->GetTotalTrackNo();
      fTracks->Clear();
      for (int i = 0; i < fTotalTracksNo; i++) {
        VirtualTrack* from = (VirtualTrack*) fVirtualEvent->GetTrack(i);
        VirtualTrack* to   = (VirtualTrack*) fTracks->ConstructedAt(i);
        to->CopyData(from);
      }
    }
  }

  void VirtualEvent::RegisterInTree(TString prefix, Bool_t save) {
    if (fVirtualEvent == NULL) { fVirtualEvent = new VirtualEvent(); }
    DataManager* manager = DataManager::Instance();
    TString branchname   = "VirtualEvent..";
    if (prefix.Length() != 0) { branchname = prefix + branchname; }
    manager->Register(branchname, "VirtualEvent.", fVirtualEvent, save);
  }

  Bool_t VirtualEvent::ExistInTree() const {
    DataManager* manager = DataManager::Instance();
    if (manager->CheckBranch("VirtualEvent.")) { return kTRUE; }
    return kFALSE;
  }

  VirtualEvent::VirtualEvent(const VirtualEvent& other) : Event(other) {
    fTracks       = new TClonesArray("Hal::VirtualTrack", 1000);
    fVirtualEvent = (VirtualEvent*) other.fVirtualEvent->Clone();
  }

  VirtualEvent::~VirtualEvent() {
    if (fVirtualEvent) delete fVirtualEvent;
  }
}  // namespace Hal
