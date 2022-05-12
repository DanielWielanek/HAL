/*
 * FluctuationsAna.cxx
 *
 *  Created on: 31 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "ChargedFluctuationsAna.h"

#include "Cout.h"
#include "CutContainer.h"
#include "CutsAndMonitors.h"
#include "DataFormat.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "EventVirtualCut.h"
#include "HalStd.h"
#include "MemoryMapManager.h"
#include "Package.h"
#include "TrackCut.h"

#include <TCollection.h>
#include <TList.h>
#include <TPRegexp.h>


namespace Hal {
  ChargedFluctuationsAna::ChargedFluctuationsAna() :
    fBins(1000),
    fMin(0),
    fMax(1000),
    fEventBins(1),
    fEventPar(DataFieldID::EEvent::kEventZero),
    fTrackColsHalf(0),
    fEventMin(-0.5),
    fEventMax(0.5),
    fHistogram(nullptr) {}

  void ChargedFluctuationsAna::ProcessEvent() {
    Int_t step          = fCurrentEventCollectionID * 8;
    Double_t evProperty = fCurrentEvent->GetFieldVal(fEventPar);
    for (int j = 0; j < fTrackColsHalf * 2; j++) {
      fCounts[j] = 0;
    }

    for (int i = 0; i < fMemoryMap->GetTemporaryTotalTracksNo(); i++) {
      fCurrentTrack = fCurrentEvent->GetTrack(i);
      for (int j = 0; j < fTrackColsHalf * 2; j++) {
        if (fCutContainer->PassTrack(fCurrentTrack, j + step)) ++fCounts[j];
      }
    }
    for (int j = 0; j < fTrackColsHalf * 2; j += 2) {
      fHistogram->Fill(fCurrentEventCollectionID, j, fCounts[2 * j], fCounts[2 * j + 1], evProperty);
    }
  }

  Task::EInitFlag ChargedFluctuationsAna::Init() {
    if (TrackAna::Init() == Task::EInitFlag::kFATAL) return Task::EInitFlag::kFATAL;
    fTrackColsHalf = fCutContainer->GetTrackCollectionsNo() / fCutContainer->GetEventCollectionsNo();
    if (fTrackColsHalf % 2 == 1) return kFATAL;
    fCounts.resize(fTrackColsHalf);
    fTrackColsHalf = fTrackColsHalf / 2;
    fHistogram     = new HistogramManager_2_3D<TH3D>();
    fHistogram->Init(fEventCollectionsNo, fTrackColsHalf, fBins, fMin, fMax, fBins, fMin, fMax, fEventBins, fEventMin, fEventMax);
    const Event* ev = DataFormatManager::Instance()->GetFormat(GetTaskID());
    TString parName = ev->GetFieldName(fEventPar);
    for (int i = 0; i < fEventCollectionsNo; i++) {
      TString eventName = Form("Event [%i]", i);
      if (i < fEventLabels.size()) { eventName = fEventLabels[i]; }
      for (int j = 0; j < fTrackColsHalf; j++) {
        Int_t posColl      = 2 * j;
        Int_t negColl      = 2 * j + 1;
        TString trackNameA = Form("Track [%i]", posColl);
        TString trackNameB = Form("Track [%i]", negColl);
        if (posColl < fTrackLabels.size()) { trackNameA = fTrackLabels[posColl]; }
        if (negColl < fTrackLabels.size()) { trackNameB = fTrackLabels[negColl]; }

        TH3D* h = fHistogram->At(i, j);
        h->SetTitle(Form("%s %s vs %s", eventName.Data(), trackNameA.Data(), trackNameB.Data()));
        h->SetName(h->GetTitle());
        h->GetXaxis()->SetTitle(Form("N_{%s}", trackNameA.Data()));
        h->GetYaxis()->SetTitle(Form("N_{%s}", trackNameB.Data()));
        h->GetZaxis()->SetTitle(Form("%s", parName.Data()));
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
    if (fTrackCollectionsNo % 2 == 0) {
      for (int i = 1; i < fEventCollectionsNo; i++) {
        for (int j = 0; j < fTrackCollectionsNo; j++) {
          fCutContainer->ReplicateCollection(ECutUpdate::kTrackUpdate, j, fTrackCollectionsNo * i + j);
        }
      }
    } else {
      Cout::PrintInfo("wrong number of track collections, this might result in crash", EInfo::kLessError);
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
      Cout::PrintInfo("cannot add two cuts with the same collection ID by ChargedFluctuationsAna::AddCut!", EInfo::kLessWarning);
    }
    if (TMath::Abs(pos.GetCollectionID() - neg.GetCollectionID()) != 1) {
      Cout::PrintInfo("cannot add two cuts with delta collection ID!=1 by ChargedFluctuationsAna::AddCut!", EInfo::kLessWarning);
    }
    TString option = CleanOpt(opt, -1);
    TrackAna::AddCut(pos, opt);
    TrackAna::AddCut(neg, opt);
  }

  void ChargedFluctuationsAna::SetEventprop(Int_t evProp) { fEventPar = evProp; }

  void
  ChargedFluctuationsAna::AddCutsAndMonitors(const CutsAndMonitors& posTrack, const CutsAndMonitors& negTrack, Option_t* opt) {
    if (posTrack.GetCut(0)->GetCollectionID() == negTrack.GetCut(0)->GetCollectionID()) {
      Cout::PrintInfo("cannot add two cuts with the same collection ID by NicaChargedFluctuationsAna::AddCut!",
                      EInfo::kLessWarning);
    }
    if (TMath::Abs(posTrack.GetCut(0)->GetCollectionID() - negTrack.GetCut(0)->GetCollectionID()) != 1) {
      Cout::PrintInfo("cannot add two cuts with delta collection ID!=1 by NicaChargedFluctuationsAna::AddCut!",
                      EInfo::kLessWarning);
    }
    for (int iCut = 0; iCut < posTrack.GetNCuts(); iCut++) {
      TrackAna::AddCut(*posTrack.GetCut(iCut), posTrack.GetCutOption(iCut));
    }
    for (int iCut = 0; iCut < negTrack.GetNCuts(); iCut++) {
      TrackAna::AddCut(*negTrack.GetCut(iCut), negTrack.GetCutOption(iCut));
    }
    for (int iMon = 0; iMon < posTrack.GetNCutMonitors(); iMon++) {
      TrackAna::AddCutMonitor(*posTrack.GetMonitor(iMon), posTrack.GetCutMonitorOption(iMon));
    }
    for (int iMon = 0; iMon < negTrack.GetNCutMonitors(); iMon++) {
      TrackAna::AddCutMonitor(*negTrack.GetMonitor(iMon), negTrack.GetCutMonitorOption(iMon));
    }
  }

}  // namespace Hal
