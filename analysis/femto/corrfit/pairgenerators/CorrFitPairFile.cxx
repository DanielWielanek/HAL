/*
 * CorrFitPairFile.cxx
 *
 *  Created on: 22 sty 2025
 *      Author: daniel
 */

#include "CorrFitPairFile.h"

#include "CorrFitMapGroupConfig.h"
#include "Cout.h"
#include "StdString.h"

#include <TChain.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>

namespace Hal {

  CorrFitPairFile::CorrFitPairFile(TString fileName, TString mode) {
    fFileName = fileName;
    if (Hal::Std::FindParam(mode, "create") || Hal::Std::FindParam(mode, "recreate")) {
      fMode = 1;
    } else {
      fMode = -1;
    }
  }

  Bool_t CorrFitPairFile::Init(Int_t startBranch, Int_t endBranch, Bool_t signal, Bool_t background) {
    if (fMode == 1) { return InitWrite(); }
    if (fMode == -1) { return InitRead(startBranch, endBranch, signal, background); }
    return kFALSE;
  }

  void CorrFitPairFile::SetConfig(const CorrFitMapGroupConfig& config) { fConfig = (CorrFitMapGroupConfig*) config.Clone(); }

  Bool_t CorrFitPairFile::InitRead(Int_t startBranch       = -1,
                                   Int_t endBranch         = -1,
                                   Bool_t enableSignal     = kTRUE,
                                   Bool_t enableBackground = kFALSE) {
    std::vector<TString> files;
    if (IsVirtual()) return kTRUE;
    if (fFileName.EndsWith("root")) {
      files.push_back(fFileName);
    } else {
      auto vec = Hal::Std::GetLinesFromFile(fFileName);
      files    = vec;
    }
    fFile = new TFile(files[0]);
    fTree = new TChain("HalTree");
    for (auto file : files)
      ((TChain*) fTree)->AddFile(file);
    fConfig = (CorrFitMapGroupConfig*) fFile->Get("HalInfo/CorrFitMapGroup");
    if (!fConfig) {
      Hal::Cout::PrintInfo("Cannot init CorrFitPairFile in read mode without config!", EInfo::kError);
      return kFALSE;
    }
    Int_t bins    = fConfig->GetNbins();
    int start_bin = 0;
    int end_bin   = bins;
    if (startBranch != -1) {
      start_bin = TMath::Min(TMath::Max(0, startBranch), bins - 1);
      end_bin   = TMath::Min(TMath::Max(0, endBranch), bins - 1);
    }
    if (fConfig->HaveBackground()) {
      for (int idx = 0; idx < bins; idx++)
        fTree->SetBranchStatus(Form("FemtoBackground_%i", idx), 0);
      if (enableBackground)
        for (int idx = start_bin; idx < end_bin; idx++) {
          fBackground.push_back(new TClonesArray("Hal::FemtoMicroPair", 100));
          fTree->SetBranchStatus(Form("FemtoBackground_%i", idx), 1);
          fTree->SetBranchAddress(Form("FemtoBackground_%i", idx), &*fBackground[idx]);
        }
    }
    if (fConfig->HaveSignal()) {
      for (int idx = 0; idx < bins; idx++)
        fTree->SetBranchStatus(Form("FemtoSignal_%i", idx), 0);
      if (enableSignal)
        for (int idx = start_bin; idx < end_bin; idx++) {
          fTree->SetBranchStatus(Form("FemtoSignal_%i", idx), 1);
          fSignals.push_back(new TClonesArray("Hal::FemtoMicroPair", 100));
          fTree->SetBranchAddress(Form("FemtoSignal_%i", idx), &*fSignals[idx]);
        }
    }
    return kTRUE;
  }

  Bool_t CorrFitPairFile::InitWrite() {
    fFile = new TFile(fFileName, "recreate");
    fTree = new TTree("HalTree", "HalTree");
    if (!fConfig) {
      Hal::Cout::PrintInfo("Cannot init CorrFitPairFile in write mode without config!", EInfo::kError);
      return kFALSE;
    }
    Int_t bins = fConfig->GetNbins();
    if (fConfig->HaveBackground()) {
      for (int idx = 0; idx < bins; idx++) {
        fBackground.push_back(new TClonesArray("Hal::FemtoMicroPair", 100));
        fTree->Branch(Form("FemtoBackground_%i", idx), &*fBackground[idx]);
      }
    }
    if (fConfig->HaveSignal()) {
      for (int idx = 0; idx < bins; idx++) {
        fSignals.push_back(new TClonesArray("Hal::FemtoMicroPair", 100));
        fTree->Branch(Form("FemtoSignal_%i", idx), &*fSignals[idx]);
      }
    }
    return kTRUE;
  }

  void CorrFitPairFile::ClearData() {
    if (fSignals.size()) {
      for (auto i : fSignals)
        i->Clear();
    }
    if (fBackground.size()) {
      for (auto i : fBackground)
        i->Clear();
    }
  }

  Int_t CorrFitPairFile::GetEntries() const { return fTree->GetEntriesFast(); }

  CorrFitPairFile::~CorrFitPairFile() {
    if (fMode == 1 && fFile) {
      fTree->Write();
      fFile->Close();
      delete fFile;
    } else {
      if (fFile) delete fFile;
    }
  }

  void CorrFitPairFile::GetEntry(Int_t i) { fTree->GetEntry(i, 0); }

  void CorrFitPairFile::Fill() { fTree->Fill(); }

  Bool_t CorrFitPairFile::IsVirtual() const {
    if (fFileName == "virtual") return kTRUE;
    return kFALSE;
  }

} /* namespace Hal */
