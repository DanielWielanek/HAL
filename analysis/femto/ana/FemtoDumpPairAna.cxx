/*
 * FemtoDumpPairAna.cxx
 *
 *  Created on: 9 lut 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoDumpPairAna.h"

#include "DataManager.h"
#include "DividedHisto.h"
#include "FemtoCorrFunc.h"
#include "FemtoPair.h"
#include "MemoryMapManager.h"
#include "ObjectMatrix.h"
#include "TwoTrackAna.h"

#include <TAxis.h>
#include <TClonesArray.h>
#include <TH1.h>
#include <TObjArray.h>


namespace Hal {
  FemtoDumpPairAna::FemtoDumpPairAna() :
    fBinLimit(10000),
    fNBins(0),
    fLimitsN(),
    fLimitsD(),
    fStep(0),
    fMax(0),
    fWriteBackground(kFALSE),
    fSignalPairs(NULL),
    fBackgroundPairs(NULL) {}

  void FemtoDumpPairAna::ProcessFemtoPair() {
    fFemtoPair->Compute();
    Double_t diff = TMath::Abs(fFemtoPair->GetT());
    Int_t bin     = (Int_t)(diff * fStep);
    if (bin >= fNBins) return;
    if (fLimitsN[bin] < fBinLimit) {
      FemtoMicroPair* mini = (FemtoMicroPair*) fSignalPairs->ConstructedAt(fSignalPairs->GetEntriesFast());
      *mini                = *fFemtoPair;
      fLimitsN[bin]++;
    }
  }

  void FemtoDumpPairAna::ProcessFemtoPair_Rotated() {
    fFemtoPair->Compute_Rotated();
    Double_t diff = TMath::Abs(fFemtoPair->GetT());
    Int_t bin     = diff * fStep;
    if (bin >= fNBins) return;
    if (fLimitsD[bin] < fBinLimit) {
      FemtoMicroPair* mini = (FemtoMicroPair*) fBackgroundPairs->ConstructedAt(fBackgroundPairs->GetEntriesFast());
      *mini                = *fFemtoPair;
      mini->SetMomenta2(-mini->GetPx2(), -mini->GetPy2(), mini->GetPz2(), mini->GetE2());
      fLimitsD[bin]++;
    }
  }

  void FemtoDumpPairAna::ProcessFemtoPair_Hemisphere() {
    fFemtoPair->Compute_Hemisphere();
    Double_t diff = TMath::Abs(fFemtoPair->GetT());
    Int_t bin     = diff * fStep;
    if (bin >= fNBins) return;
    if (fLimitsD[bin] < fBinLimit) {
      FemtoMicroPair* mini = (FemtoMicroPair*) fBackgroundPairs->ConstructedAt(fBackgroundPairs->GetEntriesFast());
      *mini                = *fFemtoPair;
      mini->SetMomenta2(-mini->GetPx2(), -mini->GetPy2(), -mini->GetPz2(), mini->GetE2());
      fLimitsD[bin]++;
    }
  }

  void FemtoDumpPairAna::ProcessFemtoPair_Mixed() {
    fFemtoPair->Compute();
    Double_t diff = TMath::Abs(fFemtoPair->GetT());
    Int_t bin     = diff * fStep;
    if (bin >= fNBins) return;
    if (fLimitsD[bin] < fBinLimit) {
      FemtoMicroPair* mini = (FemtoMicroPair*) fBackgroundPairs->ConstructedAt(fBackgroundPairs->GetEntriesFast());
      *mini                = *fFemtoPair;
      fLimitsD[bin]++;
    }
  }

  Task::EInitFlag FemtoDumpPairAna::Init() {
    Task::EInitFlag stat = FemtoBasicAna::Init();
    if (stat == Task::EInitFlag::kFATAL) return stat;
    DataManager* mngr = DataManager::Instance();
    fSignalPairs      = new TClonesArray("FemtoMicroPair", 1000);
    mngr->Register("FemtoSignal", "FemtoPairs", fSignalPairs, kTRUE);
    if (fWriteBackground) {
      fBackgroundPairs = new TClonesArray("FemtoMicroPair", 1000);
      mngr->Register("FemtoBackground", "FemtoPairs", fBackgroundPairs, kTRUE);
    } else {
      if (IdenticalParticles())
        fBackgroundMode = kNoBackgroundID;
      else
        fBackgroundMode = kNoBackgroundNID;
    }
    FemtoCorrFunc* f      = (FemtoCorrFunc*) fCFs->At(0, 0);
    DividedHisto1D* dummy = f->GetCF(0);
    fNBins                = dummy->GetNum()->GetNbinsX();
    fLimitsN.MakeBigger(fNBins);
    fLimitsD.MakeBigger(fNBins);
    Double_t min = dummy->GetNum()->GetXaxis()->GetBinLowEdge(1);
    fMax         = dummy->GetNum()->GetXaxis()->GetBinUpEdge(dummy->GetNum()->GetNbinsX());
    if (min != 0) fNBins = fNBins * 0.5;
    fStep = fMax / ((Double_t) fNBins);
    fStep = 1. / fStep;
    return Task::EInitFlag::kSUCCESS;
  }

  FemtoDumpPairAna::~FemtoDumpPairAna() {}

  void FemtoDumpPairAna::Exec(Option_t* opt) {
    fSignalPairs->Clear();
    if (fBackgroundPairs) fBackgroundPairs->Clear();
    FemtoBasicAna::Exec(opt);
  }
}  // namespace Hal
