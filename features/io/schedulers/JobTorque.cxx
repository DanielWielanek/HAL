/*
 * JobTorque.cxx
 *
 *  Created on: 17 sty 2026
 *      Author: daniel
 */

#include "JobTorque.h"

#include <TMathBase.h>
#include <TSystem.h>
#include <fstream>

namespace Hal {


  JobTorque::JobTorque(TString xmlfile) {
    if (xmlfile.Length() == 0) return;
    JobTorque::LoadCore(xmlfile);
  }

  void JobTorque::Submit(Bool_t submit) {
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

  void JobTorque::BuildTmpFile() {
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

  void JobTorque::BuildSingleFile(Int_t jobid) {
    std::ofstream plik(Form("%s/job_%i", GetTmpFile().Data(), TMath::Max(jobid, 0)));
    plik << fShell.name << std::endl;
    auto addLine = [&](TString name, ParPair pars) {
      if (pars.enabled) { plik << "#PBS -" << name << pars.name << std::endl; }
    };

    if (!IsDirect()) {
      addLine("l walltime=", fTime);
      addLine("l mem=", fMem);
      addLine("q ", fQueue);
      addLine("N ", fName);
      addLine("l ppn=", fCpuPerTask);
      auto copyL = fLogs;
      auto copyE = fErrors;
      if (!IsArray()) {
        copyL.name = copyL.name.ReplaceAll("%x_%j", Form("%i", jobid));
        copyE.name = copyE.name.ReplaceAll("%x_%j", Form("%i", jobid));
      }
      addLine("o ", copyL);
      addLine("e ", copyE);
      plik << "#PBS -t" << Form("%i-%i", GetStart(), GetEnd()) << std::endl;
    }
    if (IsArray()) {
      TString env = "`expr ${PBS_ARRAYID}`";
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

  TString JobTorque::BuildArgsCommand(Int_t jobid) const {
    TString command = TString("qsub ") + Form("%s/job_%i", GetTmpFile().Data(), TMath::Max(0, jobid));
    if (!IsDirect()) return command;
    auto addLine = [&](TString namepar, ParPair pars) {
      if (pars.enabled) { command = command + " --" + namepar + "=" + pars.name; }
    };
    if (IsArray()) { command = command + " --array=" + Form("%i", GetStart()) + "-" + Form("%i", GetEnd()); }
    addLine("l walltime=", fTime);
    addLine("l mem=", fMem);
    addLine("l ppn=", fCpuPerTask);
    addLine("q ", fQueue);
    addLine("N ", fName);
    auto copyL = fLogs;
    auto copyE = fErrors;
    if (!IsArray()) {
      copyL.name = copyL.name.ReplaceAll("%x_%j", Form("%i", jobid));
      copyE.name = copyE.name.ReplaceAll("%x_%j", Form("%i", jobid));
    }
    addLine("o ", copyL);
    addLine("e ", copyE);
    command = command + " --t=" + Form("%i-%i", GetStart(), GetEnd());
    if (fIsDependent && (jobid == 0 || jobid == -1)) { command = command + Form(" -W depend=afterok:%i", fDependencyId); }
    return command;
  }

  TString JobTorque::GetLogPath(TString raw) const {
    if (raw.Length() == 0 || raw.Contains("/dev/null")) return "/dev/null";
    return Form("%s/%%A_%%a.out", raw.Data());
  }

  TString JobTorque::GetErrorPath(TString raw) const {
    if (raw.Length() == 0 || raw.Contains("/dev/null")) return "/dev/null";
    return Form("%s/%%A_%%a.err", raw.Data());
  }

  TString JobTorque::GetMemoryFlags(TString mbs) const { return Form("%smb", mbs.Data()); }

  TString JobTorque::GetTimeFlag(std::array<int, 4> arr) const {
    if (arr[0] != 0) { return Form("%i-%02d:%02d:%0d", arr[0], arr[1], arr[2], arr[3]); }
    return Form("%02d:%02d:%0d", arr[1], arr[2], arr[3]);
  }

  JobTorque::~JobTorque() {}


} /* namespace Hal */
