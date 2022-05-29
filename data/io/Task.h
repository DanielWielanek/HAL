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

/**
 * basic class that represents task
 */
namespace Hal {
  class Package;
  class Task : public TTask {
  protected:
    /**
     * go to subdirectory in output file
     * @param name path to directory
     */
    void GoToDir(TString name);

  public:
    enum class EInitFlag { kSUCCESS, kERROR, kFATAL };
    /**
     * default constructor
     */
    Task();
    /**
     * initialize task
     * @return flag of initization, if equal to ERROR task will not be executed, if FATAL analysis will be stopped
     */
    virtual EInitFlag Init() { return EInitFlag::kSUCCESS; };
    /**
     * called at the end of analysis, writes data
     */
    virtual void FinishTask() {};
    virtual ~Task();
    ClassDef(Task, 1)
  };
}  // namespace Hal
#endif /* HAL_ANALYSIS_BASE_STEER_HALTASK_H_ */
