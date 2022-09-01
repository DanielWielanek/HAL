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
    fGroup(kFALSE),
    fXmin(0),
    fXmax(0),
    fXstep(0),
    fXbins(0),
    fGroupKstar(kFALSE) {}

  void FemtoDumpPairAna::ProcessFemtoPair() {
    fFemtoPair->Compute();
    Double_t diff = TMath::Abs(fFemtoPair->GetT());
    Int_t bin     = (Int_t)(diff * fStep);
    if (bin >= fNBins) return;
    if (fLimitsN[bin] < fBinLimit) {
      if (fGroupKstar) {  // group by kstar
        Int_t xbin = FindBinX();
        if (xbin > -1 && xbin < fXbins) {
          FemtoMicroPair* mini = (FemtoMicroPair*) fSignalPairs[xbin]->ConstructedAt(fSignalPairs[xbin]->GetEntriesFast());
          *mini                = *fFemtoPair;
          std::cout << "ADD PAIR " << xbin << std::endl;
        }
      } else {
        Int_t xbin = FindBinZ();
        if (xbin > -1 && xbin < fXbins) {
          FemtoMicroPair* mini = (FemtoMicroPair*) fSignalPairs[xbin]->ConstructedAt(fSignalPairs[xbin]->GetEntriesFast());
          *mini                = *fFemtoPair;
        }
      }
      fLimitsN[bin]++;
    }
  }

  void FemtoDumpPairAna::ProcessFemtoPair_Rotated() {
    fFemtoPair->Compute_Rotated();
    Double_t diff = TMath::Abs(fFemtoPair->GetT());
    Int_t bin     = diff * fStep;
    if (bin >= fNBins) return;
    if (fLimitsD[bin] < fBinLimit) {

      if (fGroupKstar) {  // group by kstar
        Int_t xbin = FindBinX();
        if (xbin > -1 && xbin < fXbins) {
          FemtoMicroPair* mini =
            (FemtoMicroPair*) fBackgroundPairs[xbin]->ConstructedAt(fBackgroundPairs[xbin]->GetEntriesFast());
          *mini = *fFemtoPair;
          mini->SetMomenta2(-mini->GetPx2(), -mini->GetPy2(), mini->GetPz2(), mini->GetE2());
        }
      } else {
        Int_t xbin = FindBinZ();
        if (xbin > -1 && xbin < fXbins) {
          FemtoMicroPair* mini =
            (FemtoMicroPair*) fBackgroundPairs[xbin]->ConstructedAt(fBackgroundPairs[xbin]->GetEntriesFast());
          *mini = *fFemtoPair;
          mini->SetMomenta2(-mini->GetPx2(), -mini->GetPy2(), mini->GetPz2(), mini->GetE2());
        }
      }

      fLimitsD[bin]++;
    }
  }

  void FemtoDumpPairAna::ProcessFemtoPair_Hemisphere() {
    fFemtoPair->Compute_Hemisphere();
    Double_t diff = TMath::Abs(fFemtoPair->GetT());
    Int_t bin     = diff * fStep;
    if (bin >= fNBins) return;
    if (fLimitsD[bin] < fBinLimit) {
      if (fGroupKstar) {  // group by kstar
        Int_t xbin = FindBinX();
        if (xbin > -1 && xbin < fXbins) {
          FemtoMicroPair* mini =
            (FemtoMicroPair*) fBackgroundPairs[xbin]->ConstructedAt(fBackgroundPairs[xbin]->GetEntriesFast());
          *mini = *fFemtoPair;
          mini->SetMomenta2(-mini->GetPx2(), -mini->GetPy2(), -mini->GetPz2(), mini->GetE2());
        }
      } else {
        Int_t xbin = FindBinZ();
        if (xbin > -1 && xbin < fXbins) {
          FemtoMicroPair* mini =
            (FemtoMicroPair*) fBackgroundPairs[xbin]->ConstructedAt(fBackgroundPairs[xbin]->GetEntriesFast());
          *mini = *fFemtoPair;
          mini->SetMomenta2(-mini->GetPx2(), -mini->GetPy2(), -mini->GetPz2(), mini->GetE2());
        }
      }
      fLimitsD[bin]++;
    }
  }

  void FemtoDumpPairAna::ProcessFemtoPair_Mixed() {
    fFemtoPair->Compute();
    Double_t diff = TMath::Abs(fFemtoPair->GetT());
    Int_t bin     = diff * fStep;
    if (bin >= fNBins) return;
    if (fLimitsD[bin] < fBinLimit) {
      if (fGroupKstar) {  // group by kstar
        Int_t xbin = FindBinX();
        if (xbin > -1 && xbin < fXbins) {
          FemtoMicroPair* mini =
            (FemtoMicroPair*) fBackgroundPairs[xbin]->ConstructedAt(fBackgroundPairs[xbin]->GetEntriesFast());
          *mini = *fFemtoPair;
        }
      } else {
        Int_t xbin = FindBinZ();
        if (xbin > -1 && xbin < fXbins) {
          FemtoMicroPair* mini =
            (FemtoMicroPair*) fBackgroundPairs[xbin]->ConstructedAt(fBackgroundPairs[xbin]->GetEntriesFast());
          *mini = *fFemtoPair;
        }
      }
      fLimitsD[bin]++;
    }
  }

  Task::EInitFlag FemtoDumpPairAna::Init() {
    Task::EInitFlag stat = FemtoBasicAna::Init();
    if (stat == Task::EInitFlag::kFATAL) return stat;
    DataManager* mngr     = DataManager::Instance();
    FemtoCorrFunc* f      = (FemtoCorrFunc*) fCFs->At(0, 0);
    DividedHisto1D* dummy = f->GetCF(0);
    fNBins                = dummy->GetNum()->GetNbinsX();
    fLimitsN.MakeBigger(fNBins);
    fLimitsD.MakeBigger(fNBins);
    if (!fGroup) {
      fSignalPairs.push_back(new TClonesArray("Hal::FemtoMicroPair", 1000));
      mngr->Register("FemtoSignal.", "FemtoPairs", fSignalPairs[0], kTRUE);
      if (fWriteBackground) {
        fBackgroundPairs.push_back(new TClonesArray("Hal::FemtoMicroPair", 1000));
        mngr->Register("FemtoBackground.", "FemtoPairs", fBackgroundPairs[0], kTRUE);
      }
    } else {  // grouping enabled
      fXbins = 0;
      if (dummy->GetNum()->InheritsFrom("TH3")) {  // 3d histo group by q-long/ k-long
        fGroupKstar = kFALSE;
        Std::GetAxisPar(*dummy->GetNum(), fXbins, fXmin, fXmax, "z");
        fXstep = (fXmax - fXmin) / double(fXbins);
      } else {  // 1d histo group by k* qinv
        fGroupKstar = kTRUE;
        fXbins      = dummy->GetNum()->GetNbinsX();
        Std::GetAxisPar(*dummy->GetNum(), fXbins, fXmin, fXmax, "x");
        fXstep = (fXmax - fXmin) / double(fXbins);
      }
      fXstep = 1.0 / fXstep;
      for (int i = 0; i < fXbins; i++) {
        TString brName = Form("FemtoSignal_%i.", i);
        fSignalPairs.push_back(new TClonesArray("Hal::FemtoMicroPair", 100));
        mngr->Register(brName, "FemtoPairs", fSignalPairs[i], kTRUE);
        if (fWriteBackground) {
          brName = Form("FemtoBackground_%i.", i);
          fBackgroundPairs.push_back(new TClonesArray("Hal::FemtoMicroPair", 100));
          mngr->Register(brName, "FemtoPairs", fBackgroundPairs[i], kTRUE);
        }
      }
    }

    if (IdenticalParticles())
      fBackgroundMode = kNoBackgroundID;
    else
      fBackgroundMode = kNoBackgroundNID;


    Double_t min = dummy->GetNum()->GetXaxis()->GetBinLowEdge(1);
    fMax         = dummy->GetNum()->GetXaxis()->GetBinUpEdge(dummy->GetNum()->GetNbinsX());
    if (min != 0) fNBins = fNBins * 0.5;
    fStep = fMax / ((Double_t) fNBins);
    fStep = 1. / fStep;
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

  Int_t FemtoDumpPairAna::FindBinZ() const {
    Double_t z = fFemtoPair->GetZ();
    return int((z - fXmin) * fStep);
  }

  Int_t FemtoDumpPairAna::FindBinX() const {
    Double_t x = fFemtoPair->GetT();
    return int((x - fXmin) * fStep);
  }

}  // namespace Hal
