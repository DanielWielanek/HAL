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
#include "MemoryMapManager.h"


namespace Hal {
  FemtoEventBinsAna::FemtoEventBinsAna() : FemtoBasicAna(), fEventBinningCut(nullptr), fTotalEventBins(0) {}

  FemtoEventBinsAna::~FemtoEventBinsAna() {
    if (fEventBinningCut) delete fEventBinningCut;
  }

  FemtoEventBinsAna::FemtoEventBinsAna(const FemtoEventBinsAna& other) :
    FemtoBasicAna(other), fEventBinningCut(nullptr), fTotalEventBins(other.fTotalEventBins) {
    if (other.fEventBinningCut) { fEventBinningCut = (EventBinningCut*) other.fEventBinningCut->MakeCopy(); }
  }

  Task::EInitFlag FemtoEventBinsAna::Init() {
    if (fEventBinningCut == nullptr) {
      EventVirtualCut vcut;
      EventBinningCut bin(vcut, {1});
      SetEventBinCut(bin, "");
    }
    fTotalEventBins     = fEventBinningCut->GetBinsNo();
    Bool_t ready        = fEventBinningCut->Init(GetTaskID());
    const EventCut* cut = fEventBinningCut->GetCut();
    AddCut(*cut);
    delete cut;
    if (ready == kFALSE) {
      Cout::PrintInfo(Form("Problem with initialization of  %s", fEventBinningCut->ClassName()), EInfo::kError);
      return Task::EInitFlag::kFATAL;
    }
    return FemtoBasicAna::Init();
  }

  void FemtoEventBinsAna::SetEventBinCut(const EventBinningCut& bin, Option_t* opt) {
    if (fEventBinningCut) {
      delete fEventBinningCut;
      fEventBinningCut = nullptr;
    }
    TString option = opt;
    Int_t start = -1, end = -1;
    Hal::Std::FindExpressionTwoValues(option, end, start, kTRUE);
    if (start != -1 || end != -1) {
      Cout::PrintInfo("You are trying to set eventbinning collections,  this is not allowed", EInfo::kWarning);
      Cout::PrintInfo("Cut collections will be ignored", EInfo::kWarning);
    }
    if (Hal::Std::FindParam(option, "re")) {
      fEventBinningCut = bin.MakeCopyReal();
    } else if (Hal::Std::FindParam(option, "im")) {
      fEventBinningCut = bin.MakeCopyImg();
    } else {
      fEventBinningCut = bin.MakeCopy();
    }
  }

  void FemtoEventBinsAna::InitMemoryMap() {
    fTotalEventBins = fEventBinningCut->GetBinsNo();
    fMemoryMap      = new MemoryMapManager(fCutContainer);
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
    fMemoryMap->Init(fTotalEventBins, GetTaskID(), TESTBIT(fFormatOption, eBitFormat::kCompression), brName);
  }

  Int_t FemtoEventBinsAna::GetEventBin() { return fEventBinningCut->CheckBin(fCurrentEvent); }

  Package* FemtoEventBinsAna::Report() const {
    Package* pack = FemtoBasicAna::Report();
    AddToAnaMetadata(pack, fEventBinningCut->Report());
    return pack;
  }

  void FemtoEventBinsAna::ProcessEvent() {
    const Int_t eventBin = GetEventBin();
    if (eventBin < 0) return;
    const Int_t evCol         = fCurrentEventCollectionID;
    fCurrentEventCollectionID = eventBin;
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
    fCurrentEventCollectionID = evCol;
  }

  FemtoEventBinsAna& FemtoEventBinsAna::operator=(const FemtoEventBinsAna& other) {
    if (this != &other) {
      FemtoBasicAna::operator=(other);
      if (fEventBinningCut) {
        delete fEventBinningCut;
        fEventBinningCut = nullptr;
      }
      if (other.fEventBinningCut) fEventBinningCut = other.fEventBinningCut->MakeCopy();
      fTotalEventBins = other.fTotalEventBins;
    }
    return *this;
  }

  Bool_t FemtoEventBinsAna::InitArray() {
    fCFs              = new ObjectMatrix_2();
    DividedHisto1D* h = ((FemtoCorrFunc*) fCFTemp)->GetCF(0);
    TString name      = h->GetName();
    name.ReplaceAll("[0]", "");
    h->SetName(name);
    Int_t eventBins = fEventBinningCut->GetBinsNo();
    fCFs->Init(eventBins, fTwoTrackCollectionsNo, fCFTemp);
    for (int i = 0; i < eventBins; i++) {
      for (int j = 0; j < fTwoTrackCollectionsNo; j++) {
        FemtoCorrFunc* corrfunc = (FemtoCorrFunc*) fCFs->At(i, j);
        corrfunc->SetEventCollID(0);
        corrfunc->SetPairCollID(j);
        corrfunc->Check();
        TString comment = Form("PairBin[%i]\n", j);
        std::vector<Double_t> mini, maxi;
        std::vector<TString> names;
        fEventBinningCut->GetBinParam(i, mini, maxi, names);
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

}  // namespace Hal
