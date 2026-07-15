/*
 * JobQueue.cxx
 *
 *  Created on: 16 sty 2026
 *      Author: daniel
 */

#include <TRegexp.h>
#include <TSystem.h>

#include <fstream>
#include <iostream>
#include <utility>

#include "JobQueue.h"
#include "JobSbatch.h"
#include "JobTorque.h"
#include "Std.h"
#include "StdString.h"


namespace Hal {

  const TString JobQueue::fgJobIdName      = "JOB_ID_HAL";
  const TString JobQueue::fgJobTotalNoName = "JOBS_NO_HAL";
  const TString JobQueue::fgDataset        = "DATASET_HAL";

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
    // parsing parameters
    auto& parameters = root["parameters"];
    LoadParams(parameters);

    for (auto it = fParameters.begin(); it != fParameters.end(); ++it) {
      for (auto& a : aliasarray) {
        TString from = a.first;
        TString to   = a.second;
        it->second   = it->second.ReplaceAll(Form("${%s}", from.Data()), to.Data());
      }
    }

    MakeDatasetCommands();

    if (fSource.enabled) {
      TString sourceCommand = GetSource(fSource.name);
      fCommands.insert(fCommands.begin(), sourceCommand);
    }

    if (fStartDir.enabled) {
      TString pwdCommand = GetSource(fStartDir.enabled);
      fCommands.insert(fCommands.begin(), Form("cd %s", fStartDir.name.Data()));
    }

    for (auto& i : fCommands) {
      for (auto& a : aliasarray) {
        TString from = a.first;
        TString to   = a.second;
        i            = i.ReplaceAll(Form("${%s}", from.Data()), to.Data());
      }
    }
  }

  void JobQueue::SendCommand(Bool_t send, TString command, Int_t /*jobID*/) const {
    if (fDebugCommands) { std::cout << " DEBUG JOBS : " << command << std::endl; }
    if (send) {
      TString SubmitResponse = gSystem->GetFromPipe(command);
      TRegexp reg("[0-9]+");
      TString dependency = SubmitResponse(reg);
      Int_t depId        = dependency.Atoi();
      if (fMerge.enabled && IsArray()) { gSystem->Exec(Form("hal-jobs %s --dep=%i", fMerge.name.Data(), depId)); }
    }
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

  TString JobQueue::GetExport(TString val, TString variable) const {
    TString shell    = fShell.name;
    shell            = shell.ReplaceAll(" ", "");
    auto MakeCommand = [&](TString pre, TString after) {
      return Form("%s%s%s%s", pre.Data(), variable.Data(), after.Data(), val.Data());
    };
    if (shell.EndsWith("bash")) return MakeCommand("export ", "=");
    if (shell.EndsWith("tcsh")) return MakeCommand("setenv ", " ");
    if (shell.EndsWith("zsh")) return MakeCommand("export ", "=");
    if (shell.EndsWith("sh")) return MakeCommand("export ", "=");
    if (shell.EndsWith("dash")) return MakeCommand("export ", "=");

    return "";
  }

  TString JobQueue::GetSource(TString val) const {
    TString shell = fShell.name;
    shell         = shell.ReplaceAll(" ", "");
    if (shell.EndsWith("bash")) return TString("source ") + val;
    if (shell.EndsWith("tcsh")) return TString("source ") + val;
    if (shell.EndsWith("zsh")) return TString("source ") + val;
    if (shell.EndsWith("sh")) return TString(" . ") + val;
    if (shell.EndsWith("dash")) return TString(" . ") + val;
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
    XMLNode range("parameter");
    range.AddAttribs({{"name", "range"}, {"start", "0"}, {"end", "10"}});
    res.push_back(range);
    res.push_back(addParameter("name", ""));
    res.push_back(addParameter("array", "no"));
    res.push_back(addParameter("time", "1:2:2:22"));
    res.push_back(addParameter("ram", "10G"));
    res.push_back(addParameter("direct", "no"));
    res.push_back(addParameter("extra", "no"));
    res.push_back(addParameter("tmpfile", "hal_jobs"));
    res.push_back(addParameter("merge", ""));
    res.push_back(addParameter("source", ""));
    res.push_back(addParameter("dir", "${HAL::CONST::PWD}"));
    res.push_back(addParameter("dataset", ""));
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

  void JobQueue::Submit(Bool_t submit) {
    MakeJobFiles();
    if (IsArray()) {
      TString command = MakeSubmitCommand(-1);
      SendCommand(submit, command, -1);
    } else {
      for (int i = GetStart(); i <= GetEnd(); i++) {
        TString command = MakeSubmitCommand(i);
        SendCommand(submit, command, i);
      }
    }
  }

  void JobQueue::MakeJobFiles() const {
    TString dir = GetTmpFile();
    gSystem->mkdir(dir, true);
    MakeDatasetList();
    if (IsArray()) {
      MakeJobFile(-1);
    } else {
      for (int i = GetStart(); i <= GetEnd(); i++) {
        MakeJobFile(i);
      }
    }
  }

  void JobQueue::LoadDataset(const Hal::XMLNode& node) {
    TString name = node.GetAttrib("value").GetValue();
    if (name.EndsWith(".xml")) {
      Hal::XMLFile xml(name);
      auto& root = xml.GetRootNode();

      for (int i = 0; i < root.GetNChildren(); i++) {
        auto& data  = root.GetChild(i);
        TString val = data.GetValue();
        fFiles.push_back(val);
      }
    } else if (name.EndsWith(".txt") || name.EndsWith(".list")) {
      std::ifstream in(name);
      std::string file;

      while (std::getline(in, file)) {
        if (file.empty() || file[0] == '#') continue;
        fFiles.push_back(file);
        // otwierasz file
      }
    } else {  // it's directory
      auto recursive           = node.GetAttrib("depth");
      auto extension           = node.GetAttrib("extension");
      TString extensionPattern = "root";
      if (extension) extensionPattern = extension.GetValue();
      int depth = 0;
      if (recursive) { depth = recursive.GetValue().Atoi(); }
      fFiles = Hal::Std::GetListOfFiles(name, extensionPattern, true, depth);
    }
  }

  void JobQueue::LoadStartEnd(const Hal::XMLNode& node) {
    auto start = node.GetAttrib("start");
    auto end   = node.GetAttrib("end");
    if (!start) return;
    if (!end) return;
    fStartJob = start.GetValue().Atoi();
    fEndJob   = end.GetValue().Atoi();
  }

  void JobQueue::MakeDatasetList() const {
    TString dir = GetTmpFile();
    std::ofstream file(Form("%s/dataset.txt", dir.Data()));
    if (fTrees.size() == 0)
      file << "#DATASET_HAL" << std::endl;
    else {
      file << "#DATASET_HAL";
      for (auto i : fTrees)
        file << " " << i;
      file << std::endl;
    }
    for (auto i : fFiles)
      file << i << std::endl;
    ;
    file.close();
  }

  void JobQueue::MakeDatasetCommands() {
    if (fFiles.size() == 0) return;
    TString dir               = Hal::Std::GetFullPath(GetTmpFile());
    TString datasetFile       = Form("%s/dataset.txt", dir.Data());
    datasetFile               = Hal::Std::GetFullPath(datasetFile);
    TString commandExportPath = GetExport(datasetFile, fgDataset);
    fCommands.insert(fCommands.begin(), commandExportPath);
    TString commandNJobs = GetExport(Form("%i", (fEndJob - fStartJob + 1)), fgJobTotalNoName);
    fCommands.insert(fCommands.begin(), commandNJobs);
  }

  Int_t JobQueue::GetJobId() {
    TString val = gSystem->Getenv(fgJobIdName);
    if (val.Length() == 0) return -1;
    return val.Atoi();
  }

  Int_t JobQueue::GetTotalJobs() {
    TString val = gSystem->Getenv(fgJobTotalNoName);
    if (val.Length() == 0) return -1;
    return val.Atoi();
  }

  TString JobQueue::GetDatasetPath() { return gSystem->Getenv(fgDataset); }

  void JobQueue::LoadParams(Hal::XMLNode& parameters) {
    for (int i = 0; i < parameters.GetNChildren(); i++) {
      auto& parameter  = parameters[i];
      TString atribVal = parameter.GetAttrib("value").GetValue();
      TString nodeName = parameter.GetAttrib("name").GetValue();
      if (nodeName == "dataset") {
        LoadDataset(parameter);
      } else if (nodeName == "range") {
        LoadStartEnd(parameter);
      } else {
        if (atribVal.Length() > 0 && atribVal != "no") { fParameters[parameter.GetAttrib("name").GetValue()] = atribVal; }
      }
    }
    fSource     = GetParameter("source");
    fShell      = GetParameter("shell");
    fStartDir   = GetParameter("dir");
    auto parTmp = GetParameter("tmpfile");
    if (parTmp.enabled) {
      fTmpFile = parTmp.name;
    } else {
      fTmpFile = "hal_jobs";
    }

    if (GetParameter("array").enabled) fArray = kTRUE;

    if (GetParameter("direct").enabled) fDirectCommand = kTRUE;

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
    fMerge      = GetParameter("merge");
  }

} /* namespace Hal */
