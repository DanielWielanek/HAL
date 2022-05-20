/*
 * TwoTrackAna.cxx
 *
 *  Created on: 27-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackAna.h"

#include "Cout.h"
#include "CutCollection.h"
#include "CutContainer.h"
#include "MemoryMapManager.h"
#include "Parameter.h"
#include "TwoTrack.h"
#include "TwoTrackVirtualCut.h"

#include <TString.h>


namespace Hal {
  TwoTrackAna::TwoTrackAna(Bool_t use_background) :
    MultiTrackAna(),
    fNonIdIsSet(kFALSE),
    fSignedBoth(kTRUE),
    fIdentical(kTRUE),
    fSwapPair(kTRUE),
    fCurrentTrack2CollectionNo(0),
    fCurrentTrack1CollectionNo(0),
    fCurrentTrack1(NULL),
    fCurrentTrack2(NULL),
    fBackgroundMode(kNoBackground),
    fCurrentPairCollectionID(0),
    fTwoTrackCollectionsNo(0),
    fTwoTrackCollectionsNoBackground(0),
    fCurrentSignalPair(NULL),
    fCurrentBackgroundPair(NULL) {
    if (use_background) {
      fTiers = ECutUpdate::kTwoTrackBackground;
    } else {
      fTiers = ECutUpdate::kTwoTrack;
    }
    fMixSize = 1;
    AddTags("twotrack");
  }

  Package* TwoTrackAna::Report() const {
    Package* pack = TrackAna::Report();
    AddToAnaMetadata(pack, new ParameterInt("MixSize", fMixSize));
    TString id;
    switch (fBackgroundMode) {
      case kPerfectPairsID: id = "Perfect"; break;
      case kPerfectPairsNID: id = "Perfect"; break;
      case kMixedPairsID: id = "Mixed"; break;
      case kMixedPairsNID: id = "Mixed"; break;
      case kRotatedPairsID: id = "Rotated"; break;
      case kRotatedPairsNID: id = "Rotated"; break;
      case kHemispherePairsID: id = "Hemisphere"; break;
      case kHemispherePairsNID: id = "Hemisphere"; break;
      case kNoBackgroundID: id = "No"; break;
      case kNoBackgroundNID: id = "No"; break;
      case kChargedID2: id = "Opposite charges 0/0+"; break;
      case kChargedID3: id = "Opposite charges 0/+-"; break;
      case kChargedNID2:
        if (fSignedBoth)
          id = "Opposite charges +-/++ --";
        else
          id = "Opposite charges +-/++";
        break;
      case kChargedNID3: id = "Opposite charges +-/0"; break;
      default: id = "Unknown"; break;
    }
    AddToAnaMetadata(pack, new ParameterString("Background", id));
    if (fIdentical)
      AddToAnaMetadata(pack, new ParameterString("Signal", "Identical"));
    else
      AddToAnaMetadata(pack, new ParameterString("Signal", "Non-identical"));
    return pack;
  }

  void TwoTrackAna::SetTags() {
    switch (fBackgroundMode) {
      case kPerfectPairsID: AddTags("pefect_background"); break;
      case kPerfectPairsNID: AddTags("pefect_background"); break;
      case kMixedPairsID: AddTags("mixed_background"); break;
      case kMixedPairsNID: AddTags("mixed_background"); break;
      case kRotatedPairsID: AddTags("rotated_background"); break;
      case kRotatedPairsNID: AddTags("rotated_background"); break;
      case kHemispherePairsID: AddTags("hemisphere_background"); break;
      case kHemispherePairsNID: AddTags("hemishpere_background"); break;
      case kNoBackgroundID: AddTags("no_background"); break;
      case kNoBackgroundNID: AddTags("no_background"); break;
      case kChargedID2: AddTags("charged_background"); break;
      case kChargedID3: AddTags("charged_background"); break;
      case kChargedNID2: AddTags("charged_background"); break;
      case kChargedNID3: AddTags("charged_background"); break;
      default: break;
    }
    if (fIdentical) {
      AddTags("id identical");
    } else {
      AddTags("nonid nonidentical");
    }
  }

  Task::EInitFlag TwoTrackAna::Init() {
    Task::EInitFlag in = MultiTrackAna::Init();
    SetTags();
    fCurrentSignalPair     = new TwoTrack();
    fCurrentBackgroundPair = new TwoTrack();
    fCurrentSignalPair->SetPairType(TwoTrack::kSignal);
    fCurrentBackgroundPair->SetPairType(GetPairType(fBackgroundMode));
    fCurrentBackgroundPair->SetSame(IdenticalParticles());
    fCurrentSignalPair->SetSame(IdenticalParticles());
    switch (fBackgroundMode) {
      case kChargedID2: fCurrentBackgroundPair->SetSame(kFALSE); break;
      case kChargedID3: fCurrentBackgroundPair->SetSame(kTRUE); break;
      case kChargedNID2: fCurrentBackgroundPair->SetSame(kTRUE); break;
      case kChargedNID3: fCurrentBackgroundPair->SetSame(kTRUE); break;
      default: break;
    }
    if (fIdentical) {
      Cout::PrintInfo("IDENTICAL two track analysis enabled", EInfo::kLessInfo);
    } else {
      Cout::PrintInfo("NON-IDENTICAL two track analysis enabled", EInfo::kLessInfo);
    }
    return in;
  }

  void TwoTrackAna::FinishTask() { TrackAna::FinishTask(); }

  void TwoTrackAna::SetOption(Option_t* option) {
    TString opt = option;
    if (opt.EqualTo("nonid")) {
      fIdentical  = kFALSE;
      fNonIdIsSet = kTRUE;
    } else if (opt.EqualTo("id")) {
      fIdentical  = kTRUE;
      fNonIdIsSet = kTRUE;
    } else if (opt.EqualTo(TwoTrackAna::BackgroundOptionMixed())) {
      fBackgroundMode = kMixedPairs;
      fTiers          = ECutUpdate::kTwoTrackBackground;
    } else if (opt.EqualTo(TwoTrackAna::BackgroundOptionPerfect())) {
      fBackgroundMode = kPerfectPairs;
      fTiers          = ECutUpdate::kTwoTrackBackground;
    } else if (opt.EqualTo(TwoTrackAna::BackgroundOptionRotated())) {
      fBackgroundMode = kRotatedPairs;
      fTiers          = ECutUpdate::kTwoTrackBackground;
    } else if (opt.EqualTo(TwoTrackAna::BackgroundOptionHemisphere())) {
      fBackgroundMode = kHemispherePairs;
      fTiers          = ECutUpdate::kTwoTrackBackground;
    } else if (opt.EqualTo(TwoTrackAna::BackgroundOptionNoBackground())) {
      fBackgroundMode = kNoBackground;
      fTiers          = ECutUpdate::kTwoTrack;
    } else if (opt.EqualTo(TwoTrackAna::BackgroundOptionCharge())) {
      fBackgroundMode = kCharged;
      fTiers          = ECutUpdate::kTwoTrackBackground;
    } else if (opt.EqualTo("disable:signs_sum")) {
      fSignedBoth = kFALSE;
    } else {
      MultiTrackAna::SetOption(option);
    }
  }

  void TwoTrackAna::SetMixSize(Int_t mixsize) {
    if (mixsize > 0) fMixSize = mixsize;
    if (mixsize > 10) {
#ifdef HAL_DEBUG
      Cout::PrintInfo("Mix size >10", EInfo::kLessInfo);
#endif
    }
  }

  void TwoTrackAna::InitNewCutContainer() { MultiTrackAna::InitNewCutContainer(); }

  void TwoTrackAna::CheckCutContainerCollections() {
    TrackAna::CheckCutContainerCollections();
    if (fCutContainer->GetTwoTrackCollectionsNo() == 0) { AddCut(TwoTrackVirtualCut(), "fast+sig"); }
    if (fBackgroundMode != kNoBackground) {
      if (fCutContainer->GetTwoTrackCollectionsBackgroundNo() == 0) { AddCut(TwoTrackVirtualCut(), "fast+bckg"); }
    }
    fTwoTrackCollectionsNo           = fCutContainer->GetTwoTrackCollectionsNo();
    fTwoTrackCollectionsNoBackground = fCutContainer->GetTwoTrackCollectionsBackgroundNo();
    Int_t trackTrig                  = fCutContainer->GetTrackCollectionsNo();
    switch (fBackgroundMode) {
      case kCharged: {
        if (trackTrig < 2) {
          Cout::PrintInfo("Not enough track collections", EInfo::kLessWarning);
        } else if (trackTrig > 3) {
          Cout::PrintInfo("Too much track collections (>3) fixing ...", EInfo::kLessWarning);
          for (int i = 3; i < trackTrig; i++)
            fCutContainer->RemoveCollection(ECutUpdate::kTrack, i);
          trackTrig = fCutContainer->GetTrackCollectionsNo();
        }
        if (fIdentical) {
          if (trackTrig == 2) fBackgroundMode = kChargedID2;
          if (trackTrig == 3) fBackgroundMode = kChargedID3;
        } else {
          if (trackTrig == 2) fBackgroundMode = kChargedNID2;
          if (trackTrig == 3) fBackgroundMode = kChargedNID3;
        }
      } break;
      default: {
        if (IdenticalParticles()) {
          if (trackTrig > 1) {
            Cout::PrintInfo("Too much track collections (>1), fixing ..", EInfo::kLessWarning);
            for (int i = 1; i < trackTrig; i++)
              fCutContainer->RemoveCollection(ECutUpdate::kTrack, i);
            trackTrig = 1;
          }
          switch (fBackgroundMode) {
            case kPerfectPairs: fBackgroundMode = kPerfectPairsID; break;
            case kHemispherePairs: fBackgroundMode = kHemispherePairsID; break;
            case kRotatedPairs: fBackgroundMode = kRotatedPairsID; break;
            case kMixedPairs: fBackgroundMode = kMixedPairsID; break;
            default: fBackgroundMode = kNoBackgroundID; break;
          }
        } else {
          if (trackTrig > 2) {
            Cout::PrintInfo("To much track collections (more than 1) fixing", EInfo::kLessWarning);
            for (int i = 2; i < trackTrig; i++)
              fCutContainer->RemoveCollection(ECutUpdate::kTrack, i);
            trackTrig = 2;
          }
          switch (fBackgroundMode) {
            case kPerfectPairs: fBackgroundMode = kPerfectPairsNID; break;
            case kHemispherePairs: fBackgroundMode = kHemispherePairsNID; break;
            case kRotatedPairs: fBackgroundMode = kRotatedPairsNID; break;
            case kMixedPairs: fBackgroundMode = kMixedPairsNID; break;
            default: fBackgroundMode = kNoBackgroundNID; break;
          }
        }
      } break;
    }
  }

  void TwoTrackAna::LinkCollections() {
    Int_t eventTrig = fCutContainer->GetEventCollectionsNo();
    Int_t trackTrig = fCutContainer->GetTrackCollectionsNo();
    Int_t twotrackT = fCutContainer->GetTwoTrackCollectionsNo();
    Int_t twoTrackB = fCutContainer->GetTwoTrackCollectionsBackgroundNo();
    for (int i = 0; i < eventTrig; i++) {
      for (int j = 0; j < trackTrig; j++) {
        fCutContainer->LinkCollections(ECutUpdate::kEvent, i, ECutUpdate::kTrack, j);
      }
    }
    switch (fBackgroundMode) {
      case kChargedID2: {
        for (int j = 0; j < twotrackT; j++) {  // AA / AB
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 0, ECutUpdate::kTwoTrack,
                                         j);  // A -> All
        }
        for (int j = 0; j < twoTrackB; j++) {
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 0, ECutUpdate::kTwoTrackBackground, j);  // A -> All
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 1, ECutUpdate::kTwoTrackBackground, j);  // B -> All
        }

      } break;
      case kChargedID3: {  // AA BC
        for (int j = 0; j < twotrackT; j++) {
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 0, ECutUpdate::kTwoTrack,
                                         j);  // A - >All
        }
        for (int j = 0; j < twoTrackB; j++) {
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 1, ECutUpdate::kTwoTrackBackground, j);  // B - > All
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 2, ECutUpdate::kTwoTrackBackground, j);  // C- > All
        }
      } break;
      case kChargedNID2: {  // AB AA+ BB
        for (int j = 0; j < twotrackT; j++) {
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 0, ECutUpdate::kTwoTrack,
                                         j);  // A - >All
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 1, ECutUpdate::kTwoTrack,
                                         j);  // B - >All
        }
        for (int j = 0; j < twoTrackB; j++) {
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 0, ECutUpdate::kTwoTrackBackground, j);  // A - >All
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 1, ECutUpdate::kTwoTrackBackground, j);  // B - >All
        }
      } break;
      case kChargedNID3: {  // AB CC
        for (int j = 0; j < twotrackT; j++) {
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 0, ECutUpdate::kTwoTrack,
                                         j);  // A -> All
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 1, ECutUpdate::kTwoTrack,
                                         j);  // B - >All
        }
        for (int j = 0; j < twoTrackB; j++) {
          fCutContainer->LinkCollections(ECutUpdate::kTrack, 2, ECutUpdate::kTwoTrackBackground, j);  // C - >All
        }
      } break;
      default: {
        for (int i = 0; i < trackTrig; i++) {
          for (int j = 0; j < twotrackT; j++) {
            fCutContainer->LinkCollections(ECutUpdate::kTrack, i, ECutUpdate::kTwoTrack, j);
            if (fBackgroundMode != kNoBackgroundID && fBackgroundMode != kNoBackgroundNID)
              fCutContainer->LinkCollections(ECutUpdate::kTrack,
                                             i,
                                             ECutUpdate::kTwoTrackBackground,
                                             j);  // second layer is for background
          }
        }
      } break;
    }
  }

  void TwoTrackAna::FinishEventIdentical() {
    if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID) == 0) {
      if (fBackgroundMode == kMixedPairsID) { fMemoryMap->RejectLastEvent(fCurrentEventCollectionID); }
      return;
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
        if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo) == 0) return;
        if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo) == 0) return;
        MakePairs_Charged2();
      } break;
      case kChargedID3: {
        MakePairs();
        // AA BC
        fCurrentTrack1CollectionNo = fCurrentTrackCollectionID + 1;
        fCurrentTrack2CollectionNo = fCurrentTrackCollectionID + 2;
        if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo) == 0) return;
        if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo) == 0) return;
        MakePairs_Charged3();
      } break;
      default: Cout::PrintInfo("Unknown Background mode", EInfo::kImportantWarning); break;
    }
  }

  void TwoTrackAna::FinishEventNonIdentical() {
    fCurrentTrack1CollectionNo = fCurrentTrackCollectionID;
    fCurrentTrack2CollectionNo = fCurrentTrackCollectionID + 1;
    Int_t tr1                  = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo);
    Int_t tr2                  = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo);
    if (tr1 == 0 || tr2 == 0) {
      if (fBackgroundMode == kMixedPairsNID) { fMemoryMap->RejectLastEvent(fCurrentEventCollectionID); }
      return;
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
        if (fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID) == 0) return;
        MakePairs2_Charged3();
      } break;

      default: Cout::PrintInfo("Unknown Background mode", EInfo::kImportantWarning); break;
    }
  }

  void TwoTrackAna::BuildPair(TwoTrack* pair) { pair->FastBuild(fCurrentTrack1, fCurrentTrack2); }

  void TwoTrackAna::BuildPairSwapped(TwoTrack* pair) {
#ifdef DISABLE_TWO_TRACK_SWAPPING
    pair->FastBuild(fCurrentTrack1, fCurrentTrack2);
#else
    if (fSwapPair) {
      fSwapPair = kFALSE;
      pair->FastBuild(fCurrentTrack1, fCurrentTrack2);
    } else {
      fSwapPair = kTRUE;
      pair->FastBuild(fCurrentTrack2, fCurrentTrack1);
    }
#endif
  }

  void TwoTrackAna::ProcessEvent() {
    MultiTrackAna::ProcessEvent();
    fCurrentTrackCollectionID = 0;
    if (fIdentical) {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("Finish identical event with %i tracks",
                           fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID)),
                      EInfo::kLessInfo);
#endif
      FinishEventIdentical();
    } else {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("Finish non-identical event with  %i %i tracks",
                           fMemoryMap->GetTracksNo(fCurrentEventCollectionID, 0),
                           fMemoryMap->GetTracksNo(fCurrentEventCollectionID, 1)),
                      EInfo::kLessInfo);
#endif
      FinishEventNonIdentical();
    }
  }

  void TwoTrackAna::MakePairs_Charged2() {
    // process signed background if main function is from identical tracks
    // background is made from non-identical tracks
    Int_t nTrackA             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo);
    Int_t nTrackB             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrackCollectionID);
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    for (int i = 0; i < nTrackA; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack1CollectionNo, i);
      for (int j = 0; j < nTrackB; j++) {
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack2CollectionNo, j);
        BuildPair(fCurrentBackgroundPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(k);
          if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair_Charged2(); }
        }
      }
    }
  }

  void TwoTrackAna::MakePairs2_Charged2() {
    // process signed background if main function is from non-identical tracks
    // background is made from identical tracks
    fCurrentTrackCollectionID = fCurrentTrack1CollectionNo;
    Int_t nTrack              = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrack1CollectionNo);  //
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    for (int i = 0; i < nTrack; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack1CollectionNo, i);
      for (int j = i + 1; j < nTrack; j++) {
        // if(j>=i)break;
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack1CollectionNo, j);
        BuildPairSwapped(fCurrentBackgroundPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(k);
          if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, track_cuts->GetNextAddrBackround(k))) {
            ProcessPair2_Charged2();
          }
        }
      }
    }
    if (fSignedBoth) {
      fCurrentTrackCollectionID = fCurrentTrack2CollectionNo;
      nTrack                    = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo);
      for (int i = 0; i < nTrack; i++) {
        fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack2CollectionNo, i);
        for (int j = i + 1; j < nTrack; j++) {
          // if(j>=i)break;
          fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack2CollectionNo, j);
          BuildPairSwapped(fCurrentBackgroundPair);
          for (int k = 0; k < tt_cut_no; k++) {
            fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(k);
            if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, track_cuts->GetNextAddrBackround(k))) {
              ProcessPair2_Charged2();
            }
          }
        }
      }
    }
  }

  void TwoTrackAna::MakePairs_Charged3() {
    Int_t nTrackA             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo);
    Int_t nTrackB             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrack1CollectionNo);
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    for (int i = 0; i < nTrackA; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack1CollectionNo, i);
      for (int j = 0; j < nTrackB; j++) {
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack2CollectionNo, j);
        BuildPair(fCurrentBackgroundPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(k);
          if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair_Charged3(); }
        }
      }
    }
  }

  void TwoTrackAna::MakePairs2_Charged3() {
    Int_t nTrack              = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrackCollectionID);  //
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    for (int i = 0; i < nTrack; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, i);
      for (int j = i + 1; j < nTrack; j++) {
        // if(j>=i)break;
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, j);
        BuildPairSwapped(fCurrentBackgroundPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(k);
          if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair2_Charged3(); }
        }
      }
    }
  }

  TwoTrackAna::~TwoTrackAna() {}
  //----- PROCES Event Mixing

  //--------- ANALISE IDENTICAL EVENTS DIFFERENT PARTICLES

  void TwoTrackAna::MakePairs() {
    Int_t nTrack              = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrackCollectionID);  //
    Int_t tt_cut_no           = track_cuts->GetNextNo();
#ifdef HAL_DEBUG
    Cout::PrintInfo("Making pairs", EInfo::kLessInfo);
#endif
    for (int i = 0; i < nTrack; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, i);
      for (int j = i + 1; j < nTrack; j++) {
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, j);
        BuildPairSwapped(fCurrentSignalPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddr(k);
          if (fCutContainer->PassSignalPair(fCurrentSignalPair, track_cuts->GetNextAddr(k))) { ProcessPair(); }
        }
      }
    }
  }
  //---------- ANALISE NON IDENTICAL PARTICLES

  void TwoTrackAna::MakePairs2() {
    Int_t nTrackA             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo);
    Int_t nTrackB             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrack1CollectionNo);
    Int_t tt_cut_no           = track_cuts->GetNextNo();
    for (int i = 0; i < nTrackA; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack1CollectionNo, i);
      for (int j = 0; j < nTrackB; j++) {
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack2CollectionNo, j);
        BuildPair(fCurrentSignalPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddr(k);
          if (fCutContainer->PassSignalPair(fCurrentSignalPair, fCurrentPairCollectionID)) { ProcessPair2(); }
        }
      }
    }
  }

  //--------- MIX IDENTICAL PARTICLES

  void TwoTrackAna::MakePairs_Mixed() {
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrackCollectionID);
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    Int_t nTrackA             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID);
#ifdef HAL_DEBUG
    Cout::PrintInfo("Mixing event", EInfo::kLessInfo);
#endif
    for (int l = 0; l < fMixSize; l++) {
      if (l == fMemoryMap->GetCounter(fCurrentEventCollectionID)) continue;
      Int_t nTrackB = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID, l);
      for (int i = 0; i < nTrackA; i++) {
        fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, i);
        for (int j = 0; j < nTrackB; j++) {
          fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, l, j);
          BuildPairSwapped(fCurrentBackgroundPair);
          for (int m = 0; m < tt_cut_no; m++) {
            fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(m);
            if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair_Mixed(); }
          }
        }
      }
    }
  }

  void TwoTrackAna::MakePairs2_Mixed() {
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrack1CollectionNo);
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    Int_t nTrackA             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo);
    for (int l = 0; l < fMixSize; l++) {
      if (l == fMemoryMap->GetCounter(fCurrentEventCollectionID)) continue;
      Int_t nTrackB = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo, l);
      for (int i = 0; i < nTrackA; i++) {
        fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack1CollectionNo, i);
        for (int j = 0; j < nTrackB; j++) {
          fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack2CollectionNo, l, j);
          BuildPair(fCurrentBackgroundPair);
          for (int m = 0; m < tt_cut_no; m++) {
            fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(m);
            if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair2_Mixed(); }
          }
        }
      }
    }
    nTrackA = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo);
    for (int l = 0; l < fMixSize; l++) {
      if (l == fMemoryMap->GetCounter(fCurrentEventCollectionID)) continue;
      Int_t nTrackB = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo, l);
      for (int i = 0; i < nTrackA; i++) {
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack2CollectionNo, i);
        for (int j = 0; j < nTrackB; j++) {
          fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack1CollectionNo, l, j);
          BuildPair(fCurrentBackgroundPair);
          for (int m = 0; m < tt_cut_no; m++) {
            fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(m);
            if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair2_Mixed(); }
          }
        }
      }
    }
  }

  void TwoTrackAna::MakePairs_Perfect() {
    Int_t nTrack              = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrackCollectionID);  //
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    //	fMemoryMap->PrintMap(collection_no);
    for (int i = 0; i < nTrack; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, i);
      for (int j = i + 1; j < nTrack; j++) {
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, j);
        BuildPairSwapped(fCurrentBackgroundPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(k);
          if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair_Perfect(); }
        }
      }
    }
  }

  void TwoTrackAna::MakePairs2_Perfect() {
    Int_t nTrackA             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo);
    Int_t nTrackB             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrack1CollectionNo);
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    for (int i = 0; i < nTrackA; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack1CollectionNo, i);
      for (int j = 0; j < nTrackB; j++) {
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack2CollectionNo, j);
        BuildPair(fCurrentBackgroundPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(k);
          if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair2_Perfect(); }
        }
      }
    }
  }

  void TwoTrackAna::MakePairs_Rotated() {
    Int_t nTrack              = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrackCollectionID);  //
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    //	fMemoryMap->PrintMap(collection_no);
    for (int i = 0; i < nTrack; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, i);
      for (int j = i + 1; j < nTrack; j++) {
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, j);
        BuildPairSwapped(fCurrentBackgroundPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(k);
          if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair_Rotated(); }
        }
      }
    }
  }

  void TwoTrackAna::MakePairs2_Rotated() {
    Int_t nTrackA             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo);
    Int_t nTrackB             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrack1CollectionNo);
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    for (int i = 0; i < nTrackA; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack1CollectionNo, i);
      for (int j = 0; j < nTrackB; j++) {
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack2CollectionNo, j);
        BuildPair(fCurrentBackgroundPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(k);
          if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair2_Rotated(); }
        }
      }
    }
  }

  void TwoTrackAna::MakePairs_Hemisphere() {
    Int_t nTrack              = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrackCollectionID);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrackCollectionID);  //
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    for (int i = 0; i < nTrack; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, i);
      for (int j = i + 1; j < nTrack; j++) {
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrackCollectionID, j);
        BuildPairSwapped(fCurrentBackgroundPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(k);
          if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair_Hemisphere(); }
        }
      }
    }
  }

  void TwoTrackAna::MakePairs2_Hemisphere() {
    Int_t nTrackA             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack1CollectionNo);
    Int_t nTrackB             = fMemoryMap->GetTracksNo(fCurrentEventCollectionID, fCurrentTrack2CollectionNo);
    CutCollection* track_cuts = fCutContainer->GetTrackCollection(fCurrentTrack1CollectionNo);
    Int_t tt_cut_no           = track_cuts->GetNextNoBackround();
    for (int i = 0; i < nTrackA; i++) {
      fCurrentTrack1 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack1CollectionNo, i);
      for (int j = 0; j < nTrackB; j++) {
        fCurrentTrack2 = fMemoryMap->GetTrack(fCurrentEventCollectionID, fCurrentTrack2CollectionNo, j);
        BuildPair(fCurrentBackgroundPair);
        for (int k = 0; k < tt_cut_no; k++) {
          fCurrentPairCollectionID = track_cuts->GetNextAddrBackround(k);
          if (fCutContainer->PassBackgroundPair(fCurrentBackgroundPair, fCurrentPairCollectionID)) { ProcessPair2_Hemisphere(); }
        }
      }
    }
  }

  TwoTrackAna::TwoTrackAna(const TwoTrackAna& ana) :
    MultiTrackAna(ana),
    fNonIdIsSet(ana.fNonIdIsSet),
    fSignedBoth(ana.fSignedBoth),
    fIdentical(ana.fIdentical),
    fSwapPair(ana.fSwapPair),
    fCurrentTrack2CollectionNo(0),
    fCurrentTrack1CollectionNo(0),
    fCurrentTrack1(nullptr),
    fCurrentTrack2(nullptr),
    fBackgroundMode(ana.fBackgroundMode),
    fCurrentPairCollectionID(0),
    fTwoTrackCollectionsNo(0),
    fTwoTrackCollectionsNoBackground(0),
    fCurrentSignalPair(nullptr),
    fCurrentBackgroundPair(nullptr) {}

  TwoTrack::PairType TwoTrackAna::GetPairType(EAnaMode mode) {
    switch (mode) {
      case kPerfectPairs: return TwoTrack::kBackground; break;
      case kPerfectPairsID: return TwoTrack::kBackground; break;
      case kPerfectPairsNID: return TwoTrack::kBackground; break;
      case kMixedPairs: return TwoTrack::kMixed; break;
      case kMixedPairsID: return TwoTrack::kMixed; break;

      case kMixedPairsNID: return TwoTrack::kMixed; break;
      case kRotatedPairs: return TwoTrack::kRotated; break;
      case kRotatedPairsID: return TwoTrack::kRotated; break;
      case kRotatedPairsNID: return TwoTrack::kRotated; break;
      case kHemispherePairs: return TwoTrack::kHemishpere; break;

      case kHemispherePairsID: return TwoTrack::kHemishpere; break;
      case kHemispherePairsNID: return TwoTrack::kHemishpere; break;
      default: return TwoTrack::kOther; break;
    }
    return TwoTrack::kOther;
  }

  TwoTrackAna& TwoTrackAna::operator=(const TwoTrackAna& other) {
    if (this != &other) {
      MultiTrackAna::operator    =(other);
      fNonIdIsSet                = other.fNonIdIsSet;
      fSignedBoth                = other.fSignedBoth;
      fIdentical                 = other.fIdentical;
      fSwapPair                  = other.fSwapPair;
      fCurrentTrack1CollectionNo = other.fCurrentTrack1CollectionNo;
      fCurrentTrack2CollectionNo = other.fCurrentTrack2CollectionNo;
      if (other.fCurrentTrack) { fCurrentTrack1 = other.fCurrentTrack1; }
      if (other.fCurrentTrack2) { fCurrentTrack2 = other.fCurrentTrack2; }
    }
    return *this;
  }
}  // namespace Hal
//---------------------------------------------------------------------------------------
