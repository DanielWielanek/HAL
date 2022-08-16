/*
 * SplittingToStatusTask.cxx
 *
 *  Created on: 28 kwi 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "SplittingToStatusTask.h"

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "MemoryMapManager.h"
#include "Std.h"
#include "Track.h"

#include <RtypesCore.h>

namespace Hal {
  SplittingToStatusTask::SplittingToStatusTask() { AddTags("splitting task"); }

  SplittingToStatusTask::~SplittingToStatusTask() {}

  Task::EInitFlag SplittingToStatusTask::CheckFormat() {
    DataFormatManager* formatManager = DataFormatManager::Instance();
    const Event* ev                  = formatManager->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered);
    if (ev == nullptr) return Task::EInitFlag::kFATAL;
    if (dynamic_cast<const ComplexEvent*>(ev) == nullptr) { return Task::EInitFlag::kFATAL; }
    return EventAna::CheckFormat();
  }

  Task::EInitFlag SplittingToStatusTask::Init() {
    Task::EInitFlag init = EventAna::Init();
    fCurrentEvent->Register(kFALSE);
    return init;
  }

  void SplittingToStatusTask::Exec(Option_t* opt) {
    fProcessedEvents++;
    fCurrentEvent    = fMemoryMap->GetTemporaryEvent();
    ComplexEvent* ev = static_cast<ComplexEvent*>(fCurrentEvent);
    Event* im        = ev->GetImgEvent();
    //    Event* rec       = ev->GetRealEvent();
    int size = fSplitFlag.size();
    if (size < im->GetTotalTrackNo()) fSplitFlag.resize(im->GetTotalTrackNo());

    for (int i = 0; i < im->GetTotalTrackNo(); i++)
      fSplitFlag[i] = 0;
    for (int i = 0; i < ev->GetTotalTrackNo(); i++) {
      ComplexTrack* tr = (ComplexTrack*) ev->GetTrack(i);
      if (tr->GetMatchID() > -1) { ++fSplitFlag[tr->GetMatchID()]; }
    }
    for (int i = 0; i < ev->GetTotalTrackNo(); i++) {
      ComplexTrack* z = (ComplexTrack*) ev->GetTrack(i);
      Int_t match     = z->GetMatchID();
      if (match > -1) {
        if (fSplitFlag[match] > 1)
          z->SetStatus(1);
        else
          z->SetStatus(0);
      }
    }
  }

  SplittingToStatusTask::SplittingToStatusTask(const SplittingToStatusTask& other) :
    EventAna(other), fSplitFlag(other.fSplitFlag) {}

  SplittingToStatusTask& SplittingToStatusTask::operator=(const SplittingToStatusTask& other) {
    if (this == &other) return *this;
    EventAna::operator=(other);
    fSplitFlag        = other.fSplitFlag;
    return *this;
  }
}  // namespace Hal
