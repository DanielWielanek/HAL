/*
 * JobsSbatch.cpp
 *
 *  Created on: 17 sty 2026
 *      Author: daniel
 */

#include <TMathBase.h>
#include <TSystem.h>
#include <fstream>
#include <iostream>

#include "JobSbatch.h"
#include "XMLNode.h"

namespace Hal {

  JobSbatch::JobSbatch(TString xmlfile) {
    if (xmlfile.Length() == 0) return;
    JobSbatch::LoadCore(xmlfile);
  }

  void JobSbatch::Submit(Bool_t submit) {
    BuildTmpFile();
    if (IsArray()) {
      TString command = BuildArgsCommand(-1);
      SendCommand(submit, command, -1);
    } else {
      for (int i = GetStart(); i <= GetEnd(); i++) {
        TString command = BuildArgsCommand(i);
        SendCommand(submit, command, i);
      }
    }
  }

  void JobSbatch::BuildTmpFile() {
    TString dir = GetTmpFile();
    gSystem->mkdir(dir, true);
    if (IsArray()) {
      BuildSingleFile(-1);
    } else {
      for (int i = GetStart(); i <= GetEnd(); i++) {
        BuildSingleFile(i);
      }
    }
  }

  void JobSbatch::BuildSingleFile(Int_t jobid) {
    std::ofstream plik(Form("%s/job_%i", GetTmpFile().Data(), TMath::Max(jobid, 0)));
    plik << fShell.name << std::endl;
    auto addLine = [&](TString name, ParPair pars) {
      if (pars.enabled) { plik << "#SBATCH --" << name << "=" << pars.name << std::endl; }
    };

    if (!IsDirect()) {
      addLine("time", fTime);
      addLine("mem", fMem);
      addLine("mem-per-cpu", fMemPerCpu);
      addLine("ntasks", fTasks);
      addLine("cpus-per-task", fCpuPerTask);
      addLine("partition", fQueue);
      addLine("job-name", fName);
      auto copyL = fLogs;
      auto copyE = fErrors;
      if (!IsArray()) {
        copyL.name = copyL.name.ReplaceAll("%x_%j", Form("%i", jobid));
        copyE.name = copyE.name.ReplaceAll("%x_%j", Form("%i", jobid));
      } else {
        plik << "#SBATCH --array=" << GetStart() << "-" << GetEnd() << std::endl;
      }
      addLine("output", copyL);
      addLine("error", copyE);
    }
    if (IsArray()) {
      TString env = "`expr $SLURM_ARRAY_TASK_ID`";
      plik << GetExport(env) << std::endl;
    } else {
      TString env = Form("%i", jobid);
      plik << GetExport(env) << std::endl;
    }
    for (auto i : GetCommands()) {
      plik << i << std::endl;
    }

    plik.close();
  }

  TString JobSbatch::BuildArgsCommand(Int_t jobID) const {
    TString command = TString("sbatch ") + Form("%s/job_%i", GetTmpFile().Data(), TMath::Max(0, jobID));
    if (fExtra.enabled) { command = command + " " + fExtra.name; }
    if (!IsDirect()) return command;
    auto addLine = [&](TString namepar, ParPair pars) {
      if (pars.enabled) { command = command + " --" + namepar + "=" + pars.name; }
    };
    if (IsArray()) { command = command + " --array=" + Form("%i", GetStart()) + "-" + Form("%i", GetEnd()); }
    addLine("time", fTime);
    addLine("mem", fMem);
    addLine("mem-per-cpu", fMemPerCpu);
    addLine("ntasks", fTasks);
    addLine("cpus-per-task", fCpuPerTask);
    addLine("partition", fQueue);
    addLine("job-name", fName);
    auto copyL = fLogs;
    auto copyE = fErrors;
    if (!IsArray()) {
      copyL.name = copyL.name.ReplaceAll("%x_%j", Form("%i", jobID));
      copyE.name = copyE.name.ReplaceAll("%x_%j", Form("%i", jobID));
    }
    addLine("output", copyL);
    addLine("error", copyE);
    if (fIsDependent && (jobID == 0 || jobID == -1)) { command = command + Form(" --dependency=afterany:%i", fDependencyId); }
    return command;
  }

  TString JobSbatch::GetLogPath(TString raw) const {
    if (raw.Length() == 0 || raw.Contains("/dev/null")) return "/dev/null";
    return Form("%s/%%x_%%j.out", raw.Data());
  }

  TString JobSbatch::GetErrorPath(TString raw) const {
    if (raw.Length() == 0 || raw.Contains("/dev/null")) return "/dev/null";
    return Form("%s/%%x_%%j.err", raw.Data());
  }

  TString JobSbatch::GetMemoryFlags(TString mbs) const { return mbs; }

  TString JobSbatch::GetTimeFlag(std::array<int, 4> arr) const {
    if (arr[0] != 0) { return Form("%i-%02d:%02d:%0d", arr[0], arr[1], arr[2], arr[3]); }
    return Form("%02d:%02d:%0d", arr[1], arr[2], arr[3]);
  }

  JobSbatch::~JobSbatch() {}


} /* namespace Hal */
