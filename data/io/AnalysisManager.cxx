/*
 * RunAna.cxx
 *
 *  Created on: 4 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "AnalysisManager.h"

#include "DataManager.h"
#include "IOManager.h"
#include "Package.h"
#include "Parameter.h"
#include "Source.h"
#include "Std.h"
#include "Task.h"

#include <cstdlib>

#include <RtypesCore.h>
#include <TFile.h>

namespace Hal {
  AnalysisManager::AnalysisManager() : fProcessedEvents(0), fSource(nullptr), fOutTreeName("HalTree"), fManager(nullptr) {}

  void AnalysisManager::Init() {
    if (fSource == nullptr) exit(0);
    fManager         = fSource->GetIOManager();
    DataManager* mng = DataManager::Instance();
    mng->SetManager(fManager);
    for (auto task : fTasks) {
      Task::EInitFlag stat = task->Init();
      switch (stat) {
        case Task::EInitFlag::kERROR: {
          fPassiveTasks.push_back(task);
        } break;
        case Task::EInitFlag::kSUCCESS: {
          fActiveTasks.push_back(task);
        } break;
        case Task::EInitFlag::kFATAL: {
          exit(0);
        } break;
      }
    }
  }

  void AnalysisManager::Run(Int_t start, Int_t end) {
    if (start < fManager->GetEntries()) return;
    if (end < fManager->GetEntries()) { end = fManager->GetEntries(); }
    for (int i = start; i < end; i++) {
      ++fProcessedEvents;
      fManager->GetEntry(i);
      for (auto task : fActiveTasks) {
        task->Exec("");
      }
    }
    Finish();
  }

  void AnalysisManager::Finish() {
    auto GoToDir = [](TString name) {
      TDirectory* dir            = (TDirectory*) gFile;
      std::vector<TString> array = Hal::Std::ExplodeString(name, '/');
      for (int i = 0; i < (int) array.size(); i++) {
        TDirectory* newdir = (TDirectory*) dir->FindObject(array[i]);
        if (!newdir) { dir->mkdir(array[i]); }
        dir->cd(array[i]);
      }
    };
    Package* metadata_new = new Package();
    metadata_new->SetName("RunInfo");
    metadata_new->AddObject(new ParameterString("Software ver", HAL_PHYSICALANALYSYS_VER));
    metadata_new->AddObject(new ParameterString("Date", Hal::Std::GetDate(), 'f'));
    metadata_new->AddObject(new ParameterString("Time", Hal::Std::GetTime(), 'f'));
    metadata_new->AddObject(new ParameterUInt("Processed_events", fProcessedEvents, '+'));
    metadata_new->AddObject(new ParameterString("Input file", DataManager::Instance()->GetInFile()->GetName(), 'f'));
    TList* list = new TList();
    for (auto task : fTasks) {
      TString name  = task->ClassName();
      TString label = "Active";
      for (auto probeTask : fPassiveTasks) {
        if (probeTask == task) label = "Passive";
      }
      list->AddLast(new ParameterString(name, label));
    }
    metadata_new->AddObject(list);

    GoToDir("Info");
    TDirectory* dir        = (TDirectory*) gFile;
    TDirectory* metadatata = (TDirectory*) dir->Get("Info");
    if (metadatata->Get("RunInfo")) {
      delete metadata_new;
    } else {
      metadata_new->Write("RunInfo");
    }
    gFile->cd();
    for (auto task : fTasks) {
      task->FinishTask();
    }
  }

  AnalysisManager::~AnalysisManager() {
    delete fSource;
    delete fManager;
    for (auto task : fTasks) {
      delete task;
    }
  }

}  // namespace Hal
