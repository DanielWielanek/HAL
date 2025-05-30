/*
 * TrackAna.cxx
 *
 *  Created on: 07-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackAna.h"
#include "Cout.h"
#include "MemoryMapManager.h"

#include "CutCollection.h"
#include "CutContainer.h"
#include "Event.h"
#include "TrackVirtualCut.h"

namespace Hal {
  TrackAna::TrackAna(ECutUpdate tiers) :
    EventAna(tiers), fTrackCollectionsNo(0), fCurrentTrackCollectionID(0), fTrackIndex(0), fCurrentTrack(nullptr) {}

  TrackAna::TrackAna(const TrackAna& ana) :
    EventAna(ana),
    fTrackCollectionsNo(ana.fTrackCollectionsNo),
    fCurrentTrackCollectionID(ana.fCurrentTrackCollectionID),
    fTrackIndex(ana.fTrackIndex),
    fCurrentTrack(NULL) {}

  void TrackAna::ProcessEvent() {
    fMemoryMap->PrepareMaps(fCurrentEventCollectionID);
    CutCollection* cont = fCutContainer->GetEventCollection(fCurrentEventCollectionID);
    for (fTrackIndex = 0; fTrackIndex < fMemoryMap->GetTemporaryTotalTracksNo(); fTrackIndex++) {
      fCurrentTrack = fCurrentEvent->GetTrack(fTrackIndex);
      for (int j = 0; j < cont->GetNextNo(); j++) {
        fCurrentTrackCollectionID = cont->GetNextAddr(j);
        if (fCutContainer->PassTrack(fCurrentTrack, fCurrentTrackCollectionID)) {
          fMemoryMap->AddTrackToMapTrack(fCurrentEventCollectionID,
                                         fCurrentTrackCollectionID,
                                         fTrackIndex);  // load track into memory map - may be usefull at
                                                        // finish event
          ProcessTrack();
        }
      }
    }
  }

  Task::EInitFlag TrackAna::Init() {
    if (Task::EInitFlag::kSUCCESS == EventAna::Init()) {
      fTrackCollectionsNo = fCutContainer->GetTrackCollectionsNo();
      return Task::EInitFlag::kSUCCESS;
    } else
      return Task::EInitFlag::kFATAL;
  }

  void TrackAna::SetOption(Option_t* option) { EventAna::SetOption(option); }

  void TrackAna::ProcessTrack() {}

  void TrackAna::LinkCollections() {
    Int_t trackCollections = fCutContainer->GetTrackCollectionsNo();
    Int_t eventCollections = fCutContainer->GetEventCollectionsNo();
    Int_t jump             = trackCollections / eventCollections;
    if (jump == 0) {
      Cout::PrintInfo("Too many event collections, some of them will be ignored", EInfo::kLowWarning);
      jump++;
    }
    if (trackCollections != jump * eventCollections) {
      Cout::PrintInfo("Some track collections will be ignored", EInfo::kLowWarning);
    }
    for (int i = 0; i < eventCollections; i++) {
      for (int j = i * jump; j < (i + 1) * jump; j++) {
        fCutContainer->LinkCollections(ECutUpdate::kEvent, i, ECutUpdate::kTrack, j);
      }
    }
  }

  void TrackAna::CheckCutContainerCollections() {
    EventAna::CheckCutContainerCollections();
    Int_t trackCollections = fCutContainer->GetTrackCollectionsNo();
    if (trackCollections == 0) {
#ifdef HAL_DEBUG
      Cout::PrintInfo("No track collections, virtual will be added", EInfo::kDebugInfo);
#endif
      fCutContainer->AddCut(TrackVirtualCut(), "fast");
    }
    fTrackCollectionsNo = fCutContainer->GetTrackCollectionsNo();
  }

  void TrackAna::Exec(Option_t* opt) { EventAna::Exec(opt); }

  TrackAna::~TrackAna() {}

  Package* TrackAna::Report() const {
    Package* pack = EventAna::Report();
    return pack;
  }

  void TrackAna::FinishTask() { EventAna::FinishTask(); }

  TrackAna& TrackAna::operator=(const TrackAna& other) {
    if (this != &other) {
      EventAna::operator=(other);
      fCurrentTrack             = other.fCurrentTrack;
      fTrackIndex               = other.fTrackIndex;
      fCurrentTrackCollectionID = other.fCurrentEventCollectionID;
      fTrackCollectionsNo       = other.fTrackCollectionsNo;
    }
    return *this;
  }
}  // namespace Hal
