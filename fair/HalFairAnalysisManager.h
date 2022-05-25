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

  class HalFairAnalysisManager : public FairTask {
    UInt_t fProcessedEvents;
    IOManager* fManager;
    std::vector<Task*> fTasks;
    std::vector<Task*> fActiveTasks;
    std::vector<Task*> fPassiveTasks;

  protected:
    virtual InitStatus Init();

  public:
    HalFairAnalysisManager();
    virtual void Exec(Option_t* option);
    virtual void Finish();
    virtual ~HalFairAnalysisManager();
    ClassDef(HalFairAnalysisManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FAIR_HALFAIRANALYSISMANAGER_H_ */
