/*
 * mpd_merger.cpp
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Cout.h"
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
Bool_t CheckPackagesList(TList* list) {
  for (int i = 0; i < list->GetEntries() / 2; i++) {
    if (list->At(i * 2)->InheritsFrom("HalPackage") == kFALSE) { return kFALSE; }
  }
  return kTRUE;
}
Bool_t CheckAndMergeList(TList* prim, TList* sec) {
  if (prim->GetEntries() != sec->GetEntries()) {
    Hal::Cout::PrintInfo("Prim !=Sec", Hal::EInfo::kLessError);
    return kFALSE;
  }
  for (int i = 0; i < prim->GetEntries() / 2; i++) {
    TString name_a              = ((TObjString*) prim->At(i * 2 + 1))->GetString();
    TString name_b              = ((TObjString*) sec->At(i * 2 + 1))->GetString();
    Bool_t first_can_be_merged  = prim->At(i * 2)->InheritsFrom("HalPackage");
    Bool_t second_can_be_merged = sec->At(i * 2)->InheritsFrom("HalPackage");
    if (name_a.EqualTo(name_b)) {
    } else {
      Hal::Cout::PrintInfo("Names not equal", Hal::EInfo::kLessError);
    }
    if (first_can_be_merged == kFALSE) {
      Hal::Cout::PrintInfo(Form("First object cannot be merged (%s)", name_a.Data()), Hal::EInfo::kLessError);
    }
    if (second_can_be_merged == kFALSE) {
      Hal::Cout::PrintInfo(Form("Second object cannot be merged (%s)", name_b.Data()), Hal::EInfo::kLessError);
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
  if (dir == NULL) {
    if (must_be == kTRUE) {
      Hal::Cout::PrintInfo(Form("%s directory not found in file", dir_name.Data()), Hal::EInfo::kImportantError);
    } else {
      Hal::Cout::PrintInfo(Form("%s directory not found in file", dir_name.Data()), Hal::EInfo::kLessError);
    }
    return NULL;
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

Int_t mergeAnaFiles(TString filename, TFile* file1, TString outFile, TList* list_files) {
  TList *prim_ana = nullptr, *sec_ana = nullptr;
  TList *prim_meta = nullptr, *sec_meta = nullptr;
  prim_ana  = GetListObjects(file1, "HalPhysics", kTRUE);
  prim_meta = GetListObjects(file1, "HalInfo", kFALSE);
  if (prim_ana)
    if (!CheckPackagesList(prim_ana)) {
      Hal::Cout::PrintInfo("HalPhysics contain class that cannot be merged by hal-merger", Hal::EInfo::kImportantError);
      return 0;
    }
  if (prim_meta)
    if (!CheckPackagesList(prim_meta)) {
      Hal::Cout::PrintInfo("HalInfo contain classes that cannot be merged by hal-merger", Hal::EInfo::kLessError);
      return 0;
    }
  file1->Close();
  delete file1;
  if (prim_ana == NULL || prim_meta == NULL) {
    if (prim_ana) delete prim_ana;
    if (prim_meta) delete prim_meta;
    Hal::Cout::FailSucced(Form("Opening primary file: %s ", filename.Data()), " FAIL ", kRed);
    return 0;
  } else {
    Hal::Cout::FailSucced(Form("Opening primary file: %s ", filename.Data()), " OK  ", kGreen);
  }

  for (int i = 1; i < list_files->GetEntries(); i++) {
    filename    = ((TObjString*) list_files->At(i))->GetString();
    TFile* file = new TFile(filename);
    if (file->IsZombie()) {
      Hal::Cout::FailSucced(Form("Zombie file %s", filename.Data()), "FATAL", kRed);
      file->Close();
      delete file;
      continue;
    }
    sec_ana  = GetListObjects(file, "HalPhysics", kTRUE);
    sec_meta = GetListObjects(file, "HalInfo", kFALSE);
    if (sec_ana == NULL) {
      Hal::Cout::PrintInfo(Form("No HalPhysics in %s", filename.Data()), Hal::EInfo::kLessError);
      Hal::Cout::FailSucced(Form("Error opening file %s", filename.Data()), "FATAL", kRed);
      file->Close();
      delete file;
      if (sec_meta) {
        delete sec_meta;
        sec_meta = NULL;
      }
      continue;
    }
    if (sec_meta == NULL) {
      Hal::Cout::PrintInfo(Form("No HalInfo in %s", filename.Data()), Hal::EInfo::kLessError);
      Hal::Cout::FailSucced(Form("Error opening file %s", filename.Data()), "FATAL", kRed);
      file->Close();
      delete file;
      if (sec_ana) {
        delete sec_ana;
        sec_ana = NULL;
      }
      continue;
    }
    if (CheckAndMergeList(prim_ana, sec_ana) == kFALSE) {
      Hal::Cout::PrintInfo("Cannot merge HalPhysisc directory, incompatible objects", Hal::EInfo::kLessError);
      Hal::Cout::FailSucced(Form("Error opening file %s", filename.Data()), "FATAL", kRed);
      file->Close();
      delete file;
      if (sec_ana) delete sec_ana;
      if (sec_meta) delete sec_meta;
      continue;
    }
    if (CheckAndMergeList(prim_meta, sec_meta) == kFALSE) {
      Hal::Cout::PrintInfo("Cannot merge HalInfo directory, incompatible objects", Hal::EInfo::kLessError);
      Hal::Cout::FailSucced(Form("Error opening file %s", filename.Data()), "FATAL", kRed);
      file->Close();
      delete file;
      if (sec_ana) {
        delete sec_ana;
        sec_ana = NULL;
      }
      if (sec_meta) {
        delete sec_meta;
        sec_meta = NULL;
      }
      continue;
    }
    if (sec_ana) delete sec_ana;
    if (sec_meta) delete sec_meta;
    Hal::Cout::FailSucced(Form("Closing added file: %s ", filename.Data()), " OK  ", kGreen);
    file->Close();
    delete file;
  }
  Hal::Cout::Text(Form("Saving file: %s", outFile.Data()), "L", kWhite);
  TFile* file = new TFile(outFile, "recreate");
  file->cd();
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
  file->Close();
  return 0;
}
Int_t mergeQAFiles(TString filename, TFile* file1, TString outFile, TList* list_files) {
  TList* histos = GetListObjects(file1, "HalQA", kTRUE);
  file1->Close();
  delete file1;
  for (int i = 1; i < list_files->GetEntries(); i++) {
    filename    = ((TObjString*) list_files->At(i))->GetString();
    TFile* file = new TFile(filename);
    if (file->IsZombie()) {
      Hal::Cout::FailSucced(Form("Zombie file %s", filename.Data()), "FATAL", kRed);
      file->Close();
      delete file;
      continue;
    }
    TList* histos2 = GetListObjects(file, "HalQA", kTRUE);
    CheckAndMergeList(histos, histos2);
    delete histos2;
  }
  TFile* file = new TFile(outFile, "recreate");
  file->cd();
  file->mkdir("HalQA");
  file->cd("HalQA");
  for (int i = 0; i < histos->GetEntries() / 2; i++) {
    histos->At(i * 2)->Write(((TObjString*) histos->At(i * 2 + 1))->GetString());
  }
  file->Close();
  delete file;
  return 1;
}

Int_t mergeFiles(TString outFile, TList* list_files) {
  TString filename = ((TObjString*) list_files->At(0))->GetString();
  TFile* file1     = new TFile(filename);
  TObject* qa      = file1->Get("HalQA");
  if (qa != nullptr) {
    return mergeQAFiles(filename, file1, outFile, list_files);
  } else {
    return mergeAnaFiles(filename, file1, outFile, list_files);
  }
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
int main(int argc, char* argv[]) {
  if (argc < 2) {
    Hal::Cout::PrintInfo("No arguments", Hal::EInfo::kLessError);
    Hal::Cout::Text("You can use following arguments : ", "M");
    Hal::Cout::Text("1) name of xml file (with .xml extension) with list of files to merge", "M");
    Hal::Cout::Text("2) name of output root file and input root files (like for hadd: "
                    "out.root input1.root input2.root ...)",
                    "M");
    Hal::Cout::Text("3) --n=X  + one of options above to specify maximum number of files "
                    "merged at once",
                    "M");
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
    }
  }
  gSystem->Load("libTree");
  gErrorIgnoreLevel = kError;
  Hal::Cout::Stars(kWhite);

  TList* list_files = new TList();
  TString outputFile;
  TString argname = argv[1];
  Int_t split     = 0;
  Int_t arg_pos   = 2;
  if (argname.BeginsWith("--n=")) {
    TString val(argname(4, argname.Length() - 4));
    argname = argv[2];
    split   = val.Atoi();
    arg_pos = 3;
  }
  if (argname.EndsWith(".xml")) {
    Hal::Cout::Text(Form("Opening XML file %s", argname.Data()), "L", kWhite);
    Hal::XMLFile parser(argname);
    Hal::XMLNode* root = parser.GetRootNode();
    outputFile         = root->GetAttrib("outfile")->GetValue();
    for (int i = 0; i < root->GetNChildren(); i++) {
      list_files->AddLast(new TObjString(root->GetChild(i)->GetValue()));
    }
  } else if (argname.EndsWith(".root") || argname.EndsWith(".root_t")) {
    Hal::Cout::Text("Using direct list of root files", "L", kWhite);
    outputFile = argname;
    for (int i = arg_pos; i < argc; i++) {
      list_files->AddLast(new TObjString(argv[i]));
    }
  } else {
    Hal::Cout::Text("wrong file extension !", "M", kRed);
    return 0;
  }
  if (split < 2 || split >= list_files->GetEntries()) {
    mergeFiles(outputFile, list_files);
  } else {
    --split;
    Int_t entries = list_files->GetEntries();
    Int_t nSteps  = entries / split;
    if (entries % split != 0) nSteps++;
    TString file[2] = {"temp_1.root_t", "temp_2.root_t"};
    for (int iStep = 0; iStep < nSteps; iStep++) {
      Int_t record  = iStep * split;
      Int_t j       = 0;
      TString name  = "";
      TString space = " ";
      while (record + j < entries && j < split) {
        name = name + space + ((TObjString*) list_files->At(record + j))->GetString();
        j++;
      }
      TString outFile = file[iStep % 2];
      if (iStep > 0) { name = Form("%s  %s", name.Data(), (file[(iStep + 1) % 2]).Data()); }
      if (iStep + 1 == nSteps) { outFile = argname; }
      gSystem->Exec(Form("hal-merger %s %s", outFile.Data(), name.Data()));
      if (iStep + 1 == nSteps) {  // clean up temp files if present
        if (Hal::Std::FileExists(file[0])) gSystem->Exec("rm temp_1.root_t ");
        if (Hal::Std::FileExists(file[1])) gSystem->Exec("rm temp_2.root_t ");
      }
    }
  }
  delete list_files;

  return 0;
}
