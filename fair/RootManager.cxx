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
      FairRootManager* mngr = FairRootManager::Instance();
      if (mngr->GetInFile()) { SetInputName(mngr->GetInFile()->GetName()); }
    }
    TObject* RootManager::GetObject(const char* BrName) { return FairRootManager::Instance()->GetObject(BrName); }

    TFile* RootManager::GetInFile() { return FairRootManager::Instance()->GetInFile(); }

    void RootManager::UpdateBranches() { FairRootManager::Instance()->UpdateBranches(); }

    void RootManager::SetInChain(TChain* tempChain, Int_t ident) { FairRootManager::Instance()->SetInChain(tempChain, ident); }

    void RootManager::RegisterInternal(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) {
      FairRootManager::Instance()->Register(name, folderName, obj, toFile);
    }

    void RootManager::RegisterInternal(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) {
      FairRootManager::Instance()->Register(name, Foldername, obj, toFile);
    }

    void RootManager::RefreshBranchList() {
      TList* l = FairRootManager::Instance()->GetBranchNameList();
      if (l->GetEntries() > fList->GetEntries()) {
        for (int i = 0; i < l->GetEntries(); i++) {
          TObjString* name = (TObjString*) l->At(i);
          Bool_t found     = kFALSE;
          for (int j = 0; j < fList->GetEntries(); j++) {
            TObjString* name2 = (TObjString*) fList->At(j);
            if (name->GetString().EqualTo(name2->GetString())) found = kTRUE;
          }
          if (!found) { fList->AddLast(new TObjString(*name)); }
        }
      }
    }
  }  // namespace Fair
}  // namespace Hal
