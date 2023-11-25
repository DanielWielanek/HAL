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
  FemtoEventBinsAna::FemtoEventBinsAna() : FemtoBasicAna(), fEventBinsMax(0) {}

  FemtoEventBinsAna::~FemtoEventBinsAna() {}

  FemtoEventBinsAna::FemtoEventBinsAna(const FemtoEventBinsAna& other) :
    FemtoBasicAna(other), fEventBinsMax(other.fEventBinsMax), fFakeEventBinID(other.fFakeEventBinID) {
    if (other.fEventBinningCuts.size()) {
      fEventBinningCuts.resize(other.fEventBinningCuts.size(), nullptr);
      for (unsigned int i = 0; i < other.fEventBinningCuts.size(); i++) {
        if (other.fEventBinningCuts[i]) { fEventBinningCuts[i] = other.fEventBinningCuts[i]->MakeCopy(); }
      }
    }
  }

  Task::EInitFlag FemtoEventBinsAna::Init() {
    if (!CheckBinningCuts()) return Task::EInitFlag::kFATAL;
    fEventBinsMax = 0;
    for (unsigned int i = 0; i < fEventBinningCuts.size(); i++) {
      fEventBinningCuts[i]->SetCollectionID(i);
      fEventBinsMax = TMath::Max(fEventBinsMax, fEventBinningCuts[i]->GetBinsNo());
    }
    return FemtoBasicAna::Init();
  }

  void FemtoEventBinsAna::InitMemoryMap() {
    fMemoryMap = new MemoryMapManager(fCutContainer);
    fMemoryMap->SetMixSize(fMixSize);
#ifdef HAL_DEBUG
    Cout::PrintInfo("Initialization MemoryMap", EInfo::kDebugInfo);
#endif
    std::vector<TString> brName;
    if (TESTBIT(fFormatOption, eBitFormat::kReader)) {
      brName.push_back("HalEvent.");
    } else if (TESTBIT(fFormatOption, eBitFormat::kDirectAcesss)) {
      TString evName = DataFormatManager::Instance()->GetFormat(GetTaskID())->ClassName();
      brName.push_back(Form("%s.", evName.Data()));
      brName.push_back(evName);
    }
    fMemoryMap->Init(fEventBinsMax, GetTaskID(), TESTBIT(fFormatOption, eBitFormat::kCompression), brName);
  }

  Int_t FemtoEventBinsAna::GetEventBin() { return fEventBinningCuts[fCurrentEventCollectionID]->CheckBin(fCurrentEvent); }

  Package* FemtoEventBinsAna::Report() const {
    Package* pack = FemtoBasicAna::Report();
    TList* list   = new TList();
    list->SetName("EventBinCuts");
    for (auto& rep : fEventBinningCuts) {
      list->AddLast(rep->Report());
    }
    AddToAnaMetadata(pack, list);

    return pack;
  }

  void FemtoEventBinsAna::ProcessEvent() {
    const Int_t eventBin = GetEventBin();
    fFakeEventBinID      = fCurrentEventCollectionID;
    if (eventBin < 0) return;
    fCurrentEventCollectionID = fEventBinsMax * fFakeEventBinID + eventBin;
    // we are using DummyEventCol to point event collection but still use
    // fCurrentEvent to numbering cuts
    fMemoryMap->PrepareMaps(fCurrentEventCollectionID);
    CutCollection* cont = fCutContainer->GetEventCollection(fFakeEventBinID);
    for (fTrackIndex = 0; fTrackIndex < fMemoryMap->GetTemporaryTotalTracksNo(); fTrackIndex++) {
      fCurrentTrack = fCurrentEvent->GetTrack(fTrackIndex);
      for (int j = 0; j < cont->GetNextNo(); j++) {
        fCurrentTrackCollectionID = cont->GetNextAddr(j);
        if (fCutContainer->PassTrack(fCurrentTrack, fCurrentTrackCollectionID)) {
          fMemoryMap->AddTrackToMapTrack(fCurrentEventCollectionID,
                                         fCurrentTrackCollectionID,
                                         fTrackIndex);  // load track into memory map - may be usefull at
                                                        // finish event
          ProcessTrack();
        }
      }
    }
    fMemoryMap->BufferEvent(fCurrentEventCollectionID);
    fCurrentTrackCollectionID = 0;
    if (IdenticalParticles()) {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("Finish identical event with %i tracks",
                           fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID)),
                      EInfo::kDebugInfo);
#endif
      FinishEventIdentical();
    } else {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("Finish non-identical event with %i %i tracks",
                           fMemoryMap->GetTracksNo(fCurrentEventCollectionID, 0),
                           fMemoryMap->GetTracksNo(fCurrentEventCollectionID, 1)),
                      EInfo::kDebugInfo);
#endif
      FinishEventNonIdentical();
    }
    fCurrentEventCollectionID = fFakeEventBinID;
  }

  FemtoEventBinsAna& FemtoEventBinsAna::operator=(const FemtoEventBinsAna& other) {
    if (this != &other) {
      FemtoBasicAna::operator=(other);
      for (auto i : fEventBinningCuts) {
        if (i) delete i;
      }
      fEventBinningCuts.resize(0);
      if (other.fEventBinningCuts.size()) {
        fEventBinningCuts.resize(other.fEventBinningCuts.size(), nullptr);
        for (unsigned int i = 0; i < other.fEventBinningCuts.size(); i++) {
          if (other.fEventBinningCuts[i]) { fEventBinningCuts[i] = other.fEventBinningCuts[i]->MakeCopy(); }
        }
      }
      fEventBinsMax   = other.fEventBinsMax;
      fFakeEventBinID = other.fFakeEventBinID;
    }
    return *this;
  }

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

  void FemtoEventBinsAna::AddCut(const Hal::Cut& cut, Option_t* opt) { FemtoBasicAna::AddCut(cut, opt); }

  void FemtoEventBinsAna::ProcessFemtoPair() {
    fFemtoPair->Compute();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fFakeEventBinID, fCurrentPairCollectionID))->FillNum(fFemtoPair);
  }

  void FemtoEventBinsAna::ProcessFemtoPair_Perfect() {
    fFemtoPair->Compute();
    fFemtoPair->SetWeight(1.0);
    ((FemtoCorrFunc*) fCFs->At(fFakeEventBinID, fCurrentPairCollectionID))->FillDenPerfect(fFemtoPair);
  }

  void FemtoEventBinsAna::ProcessFemtoPair_Rotated() {
    fFemtoPair->Compute_Rotated();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fFakeEventBinID, fCurrentPairCollectionID))->FillDenRotated(fFemtoPair);
  }

  void FemtoEventBinsAna::ProcessFemtoPair_Hemisphere() {
    fFemtoPair->Compute_Hemisphere();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fFakeEventBinID, fCurrentPairCollectionID))->FillDenHemisphere(fFemtoPair);
  }

  void FemtoEventBinsAna::ProcessFemtoPair_Mixed() {
    fFemtoPair->Compute_Mixed();
    fFemtoPair->SetWeight(1.0);
    ((FemtoCorrFunc*) fCFs->At(fFakeEventBinID, fCurrentPairCollectionID))->FillDenMixed(fFemtoPair);
  }

  void FemtoEventBinsAna::ProcessFemtoPair_Charged() {
    fFemtoPair->Compute_Charged();
    Double_t weight = fCalc->GenerateWeight(fFemtoPair);
    fFemtoPair->SetWeight(weight);
    ((FemtoCorrFunc*) fCFs->At(fFakeEventBinID, fCurrentPairCollectionID))->FillDenCharged(fFemtoPair);
  }

  Bool_t FemtoEventBinsAna::CheckBinningCuts() {
    /** match binned cut from cut container with vector **/
    Int_t eventCol = fCutContainer->GetEventCollectionsNo();
    fEventBinningCuts.resize(eventCol, nullptr);
    auto FindBinCut = [&](int i) {
      auto subCont  = fCutContainer->GetEventCollection(i);
      int eventCuts = subCont->GetCutNo();
      Bool_t found  = false;
      for (int evCut = 0; evCut < eventCuts; evCut++) {
        EventCut* ev = (EventCut*) subCont->GetCut(evCut);
        if (dynamic_cast<EventBinningCut*>(ev)) {
          if (fEventBinningCuts[i] == nullptr) {
            fEventBinningCuts[i] = (EventBinningCut*) ev;
            found                = true;
          } else {
            TString message =
              Form("%s %i: double event binning cut at col %i, cut %s will be ingored", __FILE__, __LINE__, i, ev->ClassName());
            Hal::Cout::PrintInfo(message, EInfo::kError);
          }
        }
      }
      return found;
    };
    for (int i = 0; i < eventCol; i++) {
      auto subCont = fCutContainer->GetEventCollection(i);
      FindBinCut(i);
      if (fEventBinningCuts[i] == nullptr) {
        TString message = Form("%s %i: missing bining cut at col=%i, automatic cut will be added", __FILE__, __LINE__, i);
        Hal::Cout::PrintInfo(message, EInfo::kError);
        Hal::EventVirtualCut v;
        EventBinningCut ev(v, {1});
        ev.SetCollectionID(i);
        AddCut(ev);
        bool found = FindBinCut(i);
        if (!found) return kFALSE;
      }
    }
    /**
     * cuts are matched, check them and initialize
     */
    return kTRUE;
  }

}  // namespace Hal
