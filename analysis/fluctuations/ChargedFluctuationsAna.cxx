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
#include <TPRegexp.h>

#include "CutContainer.h"
#include "DataFormat.h"
#include "Event.h"
#include "EventVirtualCut.h"
#include "HalStd.h"
#include "MemoryMapManager.h"
#include "Package.h"
#include "TrackCut.h"

namespace Hal {
  ChargedFluctuationsAna::ChargedFluctuationsAna() :
    fBins(1000),
    fMin(0),
    fMax(1000),
    fEventBins(1),
    fEventPar(DataFieldID::EEvent::kEventZero),
    fTrackCols(0),
    fEventMin(-0.5),
    fEventMax(0.5),
    fHistogram(nullptr) {}

  void ChargedFluctuationsAna::ProcessEvent() {
    Int_t step          = fCurrentEventCollectionID * 8;
    Double_t evProperty = fCurrentEvent->GetFieldVal(fEventPar);
    for (int j = 0; j < fTrackCols; j++) {
      fCounts[j] = 0;
    }

    for (int i = 0; i < fMemoryMap->GetTemporaryTotalTracksNo(); i++) {
      fCurrentTrack = fCurrentEvent->GetTrack(i);
      for (int j = 0; j < fTrackCols; j++) {
        if (fCutContainer->PassTrack(fCurrentTrack, j + step)) ++fCounts[j];
      }
    }
    for (int j = 0; j < fTrackCols; j += 2) {
      fHistogram->Fill(fCurrentEventCollectionID, j, fCounts[2 * j], fCounts[2 * j + 1], evProperty);
    }
  }

  Task::EInitFlag ChargedFluctuationsAna::Init() {
    if (TrackAna::Init() == Task::EInitFlag::kFATAL) return Task::EInitFlag::kFATAL;
    fTrackCols = fCutContainer->GetTrackCollectionsNo() / fCutContainer->GetEventCollectionsNo();
    if (fTrackCols % 2 == 1) return Task::EInitFlag::kFATAL;
    fHistogram = new HistogramManager_2_3D<TH3D>();
    fHistogram->Init(fEventCollectionsNo, fTrackCols / 2, fBins, fMin, fMax, fBins, fMin, fMax, fEventBins, fEventMin, fEventMax);
    for (int i = 0; i < fEventCollectionsNo; i++) {
      TString eventName = Form("Event [%i]", i);
      if (i < fEventLabels.size()) { eventName = fEventLabels[i]; }
      for (int j = 0; j < fTrackCols; j += 2) {
        TString trackNameA = Form("Track [%i]", j);
        TString trackNameB = Form("Track [%i]", j);
        if (j < fTrackLabels.size()) { trackNameA = fTrackLabels[2 * j]; }
        if (j + 1 < fTrackLabels.size()) { trackNameA = fTrackLabels[2 * j + 1]; }
        TH3D* h = fHistogram->At(i, j);
        h->SetTitle(Form("%s %s vs %s", eventName.Data(), trackNameA.Data(), trackNameB.Data()));
        h->GetXaxis()->SetTitle(Form("N_{%s}", trackNameA.Data()));
        h->GetYaxis()->SetTitle(Form("N_{%s}", trackNameB.Data()));
        h->GetZaxis()->SetTitle(Form("%s", fCurrentEvent->GetFieldName(fEventPar).Data()));
      }
    }
    return Task::EInitFlag::kSUCCESS;
  }

  Package* ChargedFluctuationsAna::Report() const {
    Package* report = TrackAna::Report();
    for (int i = 0; i < fHistogram->GetSize(); i++) {
      for (int j = 0; j < fHistogram->Get(i)->GetSize(); j++) {
        report->AddObject(fHistogram->At(i, j));
      }
    }
    return report;
  }

  void ChargedFluctuationsAna::SetTrackHistogramAxis(Int_t bins, Double_t min, Double_t max) {
    fBins = bins;
    fMin  = min;
    fMax  = max;
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


  void ChargedFluctuationsAna::AddCut(const Cut& cut, Option_t* opt) {
    if (cut.InheritsFrom("TrackCut")) return;
    TrackAna::AddCut(cut, opt);
  }

  ChargedFluctuationsAna::~ChargedFluctuationsAna() {
    if (fHistogram) { delete fHistogram; }
  }


  void ChargedFluctuationsAna::SetEventLabels(const std::initializer_list<TString>& init) {
    std::initializer_list<TString>::iterator it;
    Int_t pos = 0;
    for (it = init.begin(); it != init.end(); ++it) {
      fEventLabels.push_back(*it);
    }
  }

  void ChargedFluctuationsAna::SetTrackLabels(const std::initializer_list<TString>& init) {
    std::initializer_list<TString>::iterator it;
    Int_t pos = 0;
    for (it = init.begin(); it != init.end(); ++it) {
      fTrackLabels.push_back(*it);
    }
  }

  void ChargedFluctuationsAna::SetEventHistogramAxis(Int_t bins, Double_t min, Double_t max) {
    fEventBins = bins;
    fEventMin  = min;
    fEventMax  = max;
  }


  TString ChargedFluctuationsAna::CleanOpt(Option_t* opt, Int_t cut) const {
    TString newOpt = opt;
    TPRegexp reg("{[x0-9]*}");
    reg.Substitute(newOpt, "", "g");
    while (newOpt.Contains("++"))
      newOpt = newOpt.ReplaceAll("++", "+");
    if (newOpt.BeginsWith("+")) newOpt = newOpt(1, newOpt.Length() - 1);
    if (newOpt.EndsWith("+")) newOpt = newOpt(0, newOpt.Length() - 1);
    if (cut < 0) return newOpt;
    if (newOpt.Length() > 0)
      newOpt = Form("%s+{%i}", newOpt.Data(), cut);
    else
      newOpt = Form("{%i}", cut);
    return newOpt;
  }


  void ChargedFluctuationsAna::AddCut(const TrackCut& pos, const TrackCut& neg, Option_t* opt) {
    if (pos.GetCollectionID() == neg.GetCollectionID()) {
      LOG(WARNING) << "cannot add two cuts with the same collection ID by ChargedFluctuationsAna::AddCut!";
    }
    if (TMath::Abs(pos.GetCollectionID() - neg.GetCollectionID()) != 1) {
      LOG(WARNING) << "cannot add two cuts with delta collection ID!=1 by ChargedFluctuationsAna::AddCut!";
    }
    TString option = CleanOpt(opt, -1);
    AddCut(pos, opt);
    AddCut(neg, opt);
  }

  void ChargedFluctuationsAna::SetEventprop(Int_t evProp) { fEventPar = evProp; }
}  // namespace Hal
