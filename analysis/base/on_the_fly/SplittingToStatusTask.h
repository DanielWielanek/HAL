/*
 * SplittingToStatusTask.h
 *
 *  Created on: 28 kwi 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALSPLITTINGTOSTATUSTASK_H_
#define HALSPLITTINGTOSTATUSTASK_H_

#include "EventAna.h"

#include <vector>


/**
 * if track is splitted then status is set 1 otherwise to 0, requires complex format as input
 * this task does not accept any cuts
 */
namespace Hal {
  class SplittingToStatusTask : public EventAna {
    std::vector<Int_t> fSplitFlag;

  protected:
    virtual Task::EInitFlag CheckFormat();
    virtual Task::EInitFlag Init();

  public:
    SplittingToStatusTask();
    SplittingToStatusTask(const SplittingToStatusTask& other);
    virtual void AddCut(const Cut& /*cut*/, Option_t* /*opt = ""*/) {};
    virtual void Exec(Option_t* opt = "");
    virtual ~SplittingToStatusTask();
    SplittingToStatusTask& operator=(const SplittingToStatusTask& other);
    ClassDef(SplittingToStatusTask, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO_ANALYSIS_BASE_ON_THE_FLY_HALSPLITTINGTOSTATUSTASK_H_ */
