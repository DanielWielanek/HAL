/*
 * HalFairAnalysisManager.h
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FAIR_HALFAIRANALYSISMANAGER_H_
#define HAL_FAIR_HALFAIRANALYSISMANAGER_H_

#include "Task.h"

#include <vector>

#include <FairTask.h>
#include <RtypesCore.h>
#include <TString.h>

/**
 * wrapper of task for FairRoot
 * You should add all task for analysis here, and later add this task as to the FairRunAna
 */
namespace Hal {
  class IOManager;
  class Source;
  class TriggerTask;
} /* namespace Hal */

namespace Hal {
  namespace Fair {
    class TaskManager : public FairTask {
    protected:
      IOManager* fManager;
      std::vector<::Hal::TriggerTask*> fTriggers;
      std::vector<::Hal::TriggerTask*> fActiveTriggers;
      std::vector<::Hal::TriggerTask*> fPassiveTriggers;
      std::vector<::Hal::Task*> fTasks;
      std::vector<::Hal::Task*> fActiveTasks;
      std::vector<::Hal::Task*> fPassiveTasks;
      UInt_t fProcessedEvents;
      virtual InitStatus Init();

    public:
      TaskManager() : TaskManager(nullptr) {};
      /**
       * custom constructor for non-standard files
       * @param manager - set only if you have non-TObject files in data
       */
      TaskManager(::Hal::IOManager* manager);
      virtual void AddTrigger(::Hal::TriggerTask* trigger) { fTriggers.push_back(trigger); }
      virtual void AddTask(::Hal::Task* task) { fTasks.push_back(task); };
      virtual void Exec(Option_t* option);
      virtual void Finish();
      virtual ~TaskManager();
      ClassDef(TaskManager, 1)
    };
  }  // namespace Fair
} /* namespace Hal */

#endif /* HAL_FAIR_HALFAIRANALYSISMANAGER_H_ */
