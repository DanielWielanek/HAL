/*
 * TrackAnaChain.cxx
 *
 *  Created on: 26 sie 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackAnaChain.h"

#include "Cout.h"
#include "CutCollection.h"
#include "CutContainer.h"
#include "Event.h"
#include "EventAna.h"
#include "EventVirtualCut.h"
#include "HalStd.h"
#include "MemoryMapManager.h"
#include "Package.h"
#include "Parameter.h"
#include "TrackVirtualCut.h"

#include <TFile.h>
#include <TString.h>


namespace Hal {
  TrackAnaChain::TrackAnaChain() : fTaskNo(0), fTask(NULL) {}

  void TrackAnaChain::ProcessEvent() {
    CutCollection* cont = fCutContainer->GetEventCollection(fCurrentEventCollectionID);
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentEventCollectionID = fCurrentEventCollectionID;
    }
    for (int i = 0; i < fMemoryMap->GetTemporaryTotalTracksNo(); i++) {
      for (int j = 0; j < cont->GetNextNo(); j++) {
        fCurrentTrackCollectionID = cont->GetNextAddr(j);
        fCurrentTrack             = fCurrentEvent->GetTrack(i);
        if (fCutContainer->PassTrack(fCurrentTrack, fCurrentTrackCollectionID)) {
          fMemoryMap->AddTrackToMapTrack(fCurrentEventCollectionID,
                                         fCurrentTrackCollectionID,
                                         i);  // load track into memory map - may be usefull at finish event
          for (int k = 0; k < fTaskNo; k++) {
            fTask[k]->fCurrentTrackCollectionID = fCurrentTrackCollectionID;
            fTask[k]->ProcessTrack();
          }
        }
      }
    }
  }

  Task::EInitFlag TrackAnaChain::Init() {
    Task::EInitFlag stat = TrackAna::Init();
    if (stat == Task::EInitFlag::kSUCCESS) {
      fTrackCollectionsNo = fCutContainer->GetTrackCollectionsNo();
      for (int i = 0; i < fTaskNo; i++) {
        fTask[i]->SetFormat(fCurrentEvent);
        SynchronizeCutContainers(fTask[i], kTRUE);
        if (Task::EInitFlag::kFATAL == fTask[i]->Init()) {
          RemoveTask(i);
          continue;
        }
        LinkTask(fTask[i]);
      }
      return Task::EInitFlag::kSUCCESS;
    }
    return Task::EInitFlag::kFATAL;
  }

  void TrackAnaChain::RemoveTask(Int_t no) {
    TrackAna** temp = fTask;
    fTask           = new TrackAna*[fTaskNo - 1];
    Int_t n         = 0;
    for (int i = 0; i < fTaskNo; i++) {
      if (i == no) continue;
      fTask[n] = temp[i];
      n++;
    }
    delete temp[no];
    fTaskNo--;
    delete[] temp;
  }

  Package* TrackAnaChain::Report() const {
    Package* pack = EventAna::Report();
    for (int i = 0; i < fTaskNo; i++) {
      pack->AddObject(new ParameterString("Subtask name", fTask[i]->ClassName()));
    }
    return pack;
  }

  void TrackAnaChain::Exec(Option_t* /*opt*/) {
    fProcessedEvents++;
    fCurrentEvent = fMemoryMap->GetTemporaryEvent();
    for (fCurrentEventCollectionID = 0; fCurrentEventCollectionID < fEventCollectionsNo; fCurrentEventCollectionID++) {
      if (fCutContainer->PassEvent(fCurrentEvent, fCurrentEventCollectionID)) { ProcessEvent(); }
    }
  }

  void TrackAnaChain::AddAnalysis(TrackAna* ana) {
    if (!ana->InheritsFrom("TrackAna")) return;
    if (fTask == NULL) {
      fTask = new TrackAna*[1];
      ana->MarkAsInChain();
      fTask[0] = ana;
      fTaskNo++;
    } else {
      for (int i = 0; i < fTaskNo; i++) {
        if (ana == fTask[i]) {
          Cout::PrintInfo("Task already added", EInfo::kLessError);
          return;
        }
      }
      if (ana->InheritsFrom("MultiTrackAna")) {
        Cout::PrintInfo("Tasks with track buffering cannot be processed vy this task", EInfo::kLessError);
      }
      TrackAna** temp = fTask;
      fTask           = new TrackAna*[fTaskNo + 1];
      for (int i = 0; i < fTaskNo; i++) {
        fTask[i] = temp[i];
      }
      ana->MarkAsInChain();
      fTask[fTaskNo] = ana;
      fTaskNo++;
      delete[] temp;
    }
  }

  void TrackAnaChain::FinishTask() {
    Package* pack         = Report();
    Package* metadata_new = new Package();
    metadata_new->SetName("RunInfo");
    metadata_new->AddObject(new ParameterString("Software ver", HAL_PHYSICALANALYSYS_VER));
    metadata_new->AddObject(new ParameterString("Date", HalStd::GetDate(), 'f'));
    metadata_new->AddObject(new ParameterString("Time", HalStd::GetTime(), 'f'));
    metadata_new->AddObject(new ParameterUInt("Processed_events", fProcessedEvents, '+'));
    metadata_new->AddObject(new ParameterString("Input file", GetInputFileName(), 'f'));
    GoToDir("Info");
    TDirectory* dir = (TDirectory*) gFile;
    dir->Cd("Info");
    TDirectory* metadatata = (TDirectory*) dir->Get("Info");
    if (metadatata->Get("RunInfo")) {
      delete metadata_new;
    } else {
      metadata_new->Write("RunInfo");
    }
    GoToDir("Physics");
    pack->Write(Form("AnaPackage_%i", GetTaskID()));
    if (pack) { delete pack; }
    for (int i = 0; i < fTaskNo; i++) {
      SynchronizeCutContainers(fTask[i], kFALSE);
      Package* subtask_pack = fTask[i]->Report();
      subtask_pack->Write(Form("AnaPackage_%i", fTask[i]->GetTaskID()));
      fTask[i]->fCurrentTrack = NULL;
      fTask[i]->fCurrentEvent = NULL;
      delete subtask_pack;
    }
    gFile->cd();
    Cout::PrintInfo(Form("%s done, writing results", this->ClassName()), EInfo::kImportantInfo);
  }

  void TrackAnaChain::GoToDir(TString name) {
    TDirectory* dir    = (TDirectory*) gFile;
    TDirectory* target = (TDirectory*) dir->FindObject(name);
    if (!target) { dir->mkdir(name); }
    gFile->cd(name);
  }

  void TrackAnaChain::LinkTask(TrackAna* ana) const {
    delete ana->fMemoryMap;
    ana->fMemoryMap    = fMemoryMap;
    ana->fCurrentEvent = fCurrentEvent;
    ana->fCurrentTrack = fCurrentTrack;
  }

  void TrackAnaChain::UnlinkTask(TrackAna* ana) const {
    ana->fMemoryMap    = nullptr;
    ana->fCurrentEvent = nullptr;
    ana->fCurrentTrack = nullptr;
  }

  void TrackAnaChain::SynchronizeCutContainers(TrackAna* ana, Bool_t end) const {
    ana->AddCut(EventVirtualCut(), Form("{%ix1}", fEventCollectionsNo));
    ana->fCutContainer->MakeDummyCopies(ECutUpdate::kEventUpdate, fCutContainer, kTRUE);
    ana->AddCut(TrackVirtualCut(), Form("{%ix1}", fTrackCollectionsNo));
    ana->fCutContainer->MakeDummyCopies(ECutUpdate::kTrackUpdate, fCutContainer, end);
  }

  TrackAnaChain::~TrackAnaChain() {
    for (int i = 0; i < fTaskNo; i++) {
      UnlinkTask(fTask[i]);
      SynchronizeCutContainers(fTask[i], kFALSE);
      delete fTask[i];
    }
    delete[] fTask;
  }
}  // namespace Hal
