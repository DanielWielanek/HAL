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
  RootIOManager::RootIOManager(TString name) : IOManager(new InputRootDataInfo(name)) {};

  RootIOManager::RootIOManager(InputDataInfo* info) : IOManager(info) {}

  Bool_t RootIOManager::InitInternal() {
    std::vector<TString> chainNames;
    fInChain = ((Hal::InputRootDataInfo*) fDataInfo)->GetChain();
    FillBranches();
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

  Int_t Hal::RootIOManager::GetEntry(Int_t i, Int_t flag) {
    fInChain->GetEntry(i, flag);
    return 1;
  }

  void RootIOManager::PushTObject(TObject** obj) { fObjects.push_back(obj); }

  void RootIOManager::FillBranches() {
    TObjArray* list_branch = fInChain->GetListOfBranches();
    for (int i = 0; i < list_branch->GetEntries(); i++) {
      TBranch* branch   = (TBranch*) list_branch->At(i);
      TString name      = branch->GetName();
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

  void RootIOManager::LockUnusedBranches() {
    for (auto branch : fBranches) {
      if (branch.GetFlag() == BranchInfo::EFlag::kInPassive) {
        std::cout << "LOCK Branch " << branch.GetBranchName() << std::endl;
        fInChain->SetBranchStatus(branch.GetBranchName(), 0);
      }
    }
  }

} /* namespace Hal */
