/*
 * JobQueue.cxx
 *
 *  Created on: 16 sty 2026
 *      Author: daniel
 */

#include <TSystem.h>
#include <iostream>
#include <utility>

#include "JobQueue.h"
#include "JobSbatch.h"
#include "JobTorque.h"
#include "StdString.h"


namespace Hal {
  std::array<int, 4> JobQueue::GetTime(TString raw) const {
    std::array<int, 4> time {};
    auto vec = Hal::Std::ExplodeString(raw, ':', true);
    int c    = vec.size() - 4;
    for (unsigned int i = 0; i < vec.size(); i++) {
      time[i + c] = vec[i].Atoi();
    }
    return time;
  }

  void JobQueue::LoadCore(TString xmlfile) {
    auto xml   = Hal::XMLFile(xmlfile);
    auto& root = xml.GetRootNode();
    // parsing commands
    auto& commands = root["commands"];
    for (int i = 0; i < commands.GetNChildren(); i++) {
      fCommands.push_back(commands[i].GetValue());
    }
    // parsing parameters
    auto& parameters = root["parameters"];
    for (int i = 0; i < parameters.GetNChildren(); i++) {
      auto& parameter  = parameters[i];
      TString atribVal = parameter.GetAttrib("value").GetValue();
      if (atribVal.Length() > 0 && atribVal != "no") {
        fParameters[parameter.GetAttrib("name").GetValue()] = parameter.GetAttrib("value").GetValue();
      }
    }
    // parsing aliases
    auto& aliases = root["aliases"];
    std::vector<std::pair<TString, TString>> aliasarray;
    for (int i = 0; i < aliases.GetNChildren(); i++) {
      auto& alias = aliases[i];
      std::pair<TString, TString> aliasPair;
      aliasPair.first  = alias.GetAttrib("name").GetValue();
      aliasPair.second = alias.GetAttrib("value").GetValue();
      aliasarray.push_back(aliasPair);
    }
    std::pair<TString, TString> aliasPair;
    aliasPair.first  = "HAL::CONST::PWD";
    aliasPair.second = gSystem->pwd();
    aliasarray.push_back(aliasPair);

    for (auto& i : fCommands) {
      for (auto& a : aliasarray) {
        TString from = a.first;
        TString to   = a.second;
        i            = i.ReplaceAll(Form("${%s}", from.Data()), to.Data());
      }
    }
    for (auto it = fParameters.begin(); it != fParameters.end(); ++it) {
      for (auto& a : aliasarray) {
        TString from = a.first;
        TString to   = a.second;
        it->second   = it->second.ReplaceAll(Form("${%s}", from.Data()), to.Data());
      }
    }
    auto parStart = GetParameter("start");
    if (parStart.enabled) fStartJob = parStart.name.Atoi();

    auto parEnd = GetParameter("end");
    if (parEnd.enabled) fEndJob = parEnd.name.Atoi();

    if (GetParameter("array").enabled) fArray = kTRUE;

    if (GetParameter("direct").enabled) fDirectCommand = kTRUE;

    fShell = GetParameter("shell");

    auto parTmp = GetParameter("tmpfile");
    if (parTmp.enabled) {
      fTmpFile = parTmp.name;
    } else {
      fTmpFile = "hal_jobs";
    }

    fLogs = GetParameter("log");
    if (fLogs.enabled == false)
      fLogs.name = "";
    else
      fLogs.name = GetLogPath(fLogs.name);

    fErrors = GetParameter("error");
    if (fErrors.enabled == false)
      fErrors.name = "";
    else
      fErrors.name = GetErrorPath(fErrors.name);

    fQueue = GetParameter("queue");

    fTime = GetParameter("time");
    if (fTime.enabled) {
      auto time  = GetTime(fTime.name);
      fTime.name = GetTimeFlag(time);
    }

    fMem = GetParameter("ram");
    if (fMem.enabled) {
      TString val    = fMem.enabled;
      Double_t scale = 1;
      if (val.Contains("G")) scale = 1000;
      val       = val.ReplaceAll("M", "");
      val       = val.ReplaceAll("G", "");
      val       = val.ReplaceAll(" ", "");
      int mem   = val.Atoi();
      fMem.name = GetMemoryFlags(Form("%i", int(mem * scale)));
    }

    fExtra      = GetParameter("extra");
    fMemPerCpu  = GetParameter("ram_per_cpu");
    fTasks      = GetParameter("tasks");
    fCpuPerTask = GetParameter("cpu_per_task");
    fName       = GetParameter("name");
  }

  void JobQueue::SendCommand(Bool_t send, TString command, Int_t /*jobID*/) const {
    if (fDebugCommands) { std::cout << " DEBUG JOBS : " << command << std::endl; }
    if (send) gSystem->Exec(command);
  }

  void JobQueue::BuildExample(TString name) {


    Hal::XMLFile file(name, "create");
    file.CreateRootNode("hal-merger");
    auto& root = file.GetRootNode();
    XMLNode scheduler("scheduler", "sbatch");
    root.AddChild(scheduler);

    XMLNode aliases("aliases");
    root.AddChild(aliases);

    XMLNode parameters("parameters");

    auto vecPar = GenerateCommonParameters();
    for (auto i : vecPar) {
      parameters.AddChild(i);
    }
    root.AddChild(parameters);

    XMLNode commands("commands");
    XMLNode command("command", "root -b -q macro.C");
    commands.AddChild(command);
    root.AddChild(commands);
  }

  JobQueue::ParPair JobQueue::GetParameter(TString name) const {
    ParPair val;
    auto it = fParameters.find(name);
    if (it != fParameters.end()) {
      val.name    = it->second;
      val.enabled = true;
      return val;
    }
    val.name    = name;
    val.enabled = false;
    return val;
  }

  TString JobQueue::GetExport(TString shell, TString val) const {
    shell = shell.ReplaceAll(" ", "");
    if (shell.EndsWith("bash")) return TString("export JOB_ID_HAL=") + val;
    if (shell.EndsWith("tcsh")) return TString("setenv JOB_ID_HAL ") + val;
    if (shell.EndsWith("zsh")) return TString("export JOB_ID_HAL=") + val;
    if (shell.EndsWith("sh")) return TString("export JOB_ID_HAL=") + val;
    return "";
  }

  std::vector<XMLNode> JobQueue::GenerateCommonParameters() {
    auto addParameter = [](TString nodename, TString value) {
      XMLNode node("parameter");
      TString en = "false";
      node.AddAttribs({{"name", nodename}, {"value", value}});
      return node;
    };
    std::vector<XMLNode> res;
    res.push_back(addParameter("queue", "long"));
    res.push_back(addParameter("error", "/dev/null"));
    res.push_back(addParameter("log", "/dev/null"));
    res.push_back(addParameter("shell", "#!/bin/bash"));
    res.push_back(addParameter("start", "0"));
    res.push_back(addParameter("end", "10"));
    res.push_back(addParameter("name", ""));
    res.push_back(addParameter("array", "no"));
    res.push_back(addParameter("time", "1:2:2:22"));
    res.push_back(addParameter("ram", "10G"));
    res.push_back(addParameter("direct", "no"));
    res.push_back(addParameter("extra", "no"));
    res.push_back(addParameter("tmpfile", "hal_jobs"));
    return res;
  }

  JobQueue* JobQueue::GetInstance(TString xmlFile) {
    Hal::XMLFile file(xmlFile);
    auto root = file.GetRootNode();
    auto type = root["scheduler"];
    if (!type) {
      std::cout << "Cannot find scheduler info " << std::endl;
      return nullptr;
    }
    if (type.GetValue() == "sbatch") return new JobSbatch(xmlFile);
    if (type.GetValue() == "qsub") return new JobTorque(xmlFile);
    std::cout << "Cannot find scheduler type " << std::endl;
    return nullptr;
  }

  JobQueue::JobQueue() {}


} /* namespace Hal */
