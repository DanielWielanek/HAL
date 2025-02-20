/*
 * DbgIOManager.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "DbgIOManager.h"

#include "Cout.h"
#include "InputDataInfo.h"

#include <TBranch.h>
#include <TFile.h>
#include <TList.h>
#include <TObjString.h>
#include <TSystem.h>
#include <TTree.h>

namespace HalDbg {

  IOManager::IOManager(TString name, Int_t entries) :
    Hal::IOManager(new Hal::InputDataInfo(name)),
    fInFileName(name),
    fOutTreeName("HalTree"),
    fEntries(entries),
    fInFile(nullptr),
    fOutFile(nullptr),
    fOutTree(nullptr) {}

  Bool_t IOManager::InitInternal() {
    Hal::Cout::PrintInfo(fInFileName, Hal::EInfo::kLowWarning);
    fInFile  = new TFile(fInFileName, "recreate");
    fOutFile = new TFile(fOutFileName, "recreate");
    fOutTree = new TTree(fOutTreeName, fOutTreeName);
    Hal::Cout::PrintInfo(Form("CREATING TREE %s", fOutTreeName.Data()), Hal::EInfo::kError);
    return kTRUE;
  }

  Int_t IOManager::GetEntries() const { return fEntries; }

  IOManager::~IOManager() {
    if (fInFile) delete fInFile;
    if (fOutFile) delete fOutFile;
    gSystem->Exec(Form("rm %s", fInFileName.Data()));
  }

  TFile* IOManager::GetInFile() { return fInFile; }

  void IOManager::RegisterInternal(const char* name, const char* /*folderName*/, TNamed* obj, Bool_t toFile) {
    if (toFile) { fOutTree->Branch(name, obj); }
  }

  void IOManager::RegisterInternal(const char* name, const char* /*Foldername*/, TCollection* obj, Bool_t toFile) {
    if (toFile) { fOutTree->Branch(name, obj); }
  }

  void IOManager::SetInChain(TChain* /*tempChain*/, Int_t /*ident*/) {}

  Int_t IOManager::GetEntry(Int_t i, Int_t /*flag*/) {
    if (i < fEntries) return 1;
    return -1;
  }

  void IOManager::FillTree() { fOutTree->Fill(); }

  void IOManager::CloseManager() {
    fOutTree->Write();
    fOutFile->Close();
  }

}  // namespace HalDbg
