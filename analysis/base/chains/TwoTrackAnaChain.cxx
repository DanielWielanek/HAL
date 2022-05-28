/*
 * TwoTrackAnaChain.cxx
 *
 *  Created on: 26 sie 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackAnaChain.h"

#include "Cout.h"
#include "CutContainer.h"
#include "DataFormatManager.h"
#include "EventAna.h"
#include "EventVirtualCut.h"
#include "MemoryMapManager.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"
#include "TrackVirtualCut.h"
#include "TwoTrack.h"
#include "TwoTrackVirtualCut.h"

#include <TFile.h>
#include <TString.h>


namespace Hal {
  TwoTrackAnaChain::TwoTrackAnaChain(Bool_t use_background) : fTaskNo(0), fTask(NULL) {
    if (use_background) {
      fTiers          = ECutUpdate::kTwoTrackBackground;
      fBackgroundMode = kMixedPairs;
    }
  }

  TwoTrackAnaChain::TwoTrackAnaChain() : fTaskNo(0), fTask(NULL) {}

  void TwoTrackAnaChain::RemoveTask(Int_t no) {
    TwoTrackAna** temp = fTask;
    fTask              = new TwoTrackAna*[fTaskNo - 1];
    Int_t n            = 0;
    for (int i = 0; i < fTaskNo; i++) {
      if (i == no) continue;
      fTask[n] = temp[i];
      n++;
    }
    delete temp[no];
    fTaskNo--;
    delete[] temp;
  }

  void TwoTrackAnaChain::ProcessEvent() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentEventCollectionID = fCurrentEventCollectionID;
    }
    TwoTrackAna::ProcessEvent();
  }

  Package* TwoTrackAnaChain::Report() const {
    Package* pack = EventAna::Report();
    for (int i = 0; i < fTaskNo; i++) {
      pack->AddObject(new ParameterString("Subtask name", fTask[i]->ClassName()));
    }
    return pack;
  }

  void TwoTrackAnaChain::ProcessTrack() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessTrack();
    }
  }

  void TwoTrackAnaChain::AddAnalysis(TwoTrackAna* ana) {
    if (!ana->InheritsFrom("Hal::TwoTrackAna")) {
      Cout::PrintInfo("Tasks that doesn't inherit from TwoTrackAna cannot be processed "
                      "vy this task",
                      EInfo::kWarning);
    }
    if (ana->fTiers != fTiers) {
      Cout::PrintInfo(Form("Task that is added must have the same trier %s will not be added", ana->ClassName()),
                      EInfo::kWarning);
      return;
    }
    if (fTask == NULL) {
      fTask = new TwoTrackAna*[1];
      ana->MarkAsInChain();
      fTask[0] = ana;
      fTaskNo++;
    } else {
      for (int i = 0; i < fTaskNo; i++) {
        if (ana == fTask[i]) {
          Cout::PrintInfo("Task already added", EInfo::kError);
          return;
        }
      }
      TwoTrackAna** temp = fTask;
      fTask              = new TwoTrackAna*[fTaskNo + 1];
      for (int i = 0; i < fTaskNo; i++) {
        fTask[i] = temp[i];
      }
      ana->MarkAsInChain();
      fTask[fTaskNo] = ana;
      fTaskNo++;
      delete[] temp;
    }
  }

  void TwoTrackAnaChain::FinishTask() {
    Package* pack         = Report();
    Package* metadata_new = new Package();
    metadata_new->SetName("RunInfo");
    metadata_new->AddObject(new ParameterString("Software ver", HAL_PHYSICALANALYSYS_VER));
    metadata_new->AddObject(new ParameterString("Date", Hal::Std::GetDate(), 'f'));
    metadata_new->AddObject(new ParameterString("Time", Hal::Std::GetTime(), 'f'));
    metadata_new->AddObject(new ParameterUInt("Processed_events", fProcessedEvents, '+'));
    metadata_new->AddObject(new ParameterString("Input file", GetInputFileName(), 'f'));
    GoToDir("HalInfo");
    TDirectory* dir        = (TDirectory*) gFile;
    TDirectory* metadatata = (TDirectory*) dir->Get("HalInfo");
    if (metadatata->Get("RunInfo")) {
      delete metadata_new;
    } else {
      metadata_new->Write("RunInfo");
    }
    GoToDir("HalPhysics");
    pack->Write(Form("AnaPackage_%i", GetTaskID()));
    if (pack) { delete pack; }
    for (int i = 0; i < fTaskNo; i++) {
      SynchronizeCutContainers(fTask[i], kFALSE);
      Package* subtask_pack = fTask[i]->Report();
      subtask_pack->Write(Form("AnaPackage_%i", fTask[i]->GetTaskID()));
      delete subtask_pack;
    }
    gFile->cd();
    Cout::PrintInfo(Form("%s done, writing results", this->ClassName()), EInfo::kInfo);
  }

  Task::EInitFlag TwoTrackAnaChain::Init() {
    Task::EInitFlag stat = TwoTrackAna::Init();
    if (stat == Task::EInitFlag::kSUCCESS) {
      DataFormatManager* dataManager = DataFormatManager::Instance();
      for (int i = 0; i < fTaskNo; i++) {
        fTask[i]->SetFormat(dataManager->GetNewEvent(GetTaskID()));
        SynchronizeCutContainers(fTask[i], kTRUE);
        if (Task::EInitFlag::kFATAL == fTask[i]->Init()) {
          RemoveTask(i);
          i--;
          continue;
        }
        LinkTask(fTask[i]);
      }
      return Task::EInitFlag::kSUCCESS;
    }
    return Task::EInitFlag::kFATAL;
  }

  void TwoTrackAnaChain::ProcessPair() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair();
    }
  }

  void TwoTrackAnaChain::ProcessPair2() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair2();
    }
  }

  void TwoTrackAnaChain::ProcessPair_Mixed() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair_Mixed();
    }
  }

  void TwoTrackAnaChain::ProcessPair2_Mixed() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair2_Mixed();
    }
  }

  void TwoTrackAnaChain::ProcessPair_Perfect() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair_Perfect();
    }
  }

  void TwoTrackAnaChain::ProcessPair2_Perfect() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair2_Perfect();
    }
  }

  void TwoTrackAnaChain::ProcessPair_Rotated() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair_Rotated();
    }
  }

  void TwoTrackAnaChain::ProcessPair2_Rotated() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair2_Rotated();
    }
  }

  void TwoTrackAnaChain::ProcessPair_Hemisphere() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair_Hemisphere();
    }
  }

  void TwoTrackAnaChain::ProcessPair2_Hemisphere() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair2_Hemisphere();
    }
  }

  void TwoTrackAnaChain::ProcessPair_ChargedId() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair_ChargedId();
    }
  }

  void TwoTrackAnaChain::ProcessPair_ChargedNId() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair_ChargedNId();
    }
  }

  void TwoTrackAnaChain::ProcessPair_Charged2() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair_Charged2();
    }
  }

  void TwoTrackAnaChain::ProcessPair2_Charged2() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair2_Charged2();
    }
  }

  void TwoTrackAnaChain::ProcessPair_Charged3() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair_Charged3();
    }
  }

  void TwoTrackAnaChain::ProcessPair2_Charged3() {
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentPairCollectionID = fCurrentPairCollectionID;
      fTask[i]->ProcessPair2_Charged3();
    }
  }

  void TwoTrackAnaChain::FinishEventNonIdentical() {
    fCurrentTrack1CollectionNo = fCurrentTrackCollectionID;
    fCurrentTrack2CollectionNo = fCurrentTrackCollectionID + 1;
    Int_t tr1                  = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo);
    Int_t tr2                  = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo);
    if (tr1 == 0 || tr2 == 0) {
      if (fBackgroundMode == kMixedPairsNID) { fMemoryMap->RejectLastEvent(fCurrentEventCollectionID); }
      return;
    }
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentTrack1CollectionNo = fCurrentTrack1CollectionNo;
      fTask[i]->fCurrentTrack2CollectionNo = fCurrentTrack2CollectionNo;
      fTask[i]->fCurrentTrackCollectionID  = fCurrentTrackCollectionID;
    }
    switch (fBackgroundMode) {
      case kPerfectPairsNID: {
        MakePairs2();
        MakePairs2_Perfect();
      } break;
      case kMixedPairsNID: {
        MakePairs2();
        if (fMemoryMap->IsReadyToMixing(fCurrentEventCollectionID)) { MakePairs2_Mixed(); }
      } break;
      case kRotatedPairsNID: {
        MakePairs2();
        MakePairs2_Rotated();
      } break;
      case kHemispherePairsNID: {
        MakePairs2();
        MakePairs2_Hemisphere();
      } break;
      case kNoBackgroundNID: {
        MakePairs2();
      } break;
      case kChargedNID2: {
        MakePairs2();
        // AB AA + BB
        MakePairs2_Charged2();
      } break;
      case kChargedNID3: {
        MakePairs2();
        // AB // CC
        fCurrentTrackCollectionID = fCurrentTrackCollectionID + 2;
        for (int i = 0; i < fTaskNo; i++) {
          fTask[i]->fCurrentTrack1CollectionNo = fCurrentTrack1CollectionNo;
          fTask[i]->fCurrentTrack2CollectionNo = fCurrentTrack2CollectionNo;
          fTask[i]->fCurrentTrackCollectionID  = fCurrentTrackCollectionID;
        }
        if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID) == 0) return;
        MakePairs2_Charged3();
      } break;
      default: Cout::PrintInfo("Unknown Background mode", EInfo::kCriticalError); break;
    }
  }

  void TwoTrackAnaChain::FinishEventIdentical() {
    if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID) == 0) {
      if (fBackgroundMode == kMixedPairsID) { fMemoryMap->RejectLastEvent(fCurrentEventCollectionID); }
      return;
    }
    for (int i = 0; i < fTaskNo; i++) {
      fTask[i]->fCurrentTrack1CollectionNo = fCurrentTrack1CollectionNo;
      fTask[i]->fCurrentTrack2CollectionNo = fCurrentTrack2CollectionNo;
      fTask[i]->fCurrentTrackCollectionID  = fCurrentTrackCollectionID;
    }
    switch (fBackgroundMode) {
      case kPerfectPairsID: {
        MakePairs();
        MakePairs_Perfect();
      } break;
      case kMixedPairsID: {
        MakePairs();
        if (fMemoryMap->IsReadyToMixing(fCurrentEventCollectionID)) { MakePairs_Mixed(); }
      } break;
      case kRotatedPairsID: {
        MakePairs();
        MakePairs_Rotated();
      } break;
      case kHemispherePairsID: {
        MakePairs();
        MakePairs_Hemisphere();
      } break;
      case kNoBackgroundID: {
        MakePairs();
      } break;
      case kChargedID2: {
        MakePairs();
        // AA AB
        fCurrentTrack1CollectionNo = fCurrentTrackCollectionID;
        fCurrentTrack2CollectionNo = fCurrentTrackCollectionID + 1;
        for (int i = 0; i < fTaskNo; i++) {
          fTask[i]->fCurrentTrack1CollectionNo = fCurrentTrack1CollectionNo;
          fTask[i]->fCurrentTrack2CollectionNo = fCurrentTrack2CollectionNo;
          fTask[i]->fCurrentTrackCollectionID  = fCurrentTrackCollectionID;
        }
        if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo) == 0) return;
        if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo) == 0) return;
        MakePairs_Charged2();
      } break;
      case kChargedID3: {
        MakePairs();
        // AA BC
        fCurrentTrack1CollectionNo = fCurrentTrackCollectionID + 1;
        fCurrentTrack2CollectionNo = fCurrentTrackCollectionID + 2;
        for (int i = 0; i < fTaskNo; i++) {
          fTask[i]->fCurrentTrack1CollectionNo = fCurrentTrack1CollectionNo;
          fTask[i]->fCurrentTrack2CollectionNo = fCurrentTrack2CollectionNo;
          fTask[i]->fCurrentTrackCollectionID  = fCurrentTrackCollectionID;
        }
        if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo) == 0) return;
        if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo) == 0) return;
        MakePairs_Charged3();
      } break;
      default: Cout::PrintInfo("Unknown Background mode", EInfo::kCriticalError); break;
    }
  }

  void TwoTrackAnaChain::LinkTask(TwoTrackAna* ana) const {
    delete ana->fMemoryMap;
    ana->fMemoryMap     = fMemoryMap;
    ana->fCurrentEvent  = fCurrentEvent;
    ana->fCurrentTrack  = fCurrentTrack;
    ana->fCurrentTrack1 = fCurrentTrack1;
    ana->fCurrentTrack2 = fCurrentTrack2;
    if (fCurrentBackgroundPair) {
      delete ana->fCurrentBackgroundPair;
      ana->fCurrentBackgroundPair = fCurrentBackgroundPair;
    }
    delete ana->fCurrentSignalPair;
    ana->fCurrentSignalPair = fCurrentSignalPair;
    ana->fBackgroundMode    = fBackgroundMode;
  }

  void TwoTrackAnaChain::UnlinkTask(TwoTrackAna* ana) const {
    ana->fMemoryMap             = nullptr;
    ana->fCurrentEvent          = nullptr;
    ana->fCurrentTrack          = nullptr;
    ana->fCurrentTrack1         = nullptr;
    ana->fCurrentTrack2         = nullptr;
    ana->fCurrentBackgroundPair = nullptr;
    ana->fCurrentSignalPair     = nullptr;
  }

  void TwoTrackAnaChain::SynchronizeCutContainers(TwoTrackAna* ana, Bool_t /*end*/) const {
    ana->AddCut(EventVirtualCut(), Form("{%ix1}", fEventCollectionsNo));
    ana->fCutContainer->MakeDummyCopies(ECutUpdate::kEvent, fCutContainer, kTRUE);
    ana->AddCut(TrackVirtualCut(), Form("{%ix1}", fTrackCollectionsNo));
    ana->fCutContainer->MakeDummyCopies(ECutUpdate::kTrack, fCutContainer, kTRUE);
    ana->AddCut(TwoTrackVirtualCut(), Form("{%ix1}+sig", fTwoTrackCollectionsNo));
    ana->fCutContainer->MakeDummyCopies(ECutUpdate::kTwoTrack, fCutContainer, kTRUE);
    if (fBackgroundMode != kNoBackground) {
      ana->AddCut(TwoTrackVirtualCut(), Form("{%ix1}+bckg", fTwoTrackCollectionsNo));
      ana->fCutContainer->MakeDummyCopies(ECutUpdate::kTwoTrackBackground, fCutContainer, kTRUE);
    }
    ana->SetMixSize(fMixSize);
  }

  TwoTrackAnaChain::~TwoTrackAnaChain() {
    for (int i = 0; i < fTaskNo; i++) {
      UnlinkTask(fTask[i]);
      delete fTask[i];
    }
    delete[] fTask;
  }
}  // namespace Hal
