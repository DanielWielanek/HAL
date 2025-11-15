/*
 * mpd_merger.cpp
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Cout.h"
#include "Options.h"
#include "Package.h"
#include "Std.h"
#include "XMLNode.h"

#include <TCollection.h>
#include <TDirectory.h>
#include <TError.h>
#include <TFile.h>
#include <TKey.h>
#include <TList.h>
#include <TNamed.h>
#include <TObjString.h>
#include <TObject.h>
#include <TString.h>
#include <TSystem.h>

#include <iostream>


/** \addtogroup hal-merger
 *
 * application to merging the results of analisis
 * @param list
 * @return
 * Works in three modes:
 * - \b hal-merger \b xml_file the xml file contains the list of files to
 * merger, the root node specify the "outpufile" in argument "outfile", daugher
 * nodes contains the names of files to merge
 * - \b hal-merger \b outfile \b infiles - like in hadd command where first
 * argument specify the input file, and later list of input files is passed
 * - \b hal-merger \b --n=X \b and \b option \b above - in this case user
 * specify how many files is merged by a single instance of this application,
 * useful to merging large datasets where memory leaks lead to problems
 */
Bool_t gSkipBadFiles;
Bool_t CheckPackagesList(TList* list) {
  for (int i = 0; i < list->GetEntries() / 2; i++) {
    if (list->At(i * 2)->InheritsFrom("Hal::Package") == kFALSE) { return kFALSE; }
  }
  return kTRUE;
}
Bool_t CheckAndMergeList(TList* prim, TList* sec) {
  if (prim->GetEntries() != sec->GetEntries()) {
    Hal::Cout::PrintInfo("Prim !=Sec", Hal::EInfo::kError);
    return kFALSE;
  }
  for (int i = 0; i < prim->GetEntries() / 2; i++) {
    TString name_a              = ((TObjString*) prim->At(i * 2 + 1))->GetString();
    TString name_b              = ((TObjString*) sec->At(i * 2 + 1))->GetString();
    Bool_t first_can_be_merged  = prim->At(i * 2)->InheritsFrom("Hal::Package");
    Bool_t second_can_be_merged = sec->At(i * 2)->InheritsFrom("Hal::Package");
    if (name_a.EqualTo(name_b)) {
    } else {
      Hal::Cout::PrintInfo("Names not equal", Hal::EInfo::kError);
    }
    if (first_can_be_merged == kFALSE) {
      Hal::Cout::PrintInfo(Form("First object cannot be merged (%s)", name_a.Data()), Hal::EInfo::kError);
    }
    if (second_can_be_merged == kFALSE) {
      Hal::Cout::PrintInfo(Form("Second object cannot be merged (%s)", name_b.Data()), Hal::EInfo::kError);
    }
    if (!name_a.EqualTo(name_b) || !first_can_be_merged || !second_can_be_merged) {
      delete sec;
      return kFALSE;
    }
  }
  for (int i = 0; i < prim->GetEntries() / 2; i++) {
    Hal::Package* A = (Hal::Package*) prim->At(i * 2);
    Hal::Package* B = (Hal::Package*) sec->At(i * 2);
    A->Add(B);
  }
  return kTRUE;
}

TList* GetListObjects(TFile* file, TString dir_name, Bool_t must_be) {
  TDirectory* dir = (TDirectory*) file->Get(dir_name);
  if (dir == nullptr) {
    if (must_be == kTRUE) {
      Hal::Cout::PrintInfo(Form("%s directory not found in file", dir_name.Data()), Hal::EInfo::kDebugInfo);
    } else {
      Hal::Cout::PrintInfo(Form("%s directory not found in file", dir_name.Data()), Hal::EInfo::kDebugInfo);
    }
    return nullptr;
  } else {
    TList* key_lists = dir->GetListOfKeys();
    key_lists->SetOwner(kTRUE);
    TList* objects = new TList();
    objects->SetOwner(kTRUE);
    for (int i = 0; i < key_lists->GetEntries(); i++) {
      TKey* key = (TKey*) key_lists->At(i);
      objects->AddAt(file->Get(Form("%s/%s", dir_name.Data(), key->GetName())), i * 2);
      objects->AddAt(new TObjString(key->GetName()), i * 2 + 1);
    }
    dir->DeleteAll();
    delete dir;
    return objects;
  }
}

std::pair<TList*, TList*> OpenFirstFile(TString inFile) {
  std::pair<TList*, TList*> res;
  TList* prim_ana  = nullptr;
  TList* prim_meta = nullptr;
  TFile* firstFile = new TFile(inFile);
  prim_ana         = GetListObjects(firstFile, "HalPhysics", kTRUE);
  prim_meta        = GetListObjects(firstFile, "HalInfo", kFALSE);
  if (!prim_ana) {
    Hal::Cout::FailSucced(Form("Opening primary file: %s ", inFile.Data()), " FAIL ", kRed);
    return res;
  }
  if (!prim_meta) {
    Hal::Cout::FailSucced(Form("Opening primary file: %s ", inFile.Data()), " FAIL ", kRed);
    return res;
  }
  if (!CheckPackagesList(prim_ana)) {
    Hal::Cout::PrintInfo("HalPhysics contains class that cannot be merged by hal-merger", Hal::EInfo::kCriticalError);
    return res;
  }
  if (!CheckPackagesList(prim_meta)) {
    Hal::Cout::PrintInfo("HalInfo contains classes that cannot be merged by hal-merger", Hal::EInfo::kCriticalError);
    return res;
  }
  firstFile->Close();
  delete firstFile;
  res.first  = prim_ana;
  res.second = prim_meta;
  Hal::Cout::FailSucced(Form("Opening primary file: %s ", inFile.Data()), " OK  ", kGreen);
  return res;
}

Int_t mergeAnaFiles(TString filename, std::pair<TList*, TList*> res, TString outFile, std::vector<TString>& list_files) {
  TList* prim_ana  = res.first;
  TList* prim_meta = res.second;
  if (list_files.size() == 0) Hal::Cout::PrintInfo("No good files to merge by hal-merger", Hal::EInfo::kCriticalError);
  TList* sec_ana  = nullptr;
  TList* sec_meta = nullptr;
  TFile* file     = nullptr;
  auto clean      = [&]() {
    if (sec_ana) delete sec_ana;
    if (sec_meta) delete sec_meta;
    if (file) {
      file->Close();
      delete file;
    }
    sec_ana  = nullptr;
    sec_meta = nullptr;
    file     = nullptr;
  };

  auto printError = [&](TString message) {
    Hal::Cout::FailSucced(Form("Error opening file %s (%s)", filename.Data(), message.Data()), "FATAL", kRed);
    clean();
    if (!gSkipBadFiles) { Hal::Cout::PrintInfo("Critical error", Hal::EInfo::kCriticalError); }
  };

  for (int i = 1; i < list_files.size(); i++) {
    filename = list_files[i];
    file     = new TFile(filename);
    if (file->IsZombie()) {
      Hal::Cout::FailSucced(Form("Zombie file %s", filename.Data()), "FATAL", kRed);
      clean();
      continue;
    }
    sec_ana  = GetListObjects(file, "HalPhysics", kFALSE);
    sec_meta = GetListObjects(file, "HalInfo", kFALSE);
    if (!sec_ana) {
      printError("No HalPhysics");
      continue;
    }
    if (!sec_meta) {
      printError("No HalInfo");
      continue;
    }
    if (CheckAndMergeList(prim_ana, sec_ana) == kFALSE) {
      printError("Incompatible HalPhysics");
      continue;
    }
    if (CheckAndMergeList(prim_meta, sec_meta) == kFALSE) {
      printError("Incompatible HalInfo");
      continue;
    }
    Hal::Cout::FailSucced(Form("Closing added file: %s ", filename.Data()), " OK  ", kGreen);
    clean();
  }
  Hal::Cout::Text(Form("Saving file: %s", outFile.Data()), "L", kWhite);
  TFile* fileOut = new TFile(outFile, "recreate");
  fileOut->cd();
  TDirectory* d = gDirectory;
  d->mkdir("HalInfo");
  d->cd("HalInfo");
  for (int i = 0; i < prim_meta->GetEntries() / 2; i++) {
    prim_meta->At(i * 2)->Write(((TObjString*) prim_meta->At(i * 2 + 1))->GetString());
  }
  d->mkdir("HalPhysics");
  d->cd("HalPhysics");
  for (int i = 0; i < prim_ana->GetEntries() / 2; i++) {
    prim_ana->At(i * 2)->Write(((TObjString*) prim_ana->At(i * 2 + 1))->GetString());
  }
  fileOut->Close();
  return 0;
}

Int_t mergeFiles(TString outFile, std::vector<TString>& list_files) {
  TString filename = list_files[0];
  auto firstFile   = OpenFirstFile(filename);
  while (firstFile.first == nullptr) {
    if (gSkipBadFiles)
      list_files.erase(list_files.begin());
    else { Hal::Cout::PrintInfo(Form("Cannot open file %s", filename.Data()), Hal::EInfo::kCriticalError); }
    if (list_files.size() == 0) { Hal::Cout::PrintInfo("Cannot find any good file", Hal::EInfo::kCriticalError); }
    firstFile = OpenFirstFile(list_files[0]);
  }
  return mergeAnaFiles(filename, firstFile, outFile, list_files);
}
/**
 * Application for merging root files. This application don't merge all
 * structures in tree but only those in HalPhysics and HalInfo. User give one
 * argument with is name of xml file with list of filest to merge. Such xml file
 * should look like this: <hal-merger
 * outfile="name_of_file_with_merged_data.root">
 * <file>name_of_first_file.root</file>
 * <file>name_of_second_file.root</file>
 * <.. ..>
 * </hal-merger>
 * @param argc
 * @param argv
 * @return
 */

std::vector<std::vector<TString>> Split(std::vector<TString> vec, Int_t chunk_size) {
  std::vector<std::vector<TString>> bunches;
  for (int i = 0; i < vec.size(); i += chunk_size) {
    int last = TMath::Min((int) vec.size(), i + chunk_size);
    std::vector<TString> chunk;
    for (int j = i; j < last; ++j) {
      chunk.push_back((TString) vec[j]);
    }
    bunches.push_back(chunk);  // prosto, bez emplace_back
  }
  return bunches;
}

TString Merge(std::vector<TString>& vec) {
  if (vec.size() == 1) {
    return vec[0];
  } else {
    TString res;
    for (auto s : vec) {
      res = res + s + " ";
    }
    auto x = res[res.Length() - 1];
    //  if (x == ' ') res = res.Chop();  // remove last
    return res;
  }
  return "";
}

int main(int argc, char* argv[]) {
  gSkipBadFiles = kTRUE;
  if (argc < 2) {
    Hal::Cout::PrintInfo("No arguments use --help", Hal::EInfo::kError);
    return 0;
  } else {
    TString opt = argv[1];
    if (opt == "--help") {
      Hal::Cout::Text("You can use following arguments : ", "M");
      Hal::Cout::Text("1) name of xml file (with .xml extension) with list of files to "
                      "merge",
                      "M");
      Hal::Cout::Text("2) name of output root file and input root files (like for hadd: "
                      "out.root input1.root input2.root ...)",
                      "M");
      Hal::Cout::Text("3) --n=X  + one of options above to specify maximum number of files "
                      "merged at once",
                      "M");
      Hal::Cout::Text("4) --noskip  do not stop when found faulty files", "M");
      Hal::Cout::Text("4) --debug  enable debuggin info", "M");
    }
  }
  gSystem->Load("libTree");
  gErrorIgnoreLevel = kError;
  Hal::Cout::Stars(kWhite);

  std::vector<TString> list_files;
  Hal::MainOption opt(argc, argv);
  TString outputFile;
  TString argname = argv[1];
  Int_t split     = 0;
  Int_t nCores    = 0;
  if (opt.GetArguments().size() == 0) {
    Hal::Cout::Text("Cannot find list of files !", "M", kRed);
    return 0;
  }
  TString file_pattern = opt.GetArguments()[0];

  if (file_pattern.EndsWith(".xml")) {
    Hal::Cout::Text(Form("Opening XML file %s", argname.Data()), "L", kWhite);
    Hal::XMLFile parser(argname);
    Hal::XMLNode* root = parser.GetRootNode();
    outputFile         = root->GetAttrib("outfile")->GetValue();
    for (int i = 0; i < root->GetNChildren(); i++) {
      list_files.push_back(root->GetChild(i)->GetValue());
    }
  } else if (file_pattern.EndsWith(".root") || file_pattern.EndsWith(".root_t")) {
    Hal::Cout::Text("Using direct list of root files", "L", kWhite);
    for (unsigned int i = 1; i < opt.GetArguments().size(); i++) {
      list_files.push_back(opt.GetArguments()[i]);
    }
    outputFile = opt.GetArguments()[0];
  } else {
    Hal::Cout::Text("wrong file extension !", "M", kRed);
    return 0;
  }
  if (opt.HaveParameter("n")) { split = opt.GetParameterValue("n").Atoi(); }
  if (opt.HaveParameter("noskip")) gSkipBadFiles = kFALSE;
  if (opt.HaveParameter("cores")) nCores = opt.GetParameterValue("cores").Atoi();
  Int_t hidden = 0;
  if (opt.HaveParameter("hidden")) hidden = opt.GetParameterValue("hidden").Atoi();
  if (opt.HaveParameter("debug")) Hal::Cout::SetVerboseMode(Hal::EInfo::kDebugInfo);

  if (split < 2 || split >= list_files.size()) {
    mergeFiles(outputFile, list_files);
  } else if (nCores <= 1) {
    auto lists = Split(list_files, split - 1);
    std::pair<TString, TString> pair;
    pair.first  = Form("temp_2_%i.root_t", hidden);
    pair.second = Form("temp_1_%i.root_t", hidden);
    bool first  = true;
    for (auto sublist : lists) {
      auto merged = Merge(sublist);
      if (!first) merged = merged + " " + pair.first;
      if (Hal::Std::FileExists(pair.second)) { gSystem->Exec(Form("rm %s", pair.second.Data())); }
      gSystem->Exec(Form("hal-merger %s %s", pair.second.Data(), merged.Data()));
      auto temp   = pair.first;
      pair.first  = pair.second;
      pair.second = temp;
      first       = false;
    }
    if (Hal::Std::FileExists(pair.second)) gSystem->Exec(Form("rm %s", pair.second.Data()));
    gSystem->Exec(Form("mv %s %s", pair.first.Data(), outputFile.Data()));
  } else {
    // TODO wielowatkowey merge
  }

  return 0;
}
