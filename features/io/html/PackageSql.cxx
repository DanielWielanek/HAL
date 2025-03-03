/*
 * HalPackageSql.cxx
 *
 *  Created on: 28-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "PackageSql.h"
#include "AnaFile.h"
#include "Cout.h"
#include "HtmlCore.h"
#include "Package.h"
#include "Package2HTML.h"
#include "Parameter.h"

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

#include "Std.h"
#include "StdString.h"

namespace Hal {
  void PackageSql::ExportTask(AnaFile* extr, Int_t file_id, Int_t task_id) {
    // export main objects ??
    TSQLStatement* stat        = fSQL->Statement("INSERT INTO Files  (ID, FILE_ID,TASK_ID, Comment, SoftVer, "
                                                 "Date,DataType,ProcessedEvents,InputFIle,AnaName,Tags,PathFile) VALUES "
                                                 "(?,?,?,?,?,?,?,?,?,?,?,?)");
    Package* metadata          = (Package*) extr->GetMainObject("HalMetadata");
    TString comment            = metadata->GetComment();
    ParameterString* anatype   = (ParameterString*) metadata->GetObjectByName("Analysis Name");
    ParameterString* data      = (ParameterString*) fRunInfo->GetObjectByName("Date");
    ParameterString* sofwer    = (ParameterString*) fRunInfo->GetObjectByName("Software ver");
    ParameterString* datatype  = (ParameterString*) metadata->GetObjectByName("DataType");
    ParameterUInt* processed   = (ParameterUInt*) fRunInfo->GetObjectByName("Processed_events");
    ParameterString* inputfile = (ParameterString*) fRunInfo->GetObjectByName("Input file");
    TString path               = GetPath(fFilename);
    TList* list                = (TList*) metadata->GetObjectByName("Tags");
    TString tags               = "";
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
    std::vector<TString> data_arr = Hal::Std::ExplodeString(data_s, '-');
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

  PackageSql::PackageSql(TString filename) {
    fFilename        = filename;
    TString www_path = Hal::Std::GetConfigParameter("www_path");
    if (www_path.Length() <= 0) {
      Cout::PrintInfo("www_path not found in hal configruration file!", Hal::EInfo::kError);
      return;
    }
    TString datafile = Form("sqlite://%s/database.sqlite", www_path.Data());
    TString user     = Hal::Std::GetConfigParameter("www_path/database_user");
    TString pass     = Hal::Std::GetConfigParameter("www_path/database_password");
    if (user.Length() == 0) { user = "user"; }
    if (pass.Length() == 0) { pass = "password"; }
    // check if ifle exist
    TString outPath = Form("%s/database.sqlite", www_path.Data());
    std::ifstream f(outPath);
    HtmlCore::Instance();
    if (f.good()) {
      f.close();
      fSQL = TSQLServer::Connect(datafile, user, pass);
    } else {
      TString workdir = Hal::Std::GetHalrootPlus();
      f.close();
      gSystem->MakeDirectory(www_path.Data());
      gSystem->MakeDirectory(Form("%s/html_rep/", www_path.Data()));
      gSystem->CopyFile(Form("%s/html/database.sqlite", workdir.Data()), Form("%s/database.sqlite", www_path.Data()));
      gSystem->CopyFile(Form("%s/html/logo_search.gif", workdir.Data()), Form("%s/logo_search.gif", www_path.Data()));
      gSystem->CopyFile(Form("%s/html/index.php", workdir.Data()), Form("%s/index.php", www_path.Data()));
      gSystem->CopyFile(Form("%s/html/functions.php", workdir.Data()), Form("%s/functions.php", www_path.Data()));
      gSystem->CopyFile(Form("%s/html/table.css", workdir.Data()), Form("%s/table.css", www_path.Data()));
      gSystem->CopyFile(Form("%s/html/search_template.css", workdir.Data()), Form("%s/search_template.css", www_path.Data()));
      HtmlCore::CopyJs(www_path);
      HtmlCore::CopyCss(www_path);
      fSQL = TSQLServer::Connect(datafile, user, pass);
    }
    HtmlCore::SetMainDir(www_path.Data());
    AnaFile* extr       = new AnaFile(filename);
    TSQLStatement* stat = (TSQLStatement*) fSQL->Statement("SELECT COUNT(ID) FROM Files");
    stat->Process();
    fID = stat->GetInt(0);
    delete stat;
    stat = (TSQLStatement*) fSQL->Statement("SELECT MAX(FILE_ID) FROM Files");
    stat->Process();
    Int_t file_id = stat->GetInt(0) + 1;

    delete stat;
    TFile* file        = new TFile(filename);
    TDirectory* dir    = (TDirectory*) file->Get("HalPhysics");
    TList* keys        = dir->GetListOfKeys();
    Package2HTML* html = new Package2HTML(filename, Form("%s/html_rep/html_%i", www_path.Data(), file_id));
    delete html;
    fRunInfo = extr->GetMetaData();
    for (int i = 0; i < keys->GetEntries(); i++) {
      // extrack all subtasks
      TKey* key        = (TKey*) keys->At(i);
      TString key_name = key->GetName();
      Package* pack    = extr->GetPackage(key_name);
      if (pack == NULL) continue;
      if (extr->SwitchPackage(key_name)) {
        fRunInfo = extr->GetMetaData();
        ExportTask(extr, file_id, i);
        fID++;
      }
    }
    HtmlCore::ResetMainDir();
    delete extr;
  }

  PackageSql::~PackageSql() {}

  TString PackageSql::RemoveDir(TString path) {
    TRegexp reg(".*/");
    TString tmp = path(reg);
    TString res(tmp(0, tmp.Length() - 1));
    return res;
  }

  TString PackageSql::RemoveDoubleDot(TString path) {
    TString res(path(3, path.Length() - 3));
    return res;
  }

  Bool_t PackageSql::HasDir(TString path) {
    if (path.BeginsWith("../")) {
      return kTRUE;
    } else {
      return kFALSE;
    }
  }

  TString PackageSql::GetPath(TString filename) {
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
}  // namespace Hal
