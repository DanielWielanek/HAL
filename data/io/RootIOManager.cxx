/*
 * RootIoManager.cxx
 *
 *  Created on: 6 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "RootIOManager.h"
#include <iostream>
#include <utility>

#include <TBranch.h>
#include <TChain.h>
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
  RootIOManager::RootIOManager(TString name) : fEntries(0), fOutTreeName("HalTree"), fOutFile(nullptr), fOutTree(nullptr) {
    fInFileName.push_back(name);
  }

  Bool_t RootIOManager::Init() {
    TFile* f = new TFile(fInFileName[0]);
    fInFile.push_back(f);
    TList* list       = f->GetListOfKeys();
    TString chainName = "";
    for (int i = 0; i < list->GetEntries(); i++) {
      TKey* key    = (TKey*) list->At(i);
      TString name = key->GetName();
      TObject* obj = f->Get(name);
      if (dynamic_cast<TTree*>(obj)) {
        chainName = name;
        break;
      }
    }
    if (chainName.Length() == 0) return kFALSE;
    TChain* chain = new TChain(chainName);
    for (auto file : fInFileName) {
      chain->AddFile(file);
    }
    fInChain.push_back(chain);
    TObjArray* list_branch = chain->GetListOfBranches();
    for (int i = 0; i < list_branch->GetEntries(); i++) {
      TBranch* branch = (TBranch*) list_branch->At(i);
      TString name    = branch->GetName();
      TObject** obj   = new TObject*();
      fObjects.push_back(obj);
      chain->SetBranchAddress(name, obj);
      AddBranch(branch->GetName(), obj[0], IOManager::EBranchFlag::kIn);
    }
    fEntries = fInChain[0]->GetEntries();

    fOutFile = new TFile(fOutFileName, "recreate");
    fOutTree = new TTree(fOutTreeName, fOutTreeName);
    return kTRUE;
  }

  Int_t RootIOManager::GetEntries() const { return fEntries; }

  RootIOManager::~RootIOManager() {
    for (auto file : fInChain) {
      if (file) delete file;
    }
    for (auto file : fInFile)
      if (file) delete file;
    for (auto obj : fObjects)
      delete obj;
    if (fOutFile) delete fOutFile;
  }

  TFile* RootIOManager::GetInFile() {
    if (fInFile.size() > 0)
      return fInFile[0];
    else
      return nullptr;
  }

  void RootIOManager::UpdateBranches() {}

  void RootIOManager::RegisterInternal(const char* name, const char* /*folderName*/, TNamed* obj, Bool_t toFile) {
    if (toFile) { fOutTree->Branch(name, obj); }
  }

  void RootIOManager::RegisterInternal(const char* name, const char* /*Foldername*/, TCollection* obj, Bool_t toFile) {
    if (toFile) { fOutTree->Branch(name, obj); }
  }

  void RootIOManager::SetInChain(TChain* /*tempChain*/, Int_t /*ident*/) {}

  Int_t Hal::RootIOManager::GetEntry(Int_t i) {
    fInChain[0]->GetEntry(i);
    return 1;
  }
} /* namespace Hal */
