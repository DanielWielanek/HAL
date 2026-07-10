/*
 * JobsSbatch.h
 *
 *  Created on: 17 sty 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_IO_SCHEDULERS_JOBSBATCH_H_
#define HAL_FEATURES_IO_SCHEDULERS_JOBSBATCH_H_

#include <RtypesCore.h>
#include <TString.h>
#include <array>

#include "JobQueue.h"


namespace Hal {

  class JobSbatch : public JobQueue {

  protected:
    void MakeJobFile(Int_t jobid) const;
    TString MakeSubmitCommand(Int_t jobID) const;
    virtual TString GetLogPath(TString raw) const;
    virtual TString GetErrorPath(TString raw) const;
    virtual TString GetMemoryFlags(TString mbs) const;
    virtual TString GetTimeFlag(std::array<int, 4>) const;

  public:
    JobSbatch(TString xmlFile);
    virtual ~JobSbatch();
    ClassDef(JobSbatch, 0)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_SCHEDULERS_JOBSBATCH_H_ */
