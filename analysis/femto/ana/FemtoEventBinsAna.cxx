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


namespace Hal {
  FemtoEventBinsAna::FemtoEventBinsAna() : FemtoBasicAna(), fEventBins(0) {}

  FemtoEventBinsAna::~FemtoEventBinsAna() {
    for (auto i : fEventBinningCuts) {
      if (i) delete i;
    }
  }

  FemtoEventBinsAna::FemtoEventBinsAna(const FemtoEventBinsAna& other) :
    FemtoBasicAna(other), fEventBins(other.fEventBins), fFakeEventBinID(other.fFakeEventBinID) {
    if (other.fEventBinningCuts.size()) {
      fEventBinningCuts.resize(other.fEventBinningCuts.size(), nullptr);
      for (unsigned int i = 0; i < other.fEventBinningCuts.size(); i++) {
        if (other.fEventBinningCuts[i]) { fEventBinningCuts[i] = other.fEventBinningCuts[i]->MakeCopy(); }
      }
    }
  }

  Task::EInitFlag FemtoEventBinsAna::Init() {
    if (fEventBinningCuts.size() < fCutContainer->GetEventCollectionsNo()) {
      fEventBinningCuts.resize(fCutContainer->GetEventCollectionsNo(), nullptr);
    }
    int maxSize  = 0;
    Bool_t ready = kTRUE;
    for (unsigned int i = 0; i < fEventBinningCuts.size(); i++) {
      if (fEventBinningCuts[i] == nullptr) {
        Hal::Cout::PrintInfo(Form("%s %s null event binning cut at event col %i", __FILE__, __LINE__, i), EInfo::kError);
        EventVirtualCut vcut;
        fEventBinningCuts[i] = new EventBinningCut(vcut, {1});
      }
      if (!fEventBinningCuts[i]->Init(GetTaskID())) ready = kFALSE;
      EventCut* cut = fEventBinningCuts[i]->GetCut();
      cut->SetCollectionID(i);
      AddCut(*cut);
      delete cut;
      maxSize = TMath::Max(maxSize, fEventBinningCuts[i]->GetBinsNo());
    }
    fEventBins = maxSize;

    if (ready == kFALSE) {
      Cout::PrintInfo(Form("Problem with initialization of  %s", fEventBinningCuts[0]->ClassName()), EInfo::kError);
      return Task::EInitFlag::kFATAL;
    }
    return FemtoBasicAna::Init();
  }

  void FemtoEventBinsAna::SetEventBinCut(const EventBinningCut& bin, Option_t* opt) {

    TString option = opt;
    int times = -1, jump = -1;
    int single = -1;
    std::vector<int> cols;

    if (Hal::Std::FindExpressionSingleValue(option, single, kTRUE)) {
      cols.push_back(single);
    } else {
      cols.push_back(bin.GetCollectionID());
    }
    if (Hal::Std::FindExpressionTwoValues(option, times, jump, kTRUE)) {
      for (int i = 0; i < times; i++) {
        cols.push_back(cols[cols.size() - 1] + jump);
      }
    }
    int maxVal = 0;
    for (auto col : cols) {
      maxVal = TMath::Max(maxVal, col);
    }
    fEventBinningCuts.resize(maxVal, nullptr);


    auto makeCopy = [&]() {
      if (Hal::Std::FindParam(option, "re")) {
        return bin.MakeCopyReal();
      } else if (Hal::Std::FindParam(option, "im")) {
        return bin.MakeCopyImg();
      } else {
        return bin.MakeCopy();
      }
    };

    for (auto col : cols) {
      fEventBinningCuts[col] = makeCopy();
      fEventBinningCuts[col]->SetCollectionID(col);
    }
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
    fMemoryMap->Init(fEventBins, GetTaskID(), TESTBIT(fFormatOption, eBitFormat::kCompression), brName);
  }

  Int_t FemtoEventBinsAna::GetEventBin() { return fEventBinningCuts[fCurrentEventCollectionID]->CheckBin(fCurrentEvent); }

  Package* FemtoEventBinsAna::Report() const {
    Package* pack = FemtoBasicAna::Report();
    for (auto& rep : fEventBinningCuts) {
      AddToAnaMetadata(pack, rep->Report());
    }
    return pack;
  }

  void FemtoEventBinsAna::ProcessEvent() {
    const Int_t eventBin = GetEventBin();
    fFakeEventBinID      = fCurrentEventCollectionID;
    if (eventBin < 0) return;
    fCurrentEventCollectionID = fEventCollectionsNo * fFakeEventBinID + eventBin;

    // we are using DummyEventCol to point event collection but still use
    // fCurrentEvent to numbering cuts
    fMemoryMap->PrepareMaps(fCurrentEventCollectionID);
    CutCollection* cont = fCutContainer->GetEventCollection(0);
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
      fEventBins      = other.fEventBins;
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

  void FemtoEventBinsAna::AddCut(const Hal::Cut& cut, Option_t* opt) {
    if (cut.GetUpdateRatio() == ECutUpdate::kEvent) {
      TString option = opt;
      if (option.Contains("im")) { /*imaginary cut */
        FemtoBasicAna::AddCut(cut, "im");
      } else if (option.Contains("re")) { /*real cut*/
        FemtoBasicAna::AddCut(cut, "re");
      } else { /* normal cut */
        FemtoBasicAna::AddCut(cut);
      }
    } else {
      FemtoBasicAna::AddCut(cut, opt);
    }
  }
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

}  // namespace Hal
