/*
 * JobTorque.h
 *
 *  Created on: 17 sty 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_IO_SCHEDULERS_JOBTORQUE_H_
#define HAL_FEATURES_IO_SCHEDULERS_JOBTORQUE_H_

#include <RtypesCore.h>
#include <TString.h>
#include <array>

#include "JobQueue.h"


namespace Hal {

  class JobTorque : public JobQueue {

    void BuildTmpFile();
    void BuildSingleFile(Int_t jobid);
    TString BuildArgsCommand(Int_t jobID) const;

  protected:
    virtual TString GetLogPath(TString raw) const;
    virtual TString GetErrorPath(TString raw) const;
    virtual TString GetMemoryFlags(TString mbs) const;
    virtual TString GetTimeFlag(std::array<int, 4>) const;

  public:
    JobTorque(TString xmlFile);
    virtual void Submit(Bool_t submit);
    virtual ~JobTorque();
    ClassDef(JobTorque, 0)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_SCHEDULERS_JOBTORQUE_H_ */
