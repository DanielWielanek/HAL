/*
 * TriggerTask.h
 *
 *  Created on: 22 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_DATA_IO_TRIGGERTASK_H_
#define HAL_DATA_IO_TRIGGERTASK_H_

#include "Task.h"


namespace Hal {
  class Package;
  /**
   * special class for fast data selection - works only with AnalysisManager
   * when trigger task return false the branches with status 0 are ignored, other tasks are not executed
   */
  class TriggerTask : public Task {
    Bool_t fGood   = {kFALSE};
    Bool_t fActive = {kFALSE};

  protected:
    void MarkAsGood() { fGood = kTRUE; };
    void MarkAsBad() { fGood = kFALSE; };

  public:
    TriggerTask();
    void MarkAsActive(Bool_t active = kTRUE) { fActive = active; };
    Bool_t IsEventGood() const { return fGood; }
    virtual ~TriggerTask();
    TriggerTask(const TriggerTask& other) = default;
    Package* Report() const;
    ClassDef(TriggerTask, 1)
  };
}  // namespace Hal

#endif /* HAL_DATA_IO_TRIGGERTASK_H_ */
