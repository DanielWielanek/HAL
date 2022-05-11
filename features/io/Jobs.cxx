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
    XMLNode* root     = file.GetRootNode();
    XMLNode* settings = root->GetChild("settings");
    XMLNode* options  = root->GetChild("job_options");
    XMLNode* commands = root->GetChild("job_commands");
    XMLNode* submit   = settings->GetChild("submit");
    XMLNode* shell    = settings->GetChild("shell");
    fSubmitCommand    = submit->GetValue();
    fSubmitCommand.ReplaceAll("${HAL_PWD}", gSystem->pwd());
    TString Shell;
    if (shell != nullptr)
      Shell = shell->GetValue();
    else
      Shell = "";
    fCommands.push_back(Shell);
    XMLNode* start_id = settings->GetChild("submit_start");
    XMLNode* end_id   = settings->GetChild("submit_end");

    fStartJob = start_id->GetValue().Atoi();
    fEndJob   = end_id->GetValue().Atoi();
    fSubmitCommand.ReplaceAll("${HAL_START_ARRAY}", Form("%i", fStartJob));
    fSubmitCommand.ReplaceAll("${HAL_END_ARRAY}", Form("%i", fEndJob));
    if (options->GetAttrib("array") != nullptr)
      if (options->GetAttrib("array")->GetValue() == "yes") { fArray = kTRUE; }

    for (int i = 0; i < options->GetNChildren(); i++) {
      XMLNode* command = options->GetChild(i);
      fCommands.push_back(command->GetValue());
    }

    for (int i = 0; i < commands->GetNChildren(); i++) {
      XMLNode* command = commands->GetChild(i);
      fCommands.push_back(command->GetValue());
    }

    TString pwd = gSystem->pwd();
    for (unsigned int i = 0; i < fCommands.size(); i++) {
      fCommands[i].ReplaceAll("${HAL_START_ARRAY}", Form("%i", fStartJob));
      fCommands[i].ReplaceAll("${HAL_END_ARRAY}", Form("%i", fEndJob));
      fCommands[i].ReplaceAll("${HAL_PWD}", pwd);
    }
  }

  void Jobs::SubmitJobs() {
    if (fSubmitCommand.Contains("${HAL_JOB_FILE}")) {
      if (fArray) {
        TString submit = fSubmitCommand;
        submit.ReplaceAll("${HAL_JOB_FILE}", "jobs/job_array");
        if (fDebugCommands) {
          Cout::Text("Sending array", "M", kWhite);
          Cout::Text(submit);
        }
        gSystem->Exec(submit);
      } else {
        if (fDebugCommands) { Cout::Text(Form("Sending files %i - %i", fStartJob, fEndJob), "M", kWhite); }
        for (int i = fStartJob; i <= fEndJob; i++) {
          TString submit = fSubmitCommand;
          submit.ReplaceAll("${HAL_JOB_FILE}", Form("jobs/job_%i", i));
          if (fDebugCommands) { Cout::Text(submit); }
          gSystem->Exec(submit);
        }
      }
    } else {
      Cout::Text("NO ${HAL_JOB_FILE} trying to guess command pattern", "L", kYellow);
      if (fArray) {
        gSystem->Exec(Form("%s jobs/job_array", fSubmitCommand.Data()));
      } else {
        for (int i = fStartJob; i <= fEndJob; i++) {
          gSystem->Exec(Form("%s jobs/job_%i", fSubmitCommand.Data(), i));
        }
      }
    }
  }

  void Jobs::CreateJobs() {
    if (fArray) {
      std::ofstream job_array;
      job_array.open("jobs/job_array");
      for (unsigned int i = 0; i < fCommands.size(); i++)
        job_array << fCommands[i] << std::endl;
      job_array.close();
      gSystem->Exec("chmod a+x jobs/job_array");
    } else {
      for (int iJob = fStartJob; iJob <= fEndJob; iJob++) {
        std::ofstream job_file;
        job_file.open(Form("jobs/job_%i", iJob));
        for (unsigned int i = 0; i < fCommands.size(); i++) {
          TString commands = fCommands[i];
          commands.ReplaceAll("${HAL_JOB_ID}", Form("%i", iJob));
          job_file << commands << std::endl;
        }
        job_file.close();
        gSystem->Exec(Form("chmod a+x jobs/job_%i", iJob));
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
