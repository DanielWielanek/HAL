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

namespace Hal {
  class IOManager;
  class Source;
} /* namespace Hal */

namespace Hal {
  namespace Fair {
    class TaskManager : public ::FairTask {
    protected:
      IOManager* fManager;
      std::vector<::Hal::Task*> fTasks;
      std::vector<::Hal::Task*> fActiveTasks;
      std::vector<::Hal::Task*> fPassiveTasks;
      UInt_t fProcessedEvents;

      virtual InitStatus Init();

    public:
      TaskManager();
      virtual void AddTask(::Hal::Task* task) { fTasks.push_back(task); };
      virtual void Exec(Option_t* option);
      virtual void Finish();
      virtual ~TaskManager();
      ClassDef(TaskManager, 1)
    };
  }  // namespace Fair
} /* namespace Hal */

#endif /* HAL_FAIR_HALFAIRANALYSISMANAGER_H_ */
