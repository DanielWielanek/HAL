/*
 * MultiDimFile.cxx
 *
 *  Created on: 22 sty 2025
 *      Author: daniel
 */

#include "MultiDimFile.h"

#include <TFile.h>
#include <TTree.h>

#include "Cout.h"
#include "MultiDimDataManager.h"
#include "Std.h"
#include "StdString.h"

namespace Hal {

  MultiDimFile::MultiDimFile(TString file, TString option, Bool_t writeParams) {
    if (Hal::Std::FindParam(option, "create") || Hal::Std::FindParam(option, "recreate")) {
      fMode        = 1;
      fFile        = new TFile(file, "recreate");
      fTree        = new TTree("data", "data");
      fDataManager = new Hal::MultiDimDataManager();
      fValues      = new std::vector<Float_t>();
      if (writeParams)
        fValues->resize(fDataManager->GetParametersNo() + 1);
      else
        fValues->resize(1);
      fTree->Branch("vec", &fValues);
    } else {
      fMode        = -1;
      fFile        = new TFile(file);
      fTree        = (TTree*) fFile->Get("data");
      fDataManager = (MultiDimDataManager*) fFile->Get("info");
      fTree->SetBranchAddress("vec", &fValues);
      fTree->GetEntry(0);
      if (fValues->size() != fDataManager->GetParametersNo() + 1) {
        Hal::Cout::PrintInfo(Form("Ooops, incompatible sizes in MultiDimFile expected %i found %i",
                                  fDataManager->GetParametersNo(),
                                  (int) fValues->size()),
                             EInfo::kError);
      }
    }
  }

  void MultiDimFile::GetEntry(Int_t i) { fTree->GetEntry(i); }

  void MultiDimFile::Fill() { fTree->Fill(); }

  void MultiDimFile::SetManager(const Hal::MultiDimDataManager& mng) {
    if (fDataManager) delete fDataManager;
    fDataManager = (Hal::MultiDimDataManager*) mng.Clone();
  }

  MultiDimFile::~MultiDimFile() {
    if (Read() && fFile) {
      fFile->Close();
      delete fFile;
    } else {
      if (fFile) {
        fTree->Write();
        fDataManager->Write("info");
        delete fFile;
      }
    }
  }

  Int_t MultiDimFile::GetEntries() const { return fTree->GetEntriesFast(); }

} /* namespace Hal */
