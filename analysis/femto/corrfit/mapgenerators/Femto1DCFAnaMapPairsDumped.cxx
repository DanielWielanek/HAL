/*
 * Femto1DCFAnaMapPairs.cxx
 *
 *  Created on: 27 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Femto1DCFAnaMapPairsDumped.h"

#include "DividedHisto.h"
#include "Femto1DCFAnaMapMC.h"
#include "Femto1DMapGenerator.h"
#include "FemtoFreezoutGenerator.h"
#include "FemtoMiniPair.h"
#include "FemtoPair.h"
#include "FemtoSourceModel.h"
#include "FemtoWeightGenerator.h"

#include <TBranch.h>
#include <TClonesArray.h>
#include <TCollection.h>
#include <TDirectoryFile.h>
#include <TFile.h>
#include <TH2.h>
#include <TKey.h>
#include <TList.h>
#include <TMathBase.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TObject.h>
#include <TTree.h>

#include "CorrFitMapGroupConfig.h"
#include "CorrFitPairFile.h"

namespace Hal {
  Femto1DCFAnaMapPairsDumped::Femto1DCFAnaMapPairsDumped() :
    Femto1DCFAnaMapMC(kFALSE), fInFile("pairs.root"), fUseBackground(kFALSE), fWeightedBackround(kFALSE), fMaxEvents(-1) {}

  void Femto1DCFAnaMapPairsDumped::SetInFile(TString filename) { fPairFile = new Hal::CorrFitPairFile(filename, "read"); }

  void Femto1DCFAnaMapPairsDumped::Run(Int_t start, Int_t end) {
    if (start == end) {
      start = 0;
      end   = fPairFile->GetEntries();
    }
    for (int i = start; i < end; i++) {
      fPairFile->GetEntry(i);
      int bins = fPairFile->GetConfig()->GetNbins();
      if (!fUseBackground && fPairFile->GetConfig()->HaveSignal()) {
        for (int j = 0; j < bins; j++) {
          MakePairsWeighted(fPairFile->GetSignal(j), kTRUE);
          MakePairsUnWeighted(fPairFile->GetSignal(j), kFALSE);
        }
      } else {
        for (int j = 0; j < bins; j++)
          MakePairsWeighted(fPairFile->GetSignal(j), kTRUE);
        if (fWeightedBackround) {
          for (int j = 0; j < bins; j++)
            MakePairsWeighted(fPairFile->GetBackground(j), kFALSE);
        } else {
          for (int j = 0; j < bins; j++)
            MakePairsUnWeighted(fPairFile->GetBackground(j), kFALSE);
        }
      }
    }
  }

  Bool_t Femto1DCFAnaMapPairsDumped::Init() {
    fPairFile->Init();
    if (!fPairFile->GetConfig()->HaveBackground()) fUseBackground = kFALSE;
    return Femto1DCFAnaMapMC::Init();
  }

  void Femto1DCFAnaMapPairsDumped::MakePairsWeighted(TClonesArray* arr, Bool_t num) {
    TH2* hist;
    if (num) {
      hist = (TH2*) fMap->GetNum();
    } else {
      hist = (TH2*) fMap->GetDen();
    }

    for (int i = 0; i < arr->GetEntriesFast(); i++) {
      FemtoMiniPair* mini = (FemtoMiniPair*) arr->UncheckedAt(i);
      *fPair              = *mini;
      fPair->Compute();
      Double_t k = fPair->GetT();
      if (fIgnoreSign) { k = TMath::Abs(k); }
      for (int r_bin = 0; r_bin < fRBins; r_bin++) {
        Double_t R = fRadiiBins[r_bin];
        fGenerator->GetSourceModel()->SetRadius(R);
        fGenerator->GenerateFreezoutCooordinates(fPair);
        Double_t weight = fWeight->GenerateWeight(fPair);
        hist->Fill(k, R, weight);
      }
    }
  }

  void Femto1DCFAnaMapPairsDumped::MakePairsUnWeighted(TClonesArray* arr, Bool_t num) {
    TH2* hist;
    if (num) {
      hist = (TH2*) fMap->GetNum();
    } else {
      hist = (TH2*) fMap->GetDen();
    }
    for (int i = 0; i < arr->GetEntriesFast(); i++) {
      FemtoMiniPair* mini = (FemtoMiniPair*) arr->UncheckedAt(i);
      *fPair              = *mini;
      fPair->Compute();
      Double_t k = fPair->GetT();
      if (fIgnoreSign) { k = TMath::Abs(k); }
      for (double r_bin = 0; r_bin < fRBins; r_bin++) {
        Double_t R = fRadiiBins[r_bin];
        hist->Fill(k, R, 1);
      }
    }
  }

  Femto1DCFAnaMapPairsDumped::~Femto1DCFAnaMapPairsDumped() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
