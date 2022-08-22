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

namespace Hal {
  Femto1DCFAnaMapPairsDumped::Femto1DCFAnaMapPairsDumped() :
    Femto1DCFAnaMapMC(kFALSE),
    fInFile("pairs.root"),
    fFile(nullptr),
    fTree(nullptr),
    fPairsSignal(nullptr),
    fPairsBackground(nullptr),
    fUseBackground(kFALSE),
    fWeightedBackround(kFALSE),
    fMaxEvents(-1) {}

  void Femto1DCFAnaMapPairsDumped::SetInFile(TString filename) { fInFile = filename; }

  void Femto1DCFAnaMapPairsDumped::Run(Int_t start, Int_t end) {
    if (start == end) {
      start = 0;
      end   = fTree->GetEntries();
    }
    for (int i = start; i < end; i++) {
      fTree->GetEntry(i);
      if (!fUseBackground) {
        MakePairsWeighted(fPairsSignal, kTRUE);
        MakePairsUnWeighted(fPairsSignal, kFALSE);
      } else {
        MakePairsWeighted(fPairsSignal, kTRUE);
        if (fWeightedBackround) {
          MakePairsWeighted(fPairsBackground, kFALSE);
        } else {
          MakePairsUnWeighted(fPairsBackground, kFALSE);
        }
      }
    }
  }

  Bool_t Femto1DCFAnaMapPairsDumped::Init() {
    fFile = new TFile(fInFile);
    if (fFile->IsZombie()) return kFALSE;
    TList* list = fFile->GetListOfKeys();
    for (int i = 0; i < list->GetEntries(); i++) {
      TKey* key    = (TKey*) list->At(i);
      TObject* obj = fFile->Get(key->GetName());
      if (obj->InheritsFrom("TTree")) {
        fTree = (TTree*) obj;
        break;
      }
    }
    if (fTree == nullptr) return kFALSE;
    TBranch* br  = fTree->GetBranch("FemtoSignal.");
    fPairsSignal = new TClonesArray("Hal::FemtoMiniPair");
    br->SetAddress(&fPairsSignal);
    if (fUseBackground) {
      br               = fTree->GetBranch("FemtoBackground.");
      fPairsBackground = new TClonesArray("Hal::FemtoMiniPair");
      br->SetAddress(&fPairsBackground);
    }
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
