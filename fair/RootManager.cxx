/*
 * HalFairRootManager2.cxx
 *
 *  Created on: 5 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "RootManager.h"

#include <FairRootManager.h>
#include <RtypesCore.h>
#include <TCollection.h>
#include <TFile.h>
#include <TList.h>
#include <TObjString.h>

namespace Hal {
  namespace Fair {
    RootManager::RootManager() {
      fFairManager = FairRootManager::Instance();
      if (fFairManager->GetInFile()) { SetInputName(fFairManager->GetInFile()->GetName()); }
    }

    TObject* RootManager::GetObject(const char* BrName) { return fFairManager->GetObject(BrName); }

    TFile* RootManager::GetInFile() { return fFairManager->GetInFile(); }

    void RootManager::UpdateBranches() { fFairManager->UpdateBranches(); }

    void RootManager::SetInChain(TChain* tempChain, Int_t ident) { fFairManager->SetInChain(tempChain, ident); }

    void RootManager::RegisterInternal(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) {
      fFairManager->Register(name, folderName, obj, toFile);
    }

    void RootManager::RegisterInternal(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) {
      fFairManager->Register(name, Foldername, obj, toFile);
    }

    void RootManager::RefreshBranchList() {
      fFairManager = FairRootManager::Instance();
      TList* l     = fFairManager->GetBranchNameList();
      for (int i = 0; i < l->GetEntries(); i++) {
        TString name = ((TObjString*) l->At(i))->GetString();
        Int_t stat   = fFairManager->CheckBranch(name);
        TObject* obj = fFairManager->GetObject(name);
        switch (stat) {
          case 0:  // not exist
            break;
          case 1:  // in file
            AddBranch(name, obj, EBranchFlag::kIn);
            break;
          case 2:  // memory
            AddBranch(name, obj, EBranchFlag::kVirtual);
            break;
        }
      }
    }

    Bool_t RootManager::Init() {
      if (fFairManager == nullptr) {
        fFairManager = FairRootManager::Instance();
        if (fFairManager->GetInFile()) { SetInputName(fFairManager->GetInFile()->GetName()); }
      }
      RefreshBranchList();
      return kTRUE;
    }
  }  // namespace Fair
}  // namespace Hal
