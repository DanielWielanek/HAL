/*
 * DbgTask.cxx
 *
 *  Created on: 20 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "DbgTask.h"

#include <TSystem.h>

#include <iostream>

namespace HalDbg {
  void Task::SetLogFile(TString name) { fOutName = name; }

  Task::EInitFlag Task::Init() {
    if (fOutName.Length() > 0) {
      fUseLog = kTRUE;
      fOutLog.open(fOutName);
    }
    gSystem->GetProcInfo(&fProcInfo);
    fPrevTime = fProcInfo.fCpuUser;
    fTimer.Start();
    if (fModulo < 0) fModulo = 1;
    return Task::EInitFlag::kSUCCESS;
  }

  void Task::Exec(Option_t* /*option*/) {
    if (fCounter % fModulo == 0) {
      gSystem->GetProcInfo(&fProcInfo);
      Double_t RamKB = fProcInfo.fMemResident;
      Double_t dTime = fProcInfo.fCpuUser - fPrevTime;
      fPrevTime      = fProcInfo.fCpuUser;
      TString info   = Form("Event %8d   ResMem: %5d (MB)   DUserCPU: %3.4f (s)", fCounter, int(RamKB / 1000.0), dTime);
      if (fUseLog) {
        fOutLog << info << std::endl;
      } else {
        std::cout << info << std::endl;
      }
    }
    fCounter++;
  }

  void Task::FinishTask() {
    fTimer.Stop();
    if (fUseLog) {
      fOutLog << "Timer report" << std::endl;
      fOutLog << "CPU TIME: " << fTimer.CpuTime() << std::endl;
      fOutLog << "Events processed: " << fCounter << std::endl;
      fOutLog.close();
    } else {
      std::cout << "Timer report" << std::endl;
      std::cout << "CPU TIME: " << fTimer.CpuTime() << std::endl;
      std::cout << "Events processed: " << fCounter << std::endl;
    }
  }
}  // namespace HalDbg
