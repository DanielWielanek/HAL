/*
 * RootIoManager.cxx
 *
 *  Created on: 6 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "RootIOManager.h"
#include <utility>

#include <TBranch.h>
#include <TCollection.h>
#include <TDirectoryFile.h>
#include <TFile.h>
#include <TKey.h>
#include <TList.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TTree.h>

namespace Hal {
  RootIOManager::RootIOManager(TString name) :
    fInFileName(name), fOutTreeName("HalTree"), fInFile(nullptr), fOutFile(nullptr), fInTree(nullptr), fOutTree(nullptr) {}
  Bool_t RootIOManager::Init() {
    fInFile     = new TFile(fInFileName);
    TList* list = fInFile->GetListOfKeys();
    for (int i = 0; i < list->GetEntries(); i++) {
      TKey* key    = (TKey*) list->At(i);
      TString name = key->GetName();
      TObject* obj = fInFile->Get(name);
      fInTree      = dynamic_cast<TTree*>(obj);
      if (fInTree) break;  // found ttree
    }
    if (fInTree == nullptr) return kFALSE;
    TObjArray* list_branch = fInTree->GetListOfBranches();
    for (int i = 0; i < list_branch->GetEntries(); i++) {
      TBranch* branch = (TBranch*) list_branch->At(i);
      fInBranches.push_back(branch);
    }

    fOutFile = new TFile(fOutFileName, "recreate");
    fOutTree = new TTree(fOutTreeName, fOutTreeName);
    return kTRUE;
  }

  Int_t RootIOManager::GetEntries() const {
    if (fInTree) return fInTree->GetEntries();
    return -1;
  }

  RootIOManager::~RootIOManager() {
    if (fInFile) delete fInFile;
  }
  TObject* RootIOManager::GetObject(const char* BrName) {
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

  TFile* RootIOManager::GetInFile() { return fInFile; }

  void RootIOManager::UpdateBranches() {}

  void RootIOManager::Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) {
    if (toFile) {
      TBranch* br = fOutTree->Branch(name, obj);
      fOutBranches.push_back(br);
    } else {
      fOutBranchesVirtual.push_back(std::pair<TString, TObject*>(name, obj));
    }
  }

  void RootIOManager::Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) {
    if (toFile) {
      TBranch* br = fOutTree->Branch(name, obj);
      fOutBranches.push_back(br);
    } else {
      fOutBranchesVirtual.push_back(std::pair<TString, TObject*>(name, obj));
    }
  }

  void RootIOManager::SetInChain(TChain* tempChain, Int_t ident) {}

  Int_t RootIOManager::CheckBranch(const char* BrName) {
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

  TList* RootIOManager::GetBranchNameList() {
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

  Int_t Hal::RootIOManager::GetEntry(Int_t i) {
    fInTree->GetEntry(i);
    return 1;
  }
} /* namespace Hal */
