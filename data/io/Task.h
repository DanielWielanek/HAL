/*
 * Task.h
 *
 *  Created on: 5 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_BASE_STEER_HALTASK_H_
#define HAL_ANALYSIS_BASE_STEER_HALTASK_H_

#include <TTask.h>

namespace Hal {

  class Task : public TTask {

  public:
    enum EInitFlag { kSUCCESS, kERROR, kFATAL };
    Task();
    EInitFlag Init() { return kSUCCESS; };
    virtual void FinishTask() {};
    virtual ~Task();
    ClassDef(Task, 1)
  };
}  // namespace Hal
#endif /* HAL_ANALYSIS_BASE_STEER_HALTASK_H_ */
