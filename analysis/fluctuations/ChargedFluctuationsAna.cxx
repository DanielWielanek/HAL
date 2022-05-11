/*
 * FluctuationsAna.cxx
 *
 *  Created on: 31 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "ChargedFluctuationsAna.h"

#include <FairLogger.h>
#include <TCollection.h>
#include <TList.h>

#include "CutContainer.h"
#include "Event.h"
#include "EventVirtualCut.h"
#include "HalStd.h"
#include "MemoryMapManager.h"
#include "Package.h"

namespace Hal {
  ChargedFluctuationsAna::ChargedFluctuationsAna() : fKaon(NULL), fPion(NULL), fProton(NULL), fCharged(NULL) {
    for (int i = 0; i < 4; i++) {
      SetHistogramAxis(1000, i);
      fNPos[i] = 0;
      fNNeg[i] = 0;
    }
  }

  void ChargedFluctuationsAna::ProcessEvent() {
    Int_t step = fCurrentEventCollectionID * 8;
    for (int j = 0; j < 4; j++)
      fNPos[j] = fNNeg[j] = 0;
    for (int i = 0; i < fMemoryMap->GetTemporaryTotalTracksNo(); i++) {
      fCurrentTrack = fCurrentEvent->GetTrack(i);
      for (int j = 0; j < 4; j++) {
        if (fCutContainer->PassTrack(fCurrentTrack, j * 2 + step)) ++fNPos[j];
        if (fCutContainer->PassTrack(fCurrentTrack, j * 2 + 1 + step)) ++fNNeg[j];
      }
    }
    fPion->Fill(fCurrentEventCollectionID, fNPos[kPion], fNNeg[kPion]);
    fKaon->Fill(fCurrentEventCollectionID, fNPos[kKaon], fNNeg[kKaon]);
    fProton->Fill(fCurrentEventCollectionID, fNPos[kProton], fNNeg[kProton]);
    fCharged->Fill(fCurrentEventCollectionID, fNPos[kCharge], fNNeg[kCharge]);
  }

  Task::EInitFlag ChargedFluctuationsAna::Init() {
    if (TrackAna::Init() == Task::EInitFlag::kFATAL) return Task::EInitFlag::kFATAL;
    fPion    = new HistogramManager_1_2D<TH2D>();
    fKaon    = new HistogramManager_1_2D<TH2D>();
    fProton  = new HistogramManager_1_2D<TH2D>();
    fCharged = new HistogramManager_1_2D<TH2D>();

    fPion->Init(fEventCollectionsNo, fBins[kPion], fMin[kPion], fMax[kPion], fBins[kPion], fMin[kPion], fMax[kPion]);
    fKaon->Init(fEventCollectionsNo, fBins[kKaon], fMin[kKaon], fMax[kKaon], fBins[kKaon], fMin[kKaon], fMax[kKaon]);
    fProton->Init(
      fEventCollectionsNo, fBins[kProton], fMin[kProton], fMax[kProton], fBins[kProton], fMin[kProton], fMax[kProton]);
    fCharged->Init(
      fEventCollectionsNo, fBins[kCharge], fMin[kCharge], fMax[kCharge], fBins[kCharge], fMin[kCharge], fMax[kCharge]);
    for (int i = 0; i < fEventCollectionsNo; i++) {
      fPion->At(i)->SetTitle(HistoTitle(i, "#pi"));
      fKaon->At(i)->SetTitle(HistoTitle(i, "K"));
      fProton->At(i)->SetTitle(HistoTitle(i, "p"));
      fCharged->At(i)->SetTitle(HistoTitle(i, "X"));

      fPion->At(i)->SetName(HistoName(i, "pions"));
      fKaon->At(i)->SetName(HistoName(i, "kaon"));
      fProton->At(i)->SetName(HistoName(i, "prtotons"));
      fCharged->At(i)->SetName(HistoName(i, "charged"));
    }
    return Task::EInitFlag::kSUCCESS;
  }

  Package* ChargedFluctuationsAna::Report() const {
    Package* report = TrackAna::Report();
    for (int i = 0; i < fEventCollectionsNo; i++) {
      TList* list = new TList();
      list->SetName(Form("Event collection [%i]", i));
      list->AddLast(fPion->At(i)->Clone());
      list->AddLast(fKaon->At(i)->Clone());
      list->AddLast(fProton->At(i)->Clone());
      list->AddLast(fCharged->At(i)->Clone());
      report->AddObject(list);
    }
    return report;
  }

  void ChargedFluctuationsAna::SetHistogramAxis(Int_t bins, Double_t min, Double_t max, Int_t flag) {
    if (flag == PionPlusID() || flag == PionMinusID()) {
      fBins[kPion] = bins;
      fMin[kPion]  = min;
      fMax[kPion]  = max;
    }
    if (flag == KaonPlusID() || flag == KaonMinusID()) {
      fBins[kKaon] = bins;
      fMin[kKaon]  = min;
      fMax[kKaon]  = max;
    }
    if (flag == AntiProtonID() || flag == ProtonID()) {
      fBins[kProton] = bins;
      fMin[kProton]  = min;
      fMax[kProton]  = max;
    }
    if (flag == PositiveChargeID() || flag == NegativeChargeID()) {
      fBins[kCharge] = bins;
      fMin[kCharge]  = min;
      fMax[kCharge]  = max;
    }
  }

  void ChargedFluctuationsAna::CheckCutContainerCollections() {
    fEventCollectionsNo = fCutContainer->GetEventCollectionsNo();
    fTrackCollectionsNo = fCutContainer->GetTrackCollectionsNo();
    if (fEventCollectionsNo == 0) { AddCut(EventVirtualCut()); }
    if (fTrackCollectionsNo == 8) {
      for (int i = 1; i < fEventCollectionsNo; i++) {
        for (int j = 0; j < 8; j++) {
          fCutContainer->ReplicateCollection(ECutUpdate::kTrackUpdate, j, 8 * i + j);
        }
      }
    } else {
      LOG(ERROR) << "wrong number of track collections, this might result in crash";
    }
  }

  TString ChargedFluctuationsAna::HistoTitle(Int_t id, TString pattern) const {
    return Form("%s [%i];dN/%s^{+};dN/d%s^{-}", pattern.Data(), id, pattern.Data(), pattern.Data());
  }

  TString ChargedFluctuationsAna::HistoName(Int_t id, TString pattern) const {
    return Form("Distribution of %s in [%i]", pattern.Data(), id);
  }

  void ChargedFluctuationsAna::SetHistogramAxis(Int_t bins, Int_t flag) {
    Double_t min = -0.5;
    Double_t max = ((Double_t) bins) + 0.5;
    if (flag == PionPlusID() || flag == PionMinusID()) {
      fBins[kPion] = bins;
      fMin[kPion]  = min;
      fMax[kPion]  = max;
    }
    if (flag == KaonPlusID() || flag == KaonMinusID()) {
      fBins[kKaon] = bins;
      fMin[kKaon]  = min;
      fMax[kKaon]  = max;
    }
    if (flag == AntiProtonID() || flag == ProtonID()) {
      fBins[kProton] = bins;
      fMin[kProton]  = min;
      fMax[kProton]  = max;
    }
    if (flag == PositiveChargeID() || flag == NegativeChargeID()) {
      fBins[kCharge] = bins;
      fMin[kCharge]  = min;
      fMax[kCharge]  = max;
    }
  }

  void ChargedFluctuationsAna::AddCut(const Cut& cut, Option_t* opt) {
    if (cut.GetCollectionID() > 7 && cut.InheritsFrom("TrackCut")) return;
    TrackAna::AddCut(cut, opt);
  }

  ChargedFluctuationsAna::~ChargedFluctuationsAna() {
    if (fPion) {
      delete fPion;
      delete fKaon;
      delete fProton;
      delete fCharged;
    }
  }
}  // namespace Hal
