/*
 * HalFairAnalysisManager.cxx
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "DataManager.h"
#include "IOManager.h"
#include "Package.h"
#include "Parameter.h"
#include "RootManager.h"
#include "TaskManager.h"

#include "Source.h"
#include "Std.h"
#include "Task.h"

#include <cstdlib>

#include <RtypesCore.h>
#include <TFile.h>
namespace Hal {
  namespace Fair {
    TaskManager::TaskManager() : fProcessedEvents(0), fManager(nullptr) { fManager = new RootManager(); }

    InitStatus TaskManager::Init() {
      fManager->Init();
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
            return kFATAL;
          } break;
        }
      }
      return kSUCCESS;
    }

    void TaskManager::Exec(Option_t* option) {
      ++fProcessedEvents;
      for (auto task : fActiveTasks) {
        task->Exec(option);
      }
    }

    void TaskManager::Finish() {
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

    TaskManager::~TaskManager() {
      // TODO Auto-generated destructor stub
    }
  }  // namespace Fair
} /* namespace Hal */
