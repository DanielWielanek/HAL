/*
 * VirtualIOManager2.cxx
 *
 *  Created on: 19 gru 2024
 *      Author: daniel
 */

#include "VirtualIOManager.h"

#include <TFile.h>
#include <TTree.h>

#include "Cout.h"
#include "InputDataInfo.h"
#include "Std.h"
#include "VirtualSource.h"

namespace Hal {
  VirtualIOManager::VirtualIOManager(VirtualSource* source, Int_t events) :
    Hal::IOManager(new Hal::InputDataInfo("/dev/null")),
    fInFileName("/dev/null"),
    fOutTreeName("HalTree"),
    fEntries(events),
    fInFile(nullptr),
    fOutFile(nullptr),
    fOutTree(nullptr),
    fSource(source) {}

  Bool_t VirtualIOManager::InitInternal() {
    Hal::Cout::PrintInfo("IO manager internal init", Hal::EInfo::kDebugInfo);
    Hal::Cout::PrintInfo(fInFileName, Hal::EInfo::kDebugInfo);
    fInFile  = new TFile(fInFileName, "recreate");
    fOutFile = new TFile(fOutFileName, "recreate");
    fOutTree = new TTree(fOutTreeName, fOutTreeName);
    Hal::Cout::PrintInfo(Form("Creating output tree %s", fOutTreeName.Data()), Hal::EInfo::kDebugInfo);
    fSource->RegisterInputs();
    return kTRUE;
  }

  Int_t VirtualIOManager::GetEntries() const { return fEntries; }

  void VirtualIOManager::RegisterInternal(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) {
    if (toFile) { fOutTree->Branch(name, obj); }
  }

  void VirtualIOManager::RegisterInternal(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) {
    if (toFile) { fOutTree->Branch(name, obj); }
  }

  Int_t VirtualIOManager::GetEntry(Int_t i, Int_t flag) {
    if (i < fEntries) {
      if (fSource) fSource->GetEvent(i, flag);
      return 1;
    }
    return 0;
  }

  TFile* VirtualIOManager::GetInFile() { return fInFile; }

  void VirtualIOManager::SetInChain(TChain* /*tempChain*/, Int_t /*ident*/) {}

  void VirtualIOManager::FillTree() { fOutTree->Fill(); }

  void VirtualIOManager::CloseManager() {
    fOutTree->Write();
    fOutFile->Close();
  }

  VirtualIOManager::~VirtualIOManager() {
    if (fOutFile) delete fOutFile;
    if (fInFile) delete fInFile;
  }

  void VirtualIOManager::CdToOutput() {
    if (fOutFile) fOutFile->cd();
  }

} /* namespace Hal */
