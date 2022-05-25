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
namespace Hal {
  namespace Fair {
    TObject* RootManager::GetObject(const char* BrName) { return FairRootManager::Instance()->GetObject(BrName); }

    TFile* RootManager::GetInFile() { return FairRootManager::Instance()->GetInFile(); }

    void RootManager::UpdateBranches() { FairRootManager::Instance()->UpdateBranches(); }

    void RootManager::Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) {
      FairRootManager::Instance()->Register(name, folderName, obj, toFile);
    }

    void RootManager::Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) {
      FairRootManager::Instance()->Register(name, Foldername, obj, toFile);
    }

    void RootManager::SetInChain(TChain* tempChain, Int_t ident) {
      FairRootManager::Instance()->SetInChain(tempChain, ident);
    }

    Int_t RootManager::CheckBranch(const char* BrName) { return FairRootManager::Instance()->CheckBranch(BrName); }

    TList* RootManager::GetBranchNameList() { return FairRootManager::Instance()->GetBranchNameList(); }
  }  // namespace Fair
}  // namespace Hal
