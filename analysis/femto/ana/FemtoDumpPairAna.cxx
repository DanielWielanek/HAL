/*
 * FemtoDumpPairAna.cxx
 *
 *  Created on: 9 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoDumpPairAna.h"


#include "Cout.h"
#include "DataManager.h"
#include "DividedHisto.h"
#include "FemtoCorrFunc.h"
#include "FemtoPair.h"
#include "MemoryMapManager.h"
#include "ObjectMatrix.h"
#include "TwoTrackAna.h"

#include <TAxis.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TH1.h>
#include <TObjArray.h>


namespace Hal {
  FemtoDumpPairAna::FemtoDumpPairAna() : fBinLimit(10000), fLimitsN(), fLimitsD(), fWriteBackground(kFALSE) {}

  void FemtoDumpPairAna::ProcessFemtoPair() {
    fFemtoPair->Compute();
    Int_t bin = fGrouping.GetBin(fFemtoPair);
    if (bin < 0) return;
    if (fLimitsN[bin] < fBinLimit) {
      FemtoMicroPair* mini = (FemtoMicroPair*) fSignalPairs[bin]->ConstructedAt(fSignalPairs[bin]->GetEntriesFast());
      *mini                = *fFemtoPair;
      fLimitsN[bin]++;
    }
  }

  void FemtoDumpPairAna::ProcessFemtoPair_Rotated() {
    fFemtoPair->Compute_Rotated();
    Int_t bin = fGrouping.GetBin(fFemtoPair);
    if (bin < 0) return;
    if (fLimitsN[bin] < fBinLimit) {
      FemtoMicroPair* mini = (FemtoMicroPair*) fBackgroundPairs[bin]->ConstructedAt(fBackgroundPairs[bin]->GetEntriesFast());
      *mini                = *fFemtoPair;
      fLimitsD[bin]++;
    }
  }

  void FemtoDumpPairAna::ProcessFemtoPair_Hemisphere() {
    fFemtoPair->Compute_Hemisphere();
    Int_t bin = fGrouping.GetBin(fFemtoPair);
    if (bin < 0) return;
    if (fLimitsN[bin] < fBinLimit) {
      FemtoMicroPair* mini = (FemtoMicroPair*) fBackgroundPairs[bin]->ConstructedAt(fBackgroundPairs[bin]->GetEntriesFast());
      *mini                = *fFemtoPair;
      fLimitsD[bin]++;
    }
  }

  void FemtoDumpPairAna::ProcessFemtoPair_Mixed() {
    fFemtoPair->Compute();
    Int_t bin = fGrouping.GetBin(fFemtoPair);
    if (bin < 0) return;
    if (fLimitsN[bin] < fBinLimit) {
      FemtoMicroPair* mini = (FemtoMicroPair*) fBackgroundPairs[bin]->ConstructedAt(fBackgroundPairs[bin]->GetEntriesFast());
      *mini                = *fFemtoPair;
      fLimitsD[bin]++;
    }
  }

  Task::EInitFlag FemtoDumpPairAna::Init() {
    Task::EInitFlag stat = FemtoBasicAna::Init();
    if (stat == Task::EInitFlag::kFATAL) return stat;
    DataManager* mngr     = DataManager::Instance();
    FemtoCorrFunc* f      = (FemtoCorrFunc*) fCFs->At(0, 0);
    DividedHisto1D* dummy = f->GetCF(0);
    Int_t bins;
    Double_t min, max;

    // grouping enabled
    if (dummy->GetNum()->InheritsFrom("TH3")) {  // 3d histo group by q-long/ k-long
      fGrouping.SetGroupByKLong();
      Std::GetAxisPar(*dummy->GetNum(), bins, min, max, "z");
    } else {  // 1d histo group by k* qinv
      fGrouping.GroupByKStar();
      Std::GetAxisPar(*dummy->GetNum(), bins, min, max, "x");
    }
    fGrouping.SetFrame(fFemtoPair->GetFrame());
    fGrouping.SetAxis(bins, min, max);
    fLimitsN.MakeBigger(bins);
    fLimitsD.MakeBigger(bins);
    auto vec = fGrouping.GetBranchesByValue(0, 0);  // 0,0 -> get all branches
    int idx  = 0;
    std::vector<TString> names;
    for (int i = vec.first; i < vec.second; i++) {
      names.push_back(Form("FemtoSignal_%i", i));
    }
    for (auto branchName : names) {
      fSignalPairs.push_back(new TClonesArray("Hal::FemtoMicroPair", 100));
      mngr->Register(branchName, "FemtoPairs", fSignalPairs[idx++], kTRUE);
    }
    if (fWriteBackground) {
      std::vector<TString> names2;
      for (int i = vec.first; i < vec.second; i++) {
        names2.push_back(Form("FemtoBackground_%i", i));
      }
      idx = 0;
      for (auto branchName : names2) {
        fBackgroundPairs.push_back(new TClonesArray("Hal::FemtoMicroPair", 100));
        mngr->Register(branchName, "FemtoPairs", fBackgroundPairs[idx++], kTRUE);
      }
    }

    if (IdenticalParticles())
      fBackgroundMode = kNoBackgroundID;
    else
      fBackgroundMode = kNoBackgroundNID;
    return Task::EInitFlag::kSUCCESS;
  }

  FemtoDumpPairAna::~FemtoDumpPairAna() {}

  void FemtoDumpPairAna::Exec(Option_t* opt) {
    for (auto i : fSignalPairs)
      i->Clear();

    for (auto i : fBackgroundPairs)
      i->Clear();

    FemtoBasicAna::Exec(opt);
  }

  void FemtoDumpPairAna::FinishTask() {
    Hal::FemtoBasicAna::FinishTask();
    // Package* pack = Report();
    GoToDir("HalInfo");
    fGrouping.Clone()->Write();
    gFile->cd();
  }

  //==================================================================== group class
}  // namespace Hal
