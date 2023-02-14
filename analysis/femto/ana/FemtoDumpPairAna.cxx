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
    fGrouping.SetAxis(bins, min, max);
    fLimitsN.MakeBigger(bins);
    fLimitsD.MakeBigger(bins);
    auto vec = fGrouping.GetBranches(min, max, kTRUE);
    int idx  = 0;
    for (auto branchName : vec) {
      fSignalPairs.push_back(new TClonesArray("Hal::FemtoMicroPair", 100));
      mngr->Register(branchName, "FemtoPairs", fSignalPairs[idx++], kTRUE);
    }
    if (fWriteBackground) {
      idx = 0;
      vec = fGrouping.GetBranches(min, max, kFALSE);
      for (auto branchName : vec) {
        fBackgroundPairs.push_back(new TClonesArray("Hal::FemtoMicroPair", 100));
        mngr->Register(branchName, "FemtoPairs", fSignalPairs[idx++], kTRUE);
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
  // group clas

  CorrFitMapGroupConfig::CorrFitMapGroupConfig() : fBins(100), fMin(0), fMax(100) {
    fStep = (fMax - fMin) / Double_t(fBins);
    fStep = 1.0 / fStep;
    SetName("CorrFitMapGroup");
  }

  void CorrFitMapGroupConfig::SetAxis(Int_t bins, Double_t min, Double_t max) {
    fBins = bins;
    fMin  = min;
    fMax  = max;
    fStep = (fMax - fMin) / Double_t(fBins);
    fStep = 1.0 / fStep;
  }
  Bool_t CorrFitMapGroupConfig::GroupByLong() const {
    if (fMode == 1) return kTRUE;
    return kFALSE;
  }

  Bool_t CorrFitMapGroupConfig::GroupByKStar() const { return !GroupByLong(); }

  Int_t CorrFitMapGroupConfig::GetBin(const FemtoPair* pair) const {
    Double_t val = TMath::Abs(pair->GetT());
    if (fMode == 1) {  // long
      val = TMath::Abs(pair->GetZ());
    }
    Int_t bin = (val - fMin) * fStep;
    if (bin >= fBins) return -1;
    return bin;
  }

  void CorrFitMapGroupConfig::Add(const Object* pack) {
    auto* data = dynamic_cast<const CorrFitMapGroupConfig*>(pack);
    if (!data) {
      Hal::Cout::PrintInfo("Cannot add CorrFitMapGroupConfig", EInfo::kError);
      return;
    }
    auto printError = []() { Hal::Cout::PrintInfo("Cannot add CorrFitMapGroupConfig", EInfo::kError); };

    if (fMode != data->fMode) { printError(); }
    if (fBins != data->fBins) { printError(); };
    if (fMin != data->fMin) { printError(); }
  }

  std::vector<TString> CorrFitMapGroupConfig::GetBranches(Double_t min, Double_t max, Bool_t signal) const {
    Int_t lowBin  = (fMin - min) * fStep;
    Int_t highBin = (fMax - min) * fStep;
    std::vector<TString> result;
    TString pattern = "FemtoBackground_%i.";
    if (signal) { pattern = "FemtoSignal_%i."; }
    for (int i = lowBin; i < highBin; i++) {
      TString name = Form(pattern, i);
      result.push_back(name);
    }
    return result;
  }

  void FemtoDumpPairAna::FinishTask() {
    Hal::FemtoBasicAna::FinishTask();
    Package* pack = Report();
    GoToDir("HalInfo");
    fGrouping.Clone()->Write();
    gFile->cd();
  }

}  // namespace Hal
