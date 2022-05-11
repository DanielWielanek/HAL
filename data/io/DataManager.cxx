/*
 * HalDataManager.cxx
 *
 *  Created on: 7 wrz 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "DataManager.h"

#include "Field.h"
#include "IOManager.h"

#include <RtypesCore.h>
#include <TChain.h>
#include <TTree.h>
namespace Hal {
  DataManager::DataManager(IOManager* mngr) : fManager(mngr) {}

  DataManager* DataManager::Instance() {

    thread_local DataManager mngr;
    return &mngr;
  }

  DataManager::~DataManager() {}
  Int_t DataManager::GetEntries() const { return fManager->GetEntries(); }

  Int_t DataManager::GetEntry(Int_t i) { return fManager->GetEntry(i); }

  Bool_t DataManager::Init() { return fManager->Init(); }

  void DataManager::Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) {
    fManager->Register(name, folderName, obj, toFile);
  }

  void DataManager::Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) {
    fManager->Register(name, Foldername, obj, toFile);
  }

  void DataManager::SetInChain(TChain* tempChain, Int_t ident) { fManager->SetInChain(tempChain, ident); }

  void DataManager::UpdateBranches() { fManager->UpdateBranches(); }

  Int_t DataManager::CheckBranch(const char* BrName) { return fManager->CheckBranch(BrName); }

  TObject* DataManager::GetObject(const char* BrName) { return fManager->GetObject(BrName); }

  TFile* DataManager::GetInFile() { return fManager->GetInFile(); }

  TList* DataManager::GetBranchNameList() { return fManager->GetBranchNameList(); }

  Field* DataManager::GetField() const { return fManager->GetField(); }
  /****************************************************************************************************************************/
}  // namespace Hal
