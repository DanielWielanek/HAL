/*
 * EventAnaChain.cxx
 *
 *  Created on: 26 sie 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "EventAnaChain.h"

#include "Cout.h"
#include "Cut.h"
#include "CutCollection.h"
#include "CutContainer.h"
#include "EventVirtualCut.h"
#include "MemoryMapManager.h"
#include "MultiTrackAna.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"
#include "TrackAna.h"

#include <TFile.h>
#include <TString.h>


namespace Hal {
  EventAnaChain::EventAnaChain() : fTaskNo(0), fTask(NULL) {}

  Package* EventAnaChain::Report() const {
    Package* pack = EventAna::Report();
    for (int i = 0; i < fTaskNo; i++) {
      pack->AddObject(new ParameterString(Form("Subtask %i name", i), fTask[i]->ClassName()));
    }
    return pack;
  }

  void EventAnaChain::AddAnalysis(EventAna* ana) {
    if (dynamic_cast<MultiTrackAna*>(ana)) {
      Cout::PrintInfo("Tasks with track buffering cannot be processed vy this task", EInfo::kImportantWarning);
      return;
    }
    if (dynamic_cast<TrackAna*>(ana)) {
      Cout::PrintInfo(Form("Task %s inherits from TrackAna, any features connected to "
                           "linking collections might not work correltly",
                           ana->ClassName()),
                      EInfo::kLessError);
      return;
    }
    if (fTask == nullptr) {
      ana->MarkAsInChain();
      fTask    = new EventAna*[1];
      fTask[0] = ana;
      fTaskNo++;
    } else {
      for (int i = 0; i < fTaskNo; i++) {
        if (ana == fTask[i]) {
          Cout::PrintInfo("Task already added", EInfo::kLessError);
          return;
        }
      }
      EventAna** temp = fTask;
      fTask           = new EventAna*[fTaskNo + 1];
      for (int i = 0; i < fTaskNo; i++) {
        fTask[i] = temp[i];
      }
      ana->MarkAsInChain();
      fTask[fTaskNo] = ana;
      fTaskNo++;
      delete[] temp;
    }
  }

  void EventAnaChain::FinishTask() {
    Package* pack         = Report();
    Package* metadata_new = new Package();
    metadata_new->SetName("RunInfo");
    metadata_new->AddObject(new ParameterString("Software ver", HAL_PHYSICALANALYSYS_VER));
    metadata_new->AddObject(new ParameterString("Date", Hal::Std::GetDate(), 'f'));
    metadata_new->AddObject(new ParameterString("Time", Hal::Std::GetTime(), 'f'));
    metadata_new->AddObject(new ParameterUInt("Processed_events", fProcessedEvents, '+'));
    metadata_new->AddObject(new ParameterString("Input file", GetInputFileName(), 'f'));
    GoToDir("Info");
    TDirectory* dir        = (TDirectory*) gFile;
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
      delete subtask_pack;
      fTask[i]->fMemoryMap = NULL;
    }
    gFile->cd();
    Cout::PrintInfo(Form("%s done, writing results", this->ClassName()), EInfo::kImportantInfo);
  }

  void EventAnaChain::ProcessEvent() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentEventCollectionID = fCurrentEventCollectionID;
      fTask[i]->ProcessEvent();
    }
  }

  Task::EInitFlag EventAnaChain::Init() {
    Task::EInitFlag stat = EventAna::Init();
    if (stat == Task::EInitFlag::kSUCCESS) {
      for (int i = 0; i < fTaskNo; i++) {
        fTask[i]->SetFormat(fCurrentEvent);
        fTask[i]->AddCut(EventVirtualCut(), Form("{%ix1}", fEventCollectionsNo));
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

  void EventAnaChain::Exec(Option_t* /*opt*/) {
    fProcessedEvents++;
    fCurrentEvent = fMemoryMap->GetTemporaryEvent();
    for (int i = 0; i < fTaskNo; i++)
      fTask[i]->fCurrentEvent = fCurrentEvent;
    for (fCurrentEventCollectionID = 0; fCurrentEventCollectionID < fEventCollectionsNo; fCurrentEventCollectionID++) {
      if (fCutContainer->PassEvent(fCurrentEvent, fCurrentEventCollectionID)) { ProcessEvent(); }
    }
  }

  void EventAnaChain::RemoveTask(Int_t no) {
    EventAna** temp = fTask;
    fTask           = new EventAna*[fTaskNo - 1];
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

  void EventAnaChain::GoToDir(TString name) {
    TDirectory* dir    = (TDirectory*) gFile;
    TDirectory* target = (TDirectory*) dir->FindObject(name);
    if (!target) { dir->mkdir(name); }
    gFile->cd(name);
  }

  void EventAnaChain::LinkTask(EventAna* ana) const {
    delete ana->fMemoryMap;
    ana->fMemoryMap    = fMemoryMap;
    ana->fCurrentEvent = fCurrentEvent;
  }

  void EventAnaChain::UnlinkTask(EventAna* ana) const { ana->fMemoryMap = NULL; }

  void EventAnaChain::SynchronizeCutContainers(EventAna* ana, Bool_t end) const {
    ana->fCutContainer->MakeDummyCopies(ECutUpdate::kEvent, fCutContainer, end);
  }

  EventAnaChain::~EventAnaChain() {
    for (int i = 0; i < fTaskNo; i++) {
      UnlinkTask(fTask[i]);
      delete fTask[i];
    }
    delete[] fTask;
  }
}  // namespace Hal
