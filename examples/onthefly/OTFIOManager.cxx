/*
 * OTFIOManager.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "OTFIOManager.h"

#include "Cout.h"

#include <TBranch.h>
#include <TFile.h>
#include <TList.h>
#include <TObjString.h>
#include <TSystem.h>
#include <TTree.h>

namespace HalOTF {

  IOManager::IOManager(TString name, Int_t entries) :
    fInFileName(name), fOutTreeName("HalTree"), fEntries(entries), fInFile(nullptr), fOutFile(nullptr), fOutTree(nullptr) {}

  Bool_t IOManager::Init() {
    Hal::Cout::PrintInfo(fInFileName, Hal::EInfo::kLowWarning);
    fInFile  = new TFile(fInFileName, "recreate");
    fOutFile = new TFile(fOutFileName, "recreate");
    fOutTree = new TTree(fOutTreeName, fOutTreeName);
    return kTRUE;
  }

  Int_t IOManager::GetEntries() const { return fEntries; }

  IOManager::~IOManager() {
    if (fInFile) delete fInFile;
    if (fOutFile) delete fOutFile;
    gSystem->Exec(Form("rm %s", fInFileName.Data()));
  }

  TObject* IOManager::GetObject(const char* BrName) {
    TObject* object = nullptr;
    for (auto branch : fInBranches) {
      TString name = branch->GetName();
      if (name.EqualTo(BrName)) { branch->SetAddress(&object); }
    }
    for (auto branch : fOutBranches) {
      TString name = branch->GetName();
      if (name.EqualTo(BrName)) { branch->SetAddress(&object); }
    }
    for (auto branch : fOutBranchesVirtual) {
      if (branch.first.EqualTo(BrName)) { object = branch.second; }
    }
    return object;
  }

  TFile* IOManager::GetInFile() { return fInFile; }

  void IOManager::UpdateBranches() {}

  void IOManager::Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) {
    Hal::Cout::PrintInfo(Form("REGISTER %s", name), Hal::EInfo::kLowWarning);
    if (toFile) {
      TBranch* br = fOutTree->Branch(name, obj);
      fOutBranches.push_back(br);
    } else {
      fOutBranchesVirtual.push_back(std::pair<TString, TObject*>(name, obj));
    }
  }

  void IOManager::Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) {
    if (toFile) {
      TBranch* br = fOutTree->Branch(name, obj);
      fOutBranches.push_back(br);
    } else {
      fOutBranchesVirtual.push_back(std::pair<TString, TObject*>(name, obj));
    }
  }

  void IOManager::SetInChain(TChain* tempChain, Int_t ident) {}

  Int_t IOManager::CheckBranch(const char* BrName) {
    for (auto branch : fInBranches) {
      TString name = branch->GetName();
      if (name.EqualTo(BrName)) { return 1; }
    }
    for (auto branch : fOutBranches) {
      TString name = branch->GetName();
      if (name.EqualTo(BrName)) { return 1; }
    }
    for (auto branch : fOutBranchesVirtual) {
      if (branch.first.EqualTo(BrName)) { return 1; }
    }
    return 0;
  }

  TList* IOManager::GetBranchNameList() {
    TList* l = new TList();
    for (auto branch : fInBranches) {
      l->AddLast(new TObjString(branch->GetName()));
    }
    for (auto branch : fOutBranches) {
      l->AddLast(new TObjString(branch->GetName()));
    }
    for (auto branch : fOutBranchesVirtual) {
      l->AddLast(new TObjString(branch.first));
    }
    return l;
  }

  Int_t IOManager::GetEntry(Int_t i) {
    if (i < fEntries) return 1;
    return -1;
  }

}  // namespace HalOTF
