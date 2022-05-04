/*
 * HalPackageSql.cxx
 *
 *  Created on: 28-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "HalPackageSql.h"
#include "HalAnaFile.h"
#include "HalHtmlCore.h"
#include "HalPackage.h"
#include "HalPackage2HTML.h"
#include <TCollection.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TKey.h>
#include <TList.h>
#include <TNamed.h>
#include <TObjString.h>
#include <TRegexp.h>
#include <TSQLServer.h>
#include <TSQLStatement.h>
#include <TSystem.h>
#include <fstream>
#include <vector>

#include "HalCout.h"
#include "HalParameter.h"
#include "HalStd.h"
#include "HalStdString.h"

void HalPackageSql::ExportTask(HalAnaFile* extr, Int_t file_id, Int_t task_id) {
  // export main objects ??
  TSQLStatement* stat           = fSQL->Statement("INSERT INTO Files  (ID, FILE_ID,TASK_ID, Comment, SoftVer, "
                                        "Date,DataType,ProcessedEvents,InputFIle,AnaName,Tags,PathFile) VALUES "
                                        "(?,?,?,?,?,?,?,?,?,?,?,?)");
  HalPackage* metadata          = (HalPackage*) extr->GetMainObject("HalMetadata");
  TString comment               = metadata->GetComment();
  HalParameterString* anatype   = (HalParameterString*) metadata->GetObjectByName("Analysis Name");
  HalParameterString* data      = (HalParameterString*) fRunInfo->GetObjectByName("Date");
  HalParameterString* sofwer    = (HalParameterString*) fRunInfo->GetObjectByName("Software ver");
  HalParameterString* datatype  = (HalParameterString*) metadata->GetObjectByName("DataType");
  HalParameterUInt* processed   = (HalParameterUInt*) fRunInfo->GetObjectByName("Processed_events");
  HalParameterString* inputfile = (HalParameterString*) fRunInfo->GetObjectByName("Input file");
  TString path                  = GetPath(fFilename);
  TList* list                   = (TList*) metadata->GetObjectByName("Tags");
  TString tags                  = "";
  if (list) {
    for (int j = 0; j < list->GetEntries(); j++) {
      tags += ((TObjString*) list->At(j))->GetString();
      tags += " ";
    }
  }
  stat->SetInt(0, fID);
  stat->SetInt(1, file_id);
  stat->SetInt(2, task_id);
  stat->SetString(3, comment, comment.Length());
  stat->SetString(4, sofwer->GetValue(), sofwer->GetValue().Length());
  TString data_s                = data->GetValue();
  std::vector<TString> data_arr = HalStd::ExplodeString(data_s, '-');
  Int_t year, month, day;
  year  = data_arr[0].Atoi();
  month = data_arr[1].Atoi();
  day   = data_arr[2].Atoi();

  stat->SetDate(5, year, month, day);
  stat->SetString(6, datatype->GetValue(), datatype->GetValue().Length());
  stat->SetInt(7, processed->GetValue());
  stat->SetString(8, inputfile->GetValue(), inputfile->GetValue().Length());
  stat->SetString(9, anatype->GetValue(), anatype->GetValue().Length());
  stat->SetString(10, tags, tags.Length());
  stat->SetString(11, path, path.Length());
  stat->Process();
  delete stat;
}

HalPackageSql::HalPackageSql(TString filename) {
  fFilename        = filename;
  TString www_path = HalStd::GetConfigParameter("www_path");
  if (www_path.Length() <= 0) {
    HalCout::PrintInfo("www_path not found in hal configruration file!", Hal::EInfo::kLessError);
    return;
  }
  TString datafile = Form("sqlite://%s/database.sqlite", www_path.Data());
  TString user     = HalStd::GetConfigParameter("www_path/database_user");
  TString pass     = HalStd::GetConfigParameter("www_path/database_password");
  if (user.Length() == 0) { user = "user"; }
  if (pass.Length() == 0) { pass = "password"; }
  // check if ifle exist
  TString outPath = Form("%s/database.sqlite", www_path.Data());
  std::ifstream f(outPath);
  HalHtmlCore::Instance();
  if (f.good()) {
    f.close();
    fSQL = TSQLServer::Connect(datafile, user, pass);
  } else {
    TString workdir = HalStd::GetHalrootPlus();
    f.close();
    gSystem->MakeDirectory(www_path.Data());
    gSystem->MakeDirectory(Form("%s/html_rep/", www_path.Data()));
    gSystem->CopyFile(Form("%s/html/database.sqlite", workdir.Data()), Form("%s/database.sqlite", www_path.Data()));
    gSystem->CopyFile(Form("%s/html/logo_search.gif", workdir.Data()), Form("%s/logo_search.gif", www_path.Data()));
    gSystem->CopyFile(Form("%s/html/index.php", workdir.Data()), Form("%s/index.php", www_path.Data()));
    gSystem->CopyFile(Form("%s/html/functions.php", workdir.Data()), Form("%s/functions.php", www_path.Data()));
    gSystem->CopyFile(Form("%s/html/table.css", workdir.Data()), Form("%s/table.css", www_path.Data()));
    gSystem->CopyFile(Form("%s/html/search_template.css", workdir.Data()), Form("%s/search_template.css", www_path.Data()));
    HalHtmlCore::CopyJs(www_path);
    HalHtmlCore::CopyCss(www_path);
    fSQL = TSQLServer::Connect(datafile, user, pass);
  }
  HalHtmlCore::SetMainDir(www_path.Data());
  HalAnaFile* extr    = new HalAnaFile(filename);
  TSQLStatement* stat = (TSQLStatement*) fSQL->Statement("SELECT COUNT(ID) FROM Files");
  stat->Process();
  fID = stat->GetInt(0);
  delete stat;
  stat = (TSQLStatement*) fSQL->Statement("SELECT MAX(FILE_ID) FROM Files");
  stat->Process();
  Int_t file_id = stat->GetInt(0) + 1;

  delete stat;
  TFile* file           = new TFile(filename);
  TDirectory* dir       = (TDirectory*) file->Get("HalPhysics");
  TList* keys           = dir->GetListOfKeys();
  HalPackage2HTML* html = new HalPackage2HTML(filename, Form("%s/html_rep/html_%i", www_path.Data(), file_id));
  delete html;
  fRunInfo = extr->GetMetaData();
  for (int i = 0; i < keys->GetEntries(); i++) {
    // extrack all subtasks
    TKey* key        = (TKey*) keys->At(i);
    TString key_name = key->GetName();
    HalPackage* pack = extr->GetPackage(key_name);
    if (pack == NULL) continue;
    if (extr->SwitchPackage(key_name)) {
      fRunInfo = extr->GetMetaData();
      ExportTask(extr, file_id, i);
      fID++;
    }
  }
  HalHtmlCore::ResetMainDir();
  delete extr;
}

HalPackageSql::~HalPackageSql() {
  // TODO Auto-generated destructor stub
}

TString HalPackageSql::RemoveDir(TString path) {
  TRegexp reg(".*/");
  TString tmp = path(reg);
  TString res(tmp(0, tmp.Length() - 1));
  return res;
}

TString HalPackageSql::RemoveDoubleDot(TString path) {
  TString res(path(3, path.Length() - 3));
  return res;
}

Bool_t HalPackageSql::HasDir(TString path) {
  if (path.BeginsWith("../")) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

TString HalPackageSql::GetPath(TString filename) {
  TString pwd = gSystem->Getenv("PWD");
  while (true) {
    if (HasDir(filename)) {
      filename = RemoveDoubleDot(filename);
      pwd      = RemoveDir(pwd);
    } else {
      break;
    }
  }
  TString path = pwd;
  path         = path + "/";
  path         = path + filename;
  return path;
}
