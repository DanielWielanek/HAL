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
#include "DataManager.h"
#include "Event.h"
#include "MemoryMapManager.h"


namespace Hal {
  MiniEventTask::MiniEventTask() {}

  void MiniEventTask::Exec(Option_t* /*opt*/) {
    fCurrentEvent = fMemoryMap->GetTemporaryEvent();
    fMap.Reset(fCurrentEvent->GetTotalTrackNo());
    for (fCurrentEventCollectionID = 0; fCurrentEventCollectionID < fEventCollectionsNo; fCurrentEventCollectionID++) {
      if (fCutContainer->PassEvent(fCurrentEvent, fCurrentEventCollectionID)) {
        CutCollection* cont = fCutContainer->GetEventCollection(fCurrentEventCollectionID);
        fMemoryMap->ResetTrackMaps(fCurrentEventCollectionID,
                                   0);  // counter always = 0 due to no buffering
        for (int i = 0; i < fMemoryMap->GetTemporaryTotalTracksNo(); i++) {
          for (int j = 0; j < cont->GetNextNo(); j++) {
            fCurrentTrackCollectionID = cont->GetNextAddr(j);
            fCurrentTrack             = fCurrentEvent->GetTrack(i);
            if (fCutContainer->PassTrack(fCurrentTrack, fCurrentTrackCollectionID)) { fMap.MarkAsGood(i); }
          }
        }
      }
    }
    fMap.Recalculate();
    fCurrentEvent->Compress(fMap);
  }

  Task::EInitFlag MiniEventTask::Init() {
    Task::EInitFlag init = TrackAna::Init();
    fCurrentEvent->Register(kFALSE);
    return init;
  }

  MiniEventTask::~MiniEventTask() {}
}  // namespace Hal
