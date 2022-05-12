/*
 * MultiTrackAna.cxx
 *
 *  Created on: 07-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "MultiTrackAna.h"

#include "Cout.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "EventAna.h"
#include "HalStd.h"
#include "MemoryMapManager.h"
#include "Parameter.h"

#include <RtypesCore.h>
#include <TString.h>


namespace Hal {
  Task::EInitFlag MultiTrackAna::Init() {
    if (Task::EInitFlag::kSUCCESS == TrackAna::Init()) {
      if (fMixSize < 1) {
#ifdef HAL_DEBUG
        Cout::PrintInfo("To small mix size, increasing to 1", EInfo::kLessInfo);
#endif
        fMixSize = 1;
      }
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("Mix size is %i", fMixSize), EInfo::kLessInfo);
#endif

      return Task::EInitFlag::kSUCCESS;
    } else {
      return Task::EInitFlag::kFATAL;
    }
  }

  void MultiTrackAna::SetMixSize(Int_t to_mix) {
    if (to_mix < 1) {
      Cout::PrintInfo("Wrong mix size in SetMixSize", EInfo::kImportantWarning);
    } else {
      fMixSize = to_mix;
    }
  }

  void MultiTrackAna::SetOption(Option_t* option) { TrackAna::SetOption(option); }

  void MultiTrackAna::ProcessTrack() {}

  MultiTrackAna::~MultiTrackAna() {}

  MultiTrackAna::MultiTrackAna() { fTiers = ECutUpdate::kTwoTrackUpdate; }

  MultiTrackAna::MultiTrackAna(const MultiTrackAna& ana) : TrackAna(ana) {}

  Package* MultiTrackAna::Report() const {
    Package* pack = TrackAna::Report();
    AddToAnaMetadata(pack, new ParameterInt("MixSize", fMixSize));
    return pack;
  }

  void MultiTrackAna::ProcessEvent() {
    TrackAna::ProcessEvent();
    fMemoryMap->BufferEvent(fCurrentEventCollectionID);
  }

  void MultiTrackAna::FinishTask() { TrackAna::FinishTask(); }

  void MultiTrackAna::InitNewCutContainer() { TrackAna::InitNewCutContainer(); }

  MultiTrackAna& MultiTrackAna::operator=(const MultiTrackAna& other) {
    if (this != &other) { TrackAna::operator=(other); }
    return *this;
  }

  Task::EInitFlag MultiTrackAna::CheckFormat() {
    Task::EInitFlag stat = TrackAna::CheckFormat();
    if (stat == Task::EInitFlag::kFATAL) return stat;
    if (DataFormatManager::Instance()->GetFormat(GetTaskID(), EFormatDepth::kBuffered) == nullptr) {
      Cout::PrintInfo(Form("%s %s", this->ClassName(), ": Buffered format not set, fixing"), EInfo::kImportantWarning);
      if (DataFormatManager::Instance()->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered) == nullptr) {
        Cout::PrintInfo("Cannot fix, non-buffered format not present", EInfo::kLessError);
        return Task::EInitFlag::kFATAL;
      }
      SetFormatBuffered(DataFormatManager::Instance()->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered)->GetNewEvent());
    }
    const Event* nonBuf = DataFormatManager::Instance()->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered);
    const Event* Buf    = DataFormatManager::Instance()->GetFormat(GetTaskID(), EFormatDepth::kBuffered);
    if (Buf->IsCompatible(nonBuf)) {
#ifdef HAL_DEBUG
      Cout::PrintInfo("Formats are compatible", EInfo::kLessInfo);
#endif
      return Task::EInitFlag::kSUCCESS;
    } else {
      Cout::PrintInfo(Form("Format %s is not compatible with %s", nonBuf->GetFormatName().Data(), Buf->GetFormatName().Data()),
                      EInfo::kLessError);
    }
    return Task::EInitFlag::kSUCCESS;
  }
}  // namespace Hal
