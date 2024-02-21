/*
 * DbgTask.h
 *
 *  Created on: 20 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_DEBUGFORMAT_DBGTASK_H_
#define HAL_EXAMPLES_DEBUGFORMAT_DBGTASK_H_

#include "Task.h"

#include <TStopwatch.h>
#include <TSystem.h>
#include <fstream>


namespace HalDbg {
  class Task : public Hal::Task {
  protected:
    Int_t fModulo      = {1000};
    Int_t fCounter     = {0};
    Double_t fPrevTime = {0};
    TString fOutName;
    std::ofstream fOutLog;
    Bool_t fUseLog = {kFALSE};
    ProcInfo_t fProcInfo;
    TStopwatch fTimer;

  public:
    Task() {};
    virtual EInitFlag Init();
    void SetReportStep(Int_t step) { fModulo = step; }
    void SetLogFile(TString name);
    virtual ~Task() {}
    virtual void Exec(Option_t* option);
    virtual void FinishTask();
    ClassDef(Task, 1)
  };

}  // namespace HalDbg

#endif /* HAL_EXAMPLES_DEBUGFORMAT_DBGTASK_H_ */
