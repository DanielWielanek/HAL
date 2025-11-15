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
#include "XMLNode.h"

#include <fstream>
#include <iostream>
#include <utility>

#include "InputDataInfo.h"
#include <TBranch.h>
#include <TBranchElement.h>
#include <TChain.h>
#include <TCollection.h>
#include <TDirectoryFile.h>
#include <TFile.h>
#include <TFriendElement.h>
#include <TKey.h>
#include <TList.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TTree.h>

namespace Hal {
  RootIOManager::RootIOManager(TString name) : IOManager(new InputRootDataInfo(name)) {};

  RootIOManager::RootIOManager(InputDataInfo* info) : IOManager(info) {}

  Bool_t RootIOManager::InitInternal() {
    fInChain = ((Hal::InputRootDataInfo*) fDataInfo)->GetChain();
    UpdateBranches();
    fEntries = fInChain->GetEntries();
    fOutFile = new TFile(fOutFileName, "recreate");
    fOutTree = new TTree(fOutTreeName, fOutTreeName);
    return kTRUE;
  }

  Int_t RootIOManager::GetEntries() const { return fEntries; }

  RootIOManager::~RootIOManager() {
    if (fInChain) delete fInChain;
    for (auto obj : fObjects) {
      if (obj) delete obj;
    }
    if (fOutFile) delete fOutFile;
  }

  void RootIOManager::UpdateBranches() {
    TObjArray* list_branch = fInChain->GetListOfBranches();
    auto branches          = GetListOfBranches(fInChain, kTRUE);
    for (auto name : branches) {
      TBranch* branch = fInChain->GetBranch(name);
      if (FindBranch(name).GetFlag() != BranchInfo::EFlag::kNull) continue;  // branch with given name already exist
      if (!branch) { Hal::Cout::PrintInfo(Form("Branch %s not found!", name.Data()), EInfo::kError); }
      TString className = branch->GetClassName();
      auto classInfo    = TClass::GetClass(className, 1);
      bool object       = false;
      if (classInfo) {
        if (classInfo->InheritsFrom("TObject")) { object = true; }
      }
      if (object) {
        TObject** obj = new TObject*();
        PushTObject(obj);
        fInChain->SetBranchAddress(name, obj);
        AddBranch(branch->GetName(), obj[0], BranchInfo::EFlag::kInPassive);
      } else {
        AddBranch(branch->GetName(), nullptr, BranchInfo::EFlag::kInPassive);
      }
    }
  }

  void RootIOManager::RegisterInternal(TString name, TString folderName, TObject* obj, Bool_t toFile) {
    if (toFile) { fOutTree->Branch(name, obj); }
  }

  void RootIOManager::SetInChain(TChain* /*tempChain*/, Int_t /*ident*/) {}

  void RootIOManager::FillTree() { fOutTree->Fill(); }

  void RootIOManager::CloseManager() {
    fOutTree->Write();
    fOutFile->Close();
  }

  Int_t Hal::RootIOManager::GetEntry(Int_t i, Int_t flag) {
    fInChain->GetEntry(i, flag);
    return 1;
  }

  void RootIOManager::PushTObject(TObject** obj) { fObjects.push_back(obj); }

  void RootIOManager::LockUnusedBranches() {
    for (auto branch : fBranches) {
      if (branch.GetFlag() == BranchInfo::EFlag::kInPassive) {
        fInChain->SetBranchStatus(branch.GetBranchName(), 0);
        HalCoutDebug(Form("Locking branch %s", branch.GetBranchName().Data()));
      }
    }
  }

  std::vector<TString> RootIOManager::GetListOfBranches(TChain* chain, Bool_t friends) {

    auto convert = [](std::vector<TString>& res, TObjArray* objar) {
      for (int i = 0; i < objar->GetEntriesFast(); i++) {
        TBranchElement* branch = (TBranchElement*) objar->At(i);
        TString name           = branch->GetName();
        res.push_back(name);
      }
    };
    std::vector<TString> list;
    TObjArray* list_branch = chain->GetListOfBranches();
    convert(list, list_branch);
    if (friends) {
      auto friends_trees = chain->GetListOfFriends();
      if (friends_trees)
        for (int i = 0; i < friends_trees->GetEntries(); i++) {  // why ROOT doesnt do this?
          auto tree  = ((TFriendElement*) friends_trees->At(i))->GetTree();
          auto lista = tree->GetListOfBranches();
          convert(list, lista);
        }
    }
    return list;
  }

  void RootIOManager::CdToOutput() {
    if (fOutFile) fOutFile->cd();
  }

} /* namespace Hal */
