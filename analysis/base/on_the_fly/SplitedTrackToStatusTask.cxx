/*
 * SplitedTrackToStatusTask.cxx
 *
 *  Created on: 27 lis 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "SplitedTrackToStatusTask.h"

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "Cout.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "Link.h"
#include "MemoryMapManager.h"
#include "Track.h"

namespace Hal {
  SplitedTrackToStatusTask::SplitedTrackToStatusTask() {}

  SplitedTrackToStatusTask::~SplitedTrackToStatusTask() {}

  SplitedTrackToStatusTask::SplitedTrackToStatusTask(const SplitedTrackToStatusTask& other) : EventAna(other) {
    fArrayReco = other.fArrayReco;
    fArraySim  = other.fArraySim;
  }

  Task::EInitFlag SplitedTrackToStatusTask::Init() {
    Task::EInitFlag stat = EventAna::Init();
    if (stat == Task::EInitFlag::kFATAL) return stat;
    const Event* ev = DataFormatManager::Instance()->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered);
    if (ev->InheritsFrom("Hal::ComplexEvent")) {
      fCurrentEvent = fMemoryMap->GetTemporaryEvent();
      fCurrentEvent->Register(kFALSE);
      return Task::EInitFlag::kSUCCESS;
    }
    Cout::PrintInfo(Form("Wrong format in %s", this->ClassName()), EInfo::kError);
    return Task::EInitFlag::kFATAL;
  }

  void SplitedTrackToStatusTask::ProcessEvent() {
    ComplexEvent* complex = (ComplexEvent*) fCurrentEvent;
    Event* event_re       = complex->GetRealEvent();
    Event* event_im       = complex->GetImgEvent();
    const Int_t nTracksRe = event_re->GetTotalTrackNo();
    const Int_t nTracksIm = event_im->GetTotalTrackNo();
    fArrayReco.Resize(nTracksRe);
    fArraySim.Resize(nTracksIm);
    for (int i = 0; i < nTracksRe; i++) {
      fArrayReco[i] = -1;
    }
    for (int i = 0; i < nTracksIm; i++) {
      fArraySim[i] = -1;
    }
    // check status of MC-tracks
    for (int i = 0; i < nTracksRe; i++) {
      ComplexTrack* complex_track = (ComplexTrack*) complex->GetTrack(i);
      Track* mc_track             = complex_track->GetImgTrack();
      if (mc_track != nullptr) {
        Int_t mc_id = mc_track->GetThisID();
        if (fArraySim[mc_id] == -1) {
          fArraySim[mc_id] = 0;  // good reconstructed mc-track
        } else {
          fArraySim[mc_id] = 1;  // track reconstructed more than once
        }
      }
    }
    // fill status of MC-tracks
    for (int i = 0; i < nTracksIm; i++) {
      Track* mc_track = event_im->GetTrack(i);
      mc_track->SetStatus(fArraySim[i]);
    }
    // check/fill status of reco tracks
    for (int i = 0; i < nTracksRe; i++) {
      ComplexTrack* complex_track = (ComplexTrack*) complex->GetTrack(i);
      Track* mc_track             = complex_track->GetImgTrack();
      Track* reco_track           = complex_track->GetRealTrack();
      if (mc_track != nullptr) {
        mc_track->SetStatus(0);
        Int_t mc_id = mc_track->GetThisID();
        if (fArraySim[mc_id] == 0) {
          reco_track->SetStatus(0);  // good reconstructed mc-track
        } else {
          reco_track->SetStatus(1);  // track reconstructed more than once
        }
      } else {
        reco_track->SetStatus(-1);
      }
    }
  }

  SplitedTrackToStatusTask& SplitedTrackToStatusTask::operator=(const SplitedTrackToStatusTask& other) {
    if (this == &other) return *this;
    EventAna::operator=(other);
    fArrayReco        = other.fArrayReco;
    fArraySim         = other.fArraySim;
    return *this;
  }
}  // namespace Hal
