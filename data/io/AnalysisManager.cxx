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
#include "Source.h"
#include "Task.h"
#include <cstdlib>

#include <RtypesCore.h>

namespace Hal {
  AnalysisManager::AnalysisManager() : fSource(nullptr), fOutTreeName("HalTree"), fManager(nullptr) {}

  void AnalysisManager::Init() {
    if (fSource == nullptr) exit(0);
    fManager         = fSource->GetIOManager();
    DataManager* mng = DataManager::Instance();
    mng->SetManager(fManager);
    for (auto task : fTasks) {
      Task::EInitFlag stat = task->Init();
      switch (stat) {
        case Task::EInitFlag::kERROR: {
          // remove TODO Implement
        } break;
        case Task::EInitFlag::kSUCCESS: {
          // do nothing TODO Implement
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
      fManager->GetEntry(i);
      for (auto task : fTasks) {
        task->Exec("");
      }
    }
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
