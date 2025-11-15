/*
 * FemtoEventBinsAna.cxx
 *
 *  Created on: 4 gru 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoEventBinsAna.h"

#include "Cout.h"
#include "CutCollection.h"
#include "CutContainer.h"
#include "DataFormatManager.h"
#include "EventBinningCut.h"
#include "EventVirtualCut.h"
#include "FemtoPair.h"
#include "MemoryMapManager.h"
#include <TList.h>


namespace Hal {
  FemtoEventBinsAna::FemtoEventBinsAna() : FemtoBasicAna() { fEventBinningEnabled = kTRUE; }

  FemtoEventBinsAna::~FemtoEventBinsAna() {}

  Bool_t FemtoEventBinsAna::InitArray() {
    fCFs              = new ObjectMatrix_2();
    DividedHisto1D* h = ((FemtoCorrFunc*) fCFTemp)->GetCF(0);
    TString name      = h->GetName();
    name.ReplaceAll("[0]", "");
    h->SetName(name);
    fCFs->Init(fEventCollectionsNo, fTwoTrackCollectionsNo, fCFTemp);
    for (int i = 0; i < fEventCollectionsNo; i++) {
      for (int j = 0; j < fTwoTrackCollectionsNo; j++) {
        FemtoCorrFunc* corrfunc = (FemtoCorrFunc*) fCFs->At(i, j);
        corrfunc->SetEventCollID(i);
        corrfunc->SetPairCollID(j);
        corrfunc->Check();
        TString comment = Form("PairBin[%i]\n", j);
        std::vector<Double_t> mini, maxi;
        std::vector<TString> names;
        fEventBinningCuts[i]->GetBinParam(i, mini, maxi, names);
        for (int k = 0; k < (int) names.size(); k++) {
          comment = comment + " " + names[k] + Form("[%4.2f %4.2f]\n", mini[k], maxi[k]);
        }
        corrfunc->SetComment(comment);
      }
    }
    return kTRUE;
  }

  void FemtoEventBinsAna::ProcessFemtoPair() {
    fFemtoPair->Compute();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillNum(fFemtoPair);
  }

  void FemtoEventBinsAna::ProcessFemtoPair_Perfect() {
    fFemtoPair->Compute();
    fFemtoPair->SetWeight(1.0);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillDenPerfect(fFemtoPair);
  }

  void FemtoEventBinsAna::ProcessFemtoPair_Rotated() {
    fFemtoPair->Compute_Rotated();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillDenRotated(fFemtoPair);
  }

  void FemtoEventBinsAna::ProcessFemtoPair_Hemisphere() {
    fFemtoPair->Compute_Hemisphere();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillDenHemisphere(fFemtoPair);
  }

  void FemtoEventBinsAna::ProcessFemtoPair_Mixed() {
    fFemtoPair->Compute_Mixed();
    fFemtoPair->SetWeight(1.0);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillDenMixed(fFemtoPair);
  }

  void FemtoEventBinsAna::ProcessFemtoPair_Charged() {
    fFemtoPair->Compute_Charged();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fCurrentEventCollectionID, fCurrentPairCollectionID))->FillDenCharged(fFemtoPair);
  }

}  // namespace Hal
