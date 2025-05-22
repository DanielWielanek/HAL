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
#include "MagField.h"

#include "Cout.h"
#include "Package.h"
#include "Parameter.h"
#include "Reader.h"
#include "Source.h"
#include "Std.h"
#include "Task.h"
#include "TriggerTask.h"

#include <cstdlib>

#include <RtypesCore.h>
#include <TFile.h>
#include <TInterpreter.h>

namespace Hal {
  AnalysisManager::AnalysisManager() {}

  Bool_t AnalysisManager::Init() {
    Cout::PrintInfo("=== AnalysisManager::Init ===", EInfo::kInfo);
    fTimer.Start();
    if (fSource == nullptr) exit(0);
    Cout::PrintInfo("=== Source::Init ===", EInfo::kInfo);
    Bool_t initSource = fSource->Init();
    if (!initSource) { Cout::PrintInfo("Can't init source!", EInfo::kCriticalError); }
    fManager = fSource->GetIOManager();
    if (!fManager) {
      Cout::PrintInfo("IO manager not found!", EInfo::kCriticalError);
      exit(0);
    }
    if (fField == nullptr) { fField = new MagField(); }
    Cout::PrintInfo("=== Setting up the manager ===", EInfo::kInfo);
    fManager->SetField(fField);
    fManager->SetOutput(fOutputFile);
    fManager->Init();
    DataManager* mng = DataManager::Instance();
    mng->SetManager(fManager);
    Cout::PrintInfo("=== Init triggers ===", EInfo::kDebugInfo);
    for (auto task : fTriggers) {
      Cout::PrintInfo(Form("  Init trigger %s", task->ClassName()), EInfo::kDebugInfo);
      Task::EInitFlag stat = task->Init();
      switch (stat) {
        case Task::EInitFlag::kERROR: {
          Cout::PrintInfo(Form("  Trigger %s go to passive mode", task->ClassName()), EInfo::kDebugInfo);
          fPassiveTriggers.push_back(task);
        } break;
        case Task::EInitFlag::kSUCCESS: {
          task->MarkAsActive();
          Cout::PrintInfo(Form("  Trigger %s go to active mode", task->ClassName()), EInfo::kDebugInfo);
          fActiveTriggers.push_back(task);
        } break;
        case Task::EInitFlag::kFATAL: {
          Cout::PrintInfo(Form("  Failed to init %s", task->ClassName()), EInfo::kError);
          exit(0);
        } break;
      }
    }
    Cout::PrintInfo("=== Init tasks ===", EInfo::kInfo);
    for (auto task : fTasks) {
      Cout::PrintInfo(Form("==> Init task %s", task->ClassName()), EInfo::kDebugInfo);
      Task::EInitFlag stat = task->Init();
      switch (stat) {
        case Task::EInitFlag::kERROR: {
          Cout::PrintInfo(Form("  Task %s go to passive mode", task->ClassName()), EInfo::kDebugInfo);
          fPassiveTasks.push_back(task);
        } break;
        case Task::EInitFlag::kSUCCESS: {
          Cout::PrintInfo(Form("  Task %s go to active mode", task->ClassName()), EInfo::kDebugInfo);
          fActiveTasks.push_back(task);
        } break;
        case Task::EInitFlag::kFATAL: {
          Cout::PrintInfo(Form("  Failed to init %s", task->ClassName()), EInfo::kError);
          exit(0);
        } break;
      }
      fManager->CdToOutput();  // to clean up if user forget to come back to output file
    }
    if (fActiveTriggers.size() > 0) { fTriggersEnabled = kTRUE; }
    Cout::PrintInfo("=== Locking unused branches ===", EInfo::kDebugInfo);
    fManager->LockUnusedBranches();  // lock unused branches by setting status to zero
    if (Hal::Cout::GetVerboseMode() <= EInfo::kDebugInfo) {
      Cout::PrintInfo("=== Manager status ===", EInfo::kDebugInfo);
      fManager->PrintInfo();
    }
    fInitTime = fTimer.CpuTime();

    return kTRUE;
  }

  void AnalysisManager::Run(Int_t start, Int_t end) {
    fTimer.Reset();
    fTimer.Start();
    if (start == -1 && end == -1) {
      start = 0;
      end   = fManager->GetEntries();
    }
    if (start < 0) start = 0;
    if (start > fManager->GetEntries()) { start = fManager->GetEntries(); }
    if (end > fManager->GetEntries()) { end = fManager->GetEntries(); }
    Cout::PrintInfo(Form("Run from %i to %i events", start, end), EInfo::kInfo);
    Int_t percent = (end - start) / 100;
    if (percent == 0) percent = 1;
    for (int i = start; i < end; i++) {
      ++fProcessedEvents;
      if (fProgressBar) {
        if (fProcessedEvents % percent == 1) Hal::Cout::ProgressBar(fProcessedEvents, end - start);
      }
      DoStep(i);
      fManager->FillTree();
    }
    fTimer.Stop();
    fFinishTime = fTimer.CpuTime();
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
    metadata_new->AddObject(new ParameterString("Input file", DataManager::Instance()->GetSourceName(), 'f'));
    metadata_new->AddObject(new ParameterString("Input macro", gInterpreter->GetCurrentMacroName(), 'f'));
    metadata_new->AddObject(new ParameterDouble("Initialization Time [s]", fInitTime, 'f'));
    metadata_new->AddObject(new ParameterDouble("Processing time [s]", fFinishTime, 'f'));
    metadata_new->AddObject(fManager->GetBranchesList());

    TList* trigList = new TList();
    trigList->SetName("EventTriggers");

    for (auto probeTask : fActiveTriggers) {
      trigList->AddLast(probeTask->Report());
    }
    for (auto probeTask : fPassiveTriggers) {
      trigList->AddLast(probeTask->Report());
    }
    metadata_new->AddObject(trigList);

    TList* list = new TList();
    list->SetName("Tasks");
    for (auto task : fTasks) {
      TString name  = task->ClassName();
      TString label = "Active";
      for (auto probeTask : fPassiveTasks) {
        if (probeTask == task) label = "Passive";
      }
      list->AddLast(new ParameterString(name, label));
    }
    metadata_new->AddObject(list);

    fManager->CdToOutput();
    TDirectory* dir = (TDirectory*) gFile;

    GoToDir("HalInfo");
    TDirectory* metadatata = (TDirectory*) dir->Get("HalInfo");
    if (metadatata->Get("RunInfo")) {
      delete metadata_new;
    } else {
      metadata_new->Write("RunInfo");
    }
    gFile->cd();
    for (auto task : fActiveTasks) {
      task->FinishTask();
    }
    Cout::PrintInfo("Analysis completed", EInfo::kInfo);
    fManager->CloseManager();
  }

  void AnalysisManager::AddTask(Task* ana) {
    if (auto trig = dynamic_cast<TriggerTask*>(ana)) {
      fTriggers.push_back(trig);
    } else if (auto reader = dynamic_cast<Reader*>(ana)) {
      if (fTasks.size() != 0)
        Hal::Cout::PrintInfo("You are adding reader as not a first task,  this might lead to undefined behaviour",
                             EInfo::kWarning);
      fTasks.push_back(ana);
    } else {
      fTasks.push_back(ana);
    }
  }

  AnalysisManager::~AnalysisManager() {
    delete fSource;
    delete fManager;
    for (auto task : fTasks) {
      delete task;
    }
    for (auto task : fTriggers) {
      delete task;
    }
    if (fField) delete fField;
  }

  void AnalysisManager::DoStep(Int_t entry) {
    if (fTriggersEnabled) {
      fManager->GetEntry(entry, 0);
      for (auto task : fActiveTriggers) {
        task->Exec("");
        if (!task->IsEventGood()) return;
      }
    }
    fManager->GetEntry(entry, 1);
    for (auto task : fActiveTasks) {
      task->Exec("");
    }
  }

}  // namespace Hal
