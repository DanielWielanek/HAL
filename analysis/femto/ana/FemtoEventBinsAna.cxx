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
#include "EventBinningCut.h"
#include "EventVirtualCut.h"
#include "MemoryMapManager.h"


namespace Hal {
  FemtoEventBinsAna::FemtoEventBinsAna() :
    FemtoBasicAna(), fEventBinningCuts(nullptr), fEventBinnngsBinsNo(nullptr), fTotalEventBins(0) {}

  FemtoEventBinsAna::~FemtoEventBinsAna() {
    if (fEventBinningCuts) delete fEventBinningCuts;
    if (fEventBinnngsBinsNo) delete fEventBinnngsBinsNo;
  }

  FemtoEventBinsAna::FemtoEventBinsAna(const FemtoEventBinsAna& other) :
    FemtoBasicAna(other), fEventBinningCuts(nullptr), fEventBinnngsBinsNo(nullptr), fTotalEventBins(other.fTotalEventBins) {
    if (other.fEventBinningCuts) { fEventBinningCuts = (TObjArray*) other.fEventBinningCuts->Clone(); }
    if (other.fEventBinnngsBinsNo) fEventBinnngsBinsNo = new Array_1<Int_t>(*other.fEventBinnngsBinsNo);
  }

  Task::EInitFlag FemtoEventBinsAna::Init() {
    if (fEventBinningCuts == nullptr) {
      EventVirtualCut vcut;
      EventBinningCut bin(vcut, {1});
      AddEventBinCut(bin, "");
    }
    fEventBinnngsBinsNo = new Array_1<Int_t>(fEventBinningCuts->GetEntriesFast());
    Int_t mult          = 1;
    fEventBinnngsBinsNo->Set(0, 1);

    for (int i = 1; i <= fEventBinningCuts->GetEntriesFast(); i++) {
      EventBinningCut* evcut = (EventBinningCut*) fEventBinningCuts->UncheckedAt(i - 1);
      mult                   = mult * evcut->GetBinsNo();
      fEventBinnngsBinsNo->Set(i, mult);
      Bool_t ready = evcut->Init(GetTaskID());
      if (ready == kFALSE) {
        Cout::PrintInfo(Form("Problem with initialization of  %s", evcut->ClassName()), EInfo::kLessError);
        return Task::EInitFlag::kFATAL;
      }
    }
    return FemtoBasicAna::Init();
  }

  void FemtoEventBinsAna::AddEventBinCut(const EventBinningCut& bin, Option_t* opt) {
    if (fEventBinningCuts == nullptr) fEventBinningCuts = new TObjArray();
    TString option = opt;
    Int_t col      = bin.GetCollectionID();
    Int_t start, end;
    Bool_t loop = kFALSE;
    if (HalStd::FindExpressionTwoValues(option, end, start, kTRUE)) { loop = kTRUE; }
    EventBinningCut* cut = nullptr;
    if (HalStd::FindParam(option, "re")) {
      cut = bin.MakeCopyReal();
    } else if (HalStd::FindParam(option, "im")) {
      cut = bin.MakeCopyImg();
    } else {
      cut = bin.MakeCopy();
    }
    if (loop) {
      for (int i = 0; i < end; i++) {
        fEventBinningCuts->AddAt(cut->MakeCopy(), col + i * start);
        AddCut(*bin.GetCut(), Form("{%i}", col + i * start));
      }
    } else {
      fEventBinningCuts->AddAt(cut->MakeCopy(), col);
      AddCut(*bin.GetCut());
    }
    delete cut;
  }

  void FemtoEventBinsAna::InitMemoryMap() {
    Int_t multi_factor = 1;
    for (int i = 0; i < fEventBinningCuts->GetEntriesFast(); i++) {
      EventBinningCut* bin = (EventBinningCut*) fEventBinningCuts->UncheckedAt(i);
      multi_factor         = multi_factor * bin->GetBinsNo();
    }
    fTotalEventBins = multi_factor;
    fMemoryMap      = new MemoryMapManager(fCutContainer);
    fMemoryMap->SetMixSize(fMixSize);
#ifdef HAL_DEBUG
    Cout::PrintInfo("Initialization MemoryMap", EInfo::kLessInfo);
#endif
    fMemoryMap->Init(multi_factor, GetTaskID(), fKeepSource, fCompressEvents, fDirectAcces);
  }

  Int_t FemtoEventBinsAna::GetEventBin() {
    Int_t bin = 0;
    for (int i = 0; i < fEventBinningCuts->GetEntriesFast(); i++) {
      EventBinningCut* event = (EventBinningCut*) fEventBinningCuts->UncheckedAt(i);
      Int_t cutBin           = event->CheckBin(fCurrentEvent);
      if (cutBin == -1) return -1;
      bin = bin + cutBin * fEventBinnngsBinsNo->Get(i);
    }
    return bin;
  }

  Package* FemtoEventBinsAna::Report() const {
    Package* pack   = FemtoBasicAna::Report();
    Package* packEB = new Package();
    for (int i = 0; i < fEventBinningCuts->GetEntries(); i++) {
      packEB->AddObject(((EventBinningCut*) fEventBinningCuts->At(i))->Report());
    }
    AddToAnaMetadata(pack, packEB);
    return pack;
  }

  void FemtoEventBinsAna::ProcessEvent() {
    Int_t eventBin = GetEventBin();
    if (eventBin < 0) return;
    Int_t DummyEventCol = eventBin + fCurrentEventCollectionID * fTotalEventBins;
    // we are using DummyEventCol to point event collection but still use
    // fCurrentEvent to numbering cuts
    fMemoryMap->PrepareMaps(DummyEventCol);
    CutCollection* cont = fCutContainer->GetEventCollection(fCurrentEventCollectionID);
    for (fTrackIndex = 0; fTrackIndex < fMemoryMap->GetTemporaryTotalTracksNo(); fTrackIndex++) {
      fCurrentTrack = fCurrentEvent->GetTrack(fTrackIndex);
      for (int j = 0; j < cont->GetNextNo(); j++) {
        fCurrentTrackCollectionID = cont->GetNextAddr(j);
        if (fCutContainer->PassTrack(fCurrentTrack, fCurrentTrackCollectionID)) {
          fMemoryMap->AddTrackToMapTrack(DummyEventCol,
                                         fCurrentTrackCollectionID,
                                         fTrackIndex);  // load track into memory map - may be usefull at
                                                        // finish event
          ProcessTrack();
        }
      }
    }
    fMemoryMap->BufferEvent(DummyEventCol);
    fCurrentTrackCollectionID = 0;
    /*fCurrentEventCollectionID is not used by two track ana to get pair cuts,
     because we have fixed one or two two-track collections, therefore we can
     temporary replace fCurrentEventCollection by DummyEvent
     */
    fEventCollectionCF        = fCurrentEventCollectionID;
    fCurrentEventCollectionID = DummyEventCol;
    if (IdenticalParticles()) {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Format("Finish identical event with %i tracks",
                             fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID)),
                      EInfo::kLessInfo);
#endif
      FinishEventIdentical();
    } else {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Format("Finish non-identical event with %i %i tracks",
                             fMemoryMap->GetTracksNo(fCurrentEventCollectionID, 0),
                             fMemoryMap->GetTracksNo(fCurrentEventCollectionID, 1)),
                      EInfo::kLessInfo);
#endif
      FinishEventNonIdentical();
    }
    fCurrentEventCollectionID = fEventCollectionCF;
  }

  FemtoEventBinsAna& FemtoEventBinsAna::operator=(const FemtoEventBinsAna& other) {
    if (this != &other) {
      FemtoBasicAna::operator=(other);
      if (fEventBinningCuts) {
        delete fEventBinningCuts;
        fEventBinningCuts = nullptr;
      }
      if (fEventBinnngsBinsNo) {
        delete fEventBinnngsBinsNo;
        fEventBinnngsBinsNo = nullptr;
      }
      if (other.fEventBinningCuts) fEventBinningCuts = (TObjArray*) other.fEventBinningCuts->Clone();
      if (other.fEventBinnngsBinsNo) { fEventBinnngsBinsNo = (Array_1<Int_t>*) other.fEventBinnngsBinsNo->Clone(); }
      fTotalEventBins = other.fTotalEventBins;
    }
    return *this;
  }
}  // namespace Hal
