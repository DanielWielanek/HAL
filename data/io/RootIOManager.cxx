/*
 * RootIoManager.cxx
 *
 *  Created on: 6 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "RootIOManager.h"

#include "Cout.h"

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
    SetInputName(name);
  }

  TString RootIOManager::GetChain(TFile* file) const {
    TList* list       = file->GetListOfKeys();
    TString chainName = "";
    for (int i = 0; i < list->GetEntries(); i++) {
      TKey* key    = (TKey*) list->At(i);
      TString name = key->GetName();
      TObject* obj = file->Get(name);
      if (dynamic_cast<TTree*>(obj)) {
        chainName = name;
        break;
      }
    }
    return chainName;
  }

  Bool_t RootIOManager::Init() {
    SetInputName(fInFileName[0]);
    std::vector<TString> chainNames;
    TFile* f = new TFile(fInFileName[0]);
    chainNames.push_back(GetChain(f));  // get chain name from main files
    f->Close();
    if (chainNames[0].Length() == 0) return kFALSE;
    TChain* mainChain = new TChain(chainNames[0]);
    for (auto file : fInFileName) {
      mainChain->AddFile(file);  // add files to chain
    }

    fInChain.push_back(mainChain);
    for (auto& fileFriend : fFriendName) {  // loop over friends
      TFile* ff         = new TFile(fileFriend[0]);
      TString chainName = GetChain(ff);
      chainNames.push_back(chainName);
      TChain* friendChain = new TChain(chainName);
      fInChain.push_back(friendChain);
      for (auto singleFileFriend : fileFriend) {
        friendChain->AddFile(singleFileFriend);
      }
      ff->Close();
    }

    Long64_t ent1 = fInChain[0]->GetEntries();
    for (auto friendChain : fInChain) {
      if (ent1 != friendChain->GetEntries()) { Cout::PrintInfo("Different number of entries in chains", EInfo::kWarning); }
    }

    for (auto chain : fInChain) {
      TObjArray* list_branch = chain->GetListOfBranches();
      for (int i = 0; i < list_branch->GetEntries(); i++) {
        TBranch* branch = (TBranch*) list_branch->At(i);
        TString name    = branch->GetName();
        TObject** obj   = new TObject*();
        fObjects.push_back(obj);
        chain->SetBranchAddress(name, obj);
        AddBranch(branch->GetName(), obj[0], IOManager::EBranchFlag::kIn);
      }
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

  void RootIOManager::AddFriend(TString friendName, Int_t level) {
    if (level < 0) return;
    if (int(fFriendName.size()) <= level) fFriendName.resize(level + 1);
    fFriendName[level].push_back(friendName);
  }

  void RootIOManager::UpdateBranches() {}

  void RootIOManager::RegisterInternal(const char* name, const char* /*folderName*/, TNamed* obj, Bool_t toFile) {
    if (toFile) { fOutTree->Branch(name, obj); }
  }

  void RootIOManager::RegisterInternal(const char* name, const char* /*Foldername*/, TCollection* obj, Bool_t toFile) {
    if (toFile) { fOutTree->Branch(name, obj); }
  }

  void RootIOManager::SetInChain(TChain* /*tempChain*/, Int_t /*ident*/) {}

  void RootIOManager::FillTree() { fOutTree->Fill(); }

  void RootIOManager::CloseManager() {
    fOutTree->Write();
    fOutFile->Close();
  }

  Int_t Hal::RootIOManager::GetEntry(Int_t i) {
    for (auto chain : fInChain) {
      chain->GetEntry(i);
    }
    return 1;
  }
} /* namespace Hal */
