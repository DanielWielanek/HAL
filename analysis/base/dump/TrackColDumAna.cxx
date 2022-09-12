/*
 * TrackColDumAna.cxx
 *
 *  Created on: 12 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackColDumAna.h"

#include "CutCollection.h"
#include "CutContainer.h"
#include "DataManager.h"
#include "Event.h"
#include "MemoryMapManager.h"

#include <RtypesCore.h>

namespace Hal {

  TrackColDumAna::TrackColDumAna() {
    // TODO Auto-generated constructor stub
  }

  TrackColDumAna::~TrackColDumAna() {}

  TrackColDumAna::TrackColDumAna(const TrackColDumAna& other) : TrackAna(other) {}

  TrackColDumAna& TrackColDumAna::operator=(const TrackColDumAna& other) {
    if (this == &other) return *this;

    return *this;
  }

  Task::EInitFlag TrackColDumAna::Init() {
    Task::EInitFlag flag = TrackAna::Init();
    Int_t event_col      = fCutContainer->GetEventCollectionsNo();
    if (event_col != 0) return Task::EInitFlag::kFATAL;
    // fSelectionMap.resize(fCutContainer->GetTrackCollectionsNo());
    // fBufferSize.resize(fCutContainer->GetTrackCollectionsNo());
    fNewEvent              = fCurrentEvent->GetNewEvent();
    DataManager* ioManager = DataManager::Instance();
    ioManager->Register(Form("%s.", fNewEvent->ClassName()), "HalEvents", fNewEvent, kTRUE);
    return flag;
  }

  void TrackColDumAna::ProcessEvent() {
    fMemoryMap->PrepareMaps(fCurrentEventCollectionID);
    CutCollection* cont = fCutContainer->GetEventCollection(fCurrentEventCollectionID);
    for (fTrackIndex = 0; fTrackIndex < fMemoryMap->GetTemporaryTotalTracksNo(); fTrackIndex++) {
      fCurrentTrack = fCurrentEvent->GetTrack(fTrackIndex);
      for (int j = 0; j < cont->GetNextNo(); j++) {
        fCurrentTrackCollectionID = cont->GetNextAddr(j);
        if (fCutContainer->PassTrack(fCurrentTrack, fCurrentTrackCollectionID)) {
          fCurrentTrack->SetStatus(fCurrentTrack->GetStatus() + fCurrentTrackCollectionID * 1000);  // change track status !!
          fMemoryMap->AddTrackToMapTrack(fCurrentEventCollectionID,
                                         fCurrentTrackCollectionID,
                                         fTrackIndex);  // load track into memory map - may be usefull at
                                                        // finish event
          ProcessTrack();
        }
      }
    }
    fMemoryMap->BufferEvent(fCurrentEventCollectionID);
    // int tracks = fMemoryMap->GetTemporaryTotalTracksNo();
  }


} /* namespace Hal */
