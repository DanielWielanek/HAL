/*
 * MiniEventTask.cxx
 *
 *  Created on: 19 lut 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "MiniEventTask.h"

#include "CutCollection.h"
#include "CutContainer.h"
#include "Event.h"
#include "MemoryMapManager.h"


namespace Hal {
  MiniEventTask::MiniEventTask() : fArraySize(1000), fArray(NULL), fMap(NULL) {}

  void MiniEventTask::Exec(Option_t* /*opt*/) {
    fCurrentEvent = fMemoryMap->GetTemporaryEvent();
    if (fCurrentEvent->GetTotalTrackNo() > fArraySize) {
      delete[] fArray;
      delete[] fMap;
      fArraySize = fCurrentEvent->GetTotalTrackNo() * 2;
      fArray     = new Int_t[fArraySize];
      fMap       = new Int_t[fArraySize];
    }
    for (int i = 0; i < fArraySize; i++) {
      fArray[i] = 0;
    }
    Check();
    Sum();
  }

  Task::EInitFlag MiniEventTask::Init() {
    Task::EInitFlag init = TrackAna::Init();
    fArray               = new Int_t[fArraySize];
    fMap                 = new Int_t[fArraySize];
    return init;
  }

  void MiniEventTask::Check() {
    for (fCurrentEventCollectionID = 0; fCurrentEventCollectionID < fEventCollectionsNo; fCurrentEventCollectionID++) {
      if (fCutContainer->PassEvent(fCurrentEvent, fCurrentEventCollectionID)) {
        CutCollection* cont = fCutContainer->GetEventCollection(fCurrentEventCollectionID);
        fMemoryMap->ResetTrackMaps(fCurrentEventCollectionID,
                                   0);  // counter always = 0 due to no buffering
        for (int i = 0; i < fMemoryMap->GetTemporaryTotalTracksNo(); i++) {
          for (int j = 0; j < cont->GetNextNo(); j++) {
            fCurrentTrackCollectionID = cont->GetNextAddr(j);
            fCurrentTrack             = fCurrentEvent->GetTrack(i);
            if (fCutContainer->PassTrack(fCurrentTrack, fCurrentTrackCollectionID)) { fArray[i] = 1; }
          }
        }
      }
    }
  }

  void MiniEventTask::Sum() {
    Int_t maps_size = 0;
    for (int i = 0; i < fArraySize; i++) {
      if (fArray[i] == 1) { fMap[maps_size++] = i; }
    }
    fCurrentEvent->Compress(fMap, maps_size);
  }

  MiniEventTask::~MiniEventTask() {
    if (fArray) delete[] fArray;
    if (fMap) { delete[] fMap; }
  }
}  // namespace Hal
