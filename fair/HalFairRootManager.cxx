/*
 * HalFairRootManager2.cxx
 *
 *  Created on: 5 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "HalFairRootManager.h"

#include <FairRootManager.h>
#include <RtypesCore.h>
namespace Hal {
  TObject* HalFairRootManager::GetObject(const char* BrName) { return FairRootManager::Instance()->GetObject(BrName); }

  TFile* HalFairRootManager::GetInFile() { return FairRootManager::Instance()->GetInFile(); }

  void HalFairRootManager::UpdateBranches() { FairRootManager::Instance()->UpdateBranches(); }

  void HalFairRootManager::Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) {
    FairRootManager::Instance()->Register(name, folderName, obj, toFile);
  }

  void HalFairRootManager::Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) {
    FairRootManager::Instance()->Register(name, Foldername, obj, toFile);
  }

  void HalFairRootManager::SetInChain(TChain* tempChain, Int_t ident) {
    FairRootManager::Instance()->SetInChain(tempChain, ident);
  }

  Int_t HalFairRootManager::CheckBranch(const char* BrName) { return FairRootManager::Instance()->CheckBranch(BrName); }

  TList* HalFairRootManager::GetBranchNameList() { return FairRootManager::Instance()->GetBranchNameList(); }
}  // namespace Hal
