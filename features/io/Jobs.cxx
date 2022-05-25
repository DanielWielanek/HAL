/*
 * HalJobs.cxx
 *
 *  Created on: 6 maj 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Jobs.h"

#include "Cout.h"
#include "XMLNode.h"
#include <TSystem.h>
#include <fstream>
#include <iostream>

namespace Hal {
  Jobs::Jobs(TString name) : fArray(kFALSE), fDebugCommands(kFALSE), fFile(name), fStartJob(1), fEndJob(1) {
    XMLFile file(fFile);
    XMLNode* root = file.GetRootNode();

    XMLNode* settings = root->GetChild("settings");
    fSubmitCommand    = settings->GetAttrib("submit")->GetValue();

    TString Shell = settings->GetAttrib("shell")->GetValue();
    TString array = settings->GetAttrib("array")->GetValue();
    if (settings->GetAttrib("debug")) {
      if (settings->GetAttrib("debug")->GetValue().EqualTo("yes")) fDebugCommands = kTRUE;
    }
    array.ToLower();
    if (array.EqualTo("yes")) fArray = kTRUE;
    fStartJob = settings->GetAttrib("start")->GetValue().Atoi();
    fEndJob   = settings->GetAttrib("end")->GetValue().Atoi();
    fDir      = settings->GetAttrib("dir")->GetValue();
    gSystem->mkdir(fDir);

    XMLNode* parameters = root->GetChild("parameters");
    if (parameters) {
      const Int_t nPar = parameters->GetNChildren("parameter");
      for (int i = 0; i < nPar; i++) {
        XMLNode* param   = parameters->GetChild("parameter", i);
        TString parname  = param->GetAttrib("name")->GetValue();
        TString parvalue = param->GetAttrib("value")->GetValue();
        fParameters.push_back(std::pair<TString, TString>(parname, parvalue));
      }
    }
    fParameters.push_back(std::pair<TString, TString>("HAL::CONST::PWD", gSystem->pwd()));
    fParameters.push_back(std::pair<TString, TString>("HAL::CONST::START", Form("%i", fStartJob)));
    fParameters.push_back(std::pair<TString, TString>("HAL::CONST::END", Form("%i", fEndJob)));

    fCommands.push_back(Shell);
    XMLNode* commands     = root->GetChild("commands");
    const Int_t nCommands = commands->GetNChildren("command");
    for (int i = 0; i < nCommands; i++) {
      XMLNode* command = commands->GetChild("command", i);
      fCommands.push_back(command->GetValue());
    }


    for (auto parameter : fParameters) {
      TString patternToReplace = Form("${%s}", parameter.first.Data());

      for (auto& command : fCommands) {
        command.ReplaceAll(patternToReplace, parameter.second);
      }
      fSubmitCommand.ReplaceAll(patternToReplace, parameter.second);
    }
  }

  void Jobs::SubmitJobs() {
    if (fSubmitCommand.Contains("${HAL::CONST::JOB_FILE}")) {
      if (fArray) {
        TString submit = fSubmitCommand;
        submit.ReplaceAll("${HAL::CONST::JOB_FILE}", Form("%s/job_array", fDir.Data()));
        if (fDebugCommands) {
          Cout::Text("Sending array", "M", kWhite);
          Cout::Text(submit);
        }
        gSystem->Exec(submit);
      } else {
        if (fDebugCommands) { Cout::Text(Form("Sending files %i - %i", fStartJob, fEndJob), "M", kWhite); }
        for (int i = fStartJob; i <= fEndJob; i++) {
          TString submit = fSubmitCommand;
          submit.ReplaceAll("${HAL::CONST::JOB_FILE}", Form("%s/job_%i", fDir.Data(), i));
          if (fDebugCommands) { Cout::Text(submit); }
          gSystem->Exec(submit);
        }
      }
    } else {
      Cout::Text("NO ${HAL::CONST::JOB_FILE} trying to guess command pattern", "L", kYellow);
      if (fArray) {
        gSystem->Exec(Form("%s %s/job_array", fDir.Data(), fSubmitCommand.Data()));
      } else {
        for (int i = fStartJob; i <= fEndJob; i++) {
          gSystem->Exec(Form("%s %s/job_%i", fDir.Data(), fSubmitCommand.Data(), i));
        }
      }
    }
  }

  void Jobs::CreateJobs() {
    if (fArray) {
      std::ofstream job_array;
      job_array.open(Form("%s/job_array", fDir.Data()));
      for (unsigned int i = 0; i < fCommands.size(); i++)
        job_array << fCommands[i] << std::endl;
      job_array.close();
      gSystem->Exec(Form("chmod a+x %s/job_array", fDir.Data()));
    } else {
      for (int iJob = fStartJob; iJob <= fEndJob; iJob++) {
        std::ofstream job_file;
        job_file.open(Form("%s/job_%i", fDir.Data(), iJob));
        for (unsigned int i = 0; i < fCommands.size(); i++) {
          TString commands = fCommands[i];
          commands.ReplaceAll("${HAL::CONST::JOB_ID}", Form("%i", iJob));
          job_file << commands << std::endl;
        }
        job_file.close();
        gSystem->Exec(Form("chmod a+x %s/job_%i", fDir.Data(), iJob));
      }
    }
  }

  Jobs::~Jobs() {}

  Int_t Jobs::GetNVariables(TString textfile) {
    if (textfile.EndsWith(".xml")) { return GetNVariablesXML(textfile); }
    return GetNVariablesTxt(textfile);
  }

  TString Jobs::GetParameter(TString textfile, Int_t job_no, Int_t var_no) {
    if (textfile.EndsWith(".xml")) return GetParameterXml(textfile, job_no, var_no);
    return GetParameterTxt(textfile, job_no, var_no);
  }

  void Jobs::CreateDummyTxtFile(TString textfile, Int_t jobs, Int_t vars) {
    std::ofstream file;
    file.open(textfile);
    file << "=======_HAL_JOBS_INPUT_FILE_=======" << std::endl;
    file << "NVAR " << vars << std::endl;
    file << "NJOBS " << jobs << std::endl;
    for (int i = 0; i < jobs; i++) {
      for (int j = 0; j < vars; j++) {
        file << Form("job_%i_var_%i", i, j) << " ";
      }
      file << std::endl;
    }
    file.close();
  }

  Int_t Jobs::GetNVariablesTxt(TString textfile) {
    std::ifstream file;
    file.open(textfile);
    if (file.good()) {
      int pars, jobs;
      TString dummy;
      file >> dummy;
      file >> dummy >> pars;
      if (dummy != "NVAR") {
        std::cout << "HalJobs::CreateJobsTxt failed to find Nvar: that specify "
                     "number of variables"
                  << std::endl;
      }
      file >> dummy >> jobs;
      if (dummy != "NJOBS") {
        std::cout << "HalJobs::CreateJobsTxt failed to find Njobs that specify "
                     "number of variables"
                  << std::endl;
      }
      file.close();
      return pars;
    } else {
      std::cout << "HalJobs::GetNVariable failed to find txt file" << std::endl;
      file.close();
      return 0;
    }
  }

  Int_t Jobs::GetNVariablesXML(TString xmlfile) {
    XMLFile file(xmlfile);
    XMLNode* root = file.GetRootNode();
    return root->GetChild(0)->GetNAttributes();
  }

  TString Jobs::GetParameterTxt(TString textfile, Int_t job, Int_t var) {
    Int_t vals = GetNVariables(textfile);
    if (vals == 0) return "";
    std::ifstream file;
    file.open(textfile);
    int pars, jobs;
    TString dummy;
    file >> dummy;
    file >> dummy >> pars;
    file >> dummy >> jobs;
    if (job > jobs) return "";
    if (var > pars) return "";
    for (int i = 0; i < job; i++) {
      for (int j = 0; j < pars; j++) {
        file >> dummy;
      }
    }
    for (int j = 0; j <= var; j++)
      file >> dummy;
    file.close();
    return dummy;
  }

  TString Jobs::GetParameterXml(TString xmlfile, Int_t job, Int_t var) {
    XMLFile p(xmlfile);
    XMLNode* root = p.GetRootNode();
    XMLNode* nod  = root->GetChild(job);
    TString val   = nod->GetAttrib(var)->GetValue();
    return val;
  }

  void Jobs::CreateDummyXMLFile(TString xmlfile, Int_t jobs, Int_t vars) {
    XMLFile file(xmlfile, "write");
    file.CreateRootNode("jobs");
    XMLNode* node = file.GetRootNode();
    for (int iJobs = 0; iJobs < jobs; iJobs++) {
      XMLNode* job = new XMLNode("job", Form("job%iJobs", iJobs));
      for (int iAttrib = 0; iAttrib < vars; iAttrib++) {
        job->AddAttrib(new XMLAttrib(Form("par_%i", iAttrib), Form("job_%i_var_%i", iJobs, iAttrib)));
      }
      node->AddChild(job);
    }
    file.Close();
  }
}  // namespace Hal
