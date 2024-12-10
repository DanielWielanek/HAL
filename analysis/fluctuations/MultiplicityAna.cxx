/*
 * MultiplicityAna.cxx
 *
 *  Created on: 4 gru 2024
 *      Author: daniel
 */

#include "MultiplicityAna.h"

#include <TH1.h>
#include <TH3.h>
#include <utility>

#include "Const.h"
#include "CorrelationHisto.h"
#include "Cout.h"
#include "Cut.h"
#include "CutContainer.h"
#include "Package.h"
#include "Std.h"
#include "StdString.h"
#include "TrackPdgCut.h"
#include <iostream>

namespace Hal {

  MultiplicityAna::MultiplicityAna() {}

  void MultiplicityAna::SetMcPidNames(TString opt) {
    std::vector<std::pair<Int_t, TString>> labels;
    auto add = [&](Int_t pid, TString label) {
      std::pair<Int_t, TString> p;
      p.first  = pid;
      p.second = label;
      labels.push_back(p);
    };
    add(Hal::Const::PionPlusPID(), "#pi^{+}");
    add(-Hal::Const::PionPlusPID(), "#pi^{-}");
    add(Hal::Const::KaonPlusPID(), "K^{+}");
    add(-Hal::Const::KaonPlusPID(), "K^{-}");
    add(Hal::Const::ProtonPID(), "p");
    add(-Hal::Const::ProtonPID(), "#bar{p}");
    if (opt == "all") {
      add(Hal::Const::PionZeroPID(), "#pi^{0}");
      add(Hal::Const::KaonZeroShortPID(), "K^{0}_{S}");
      add(Hal::Const::NeutronPID(), "n");
      add(-Hal::Const::NeutronPID(), "#bar{n}");
      add(Hal::Const::KaonZeroLongPID(), "K^{0}_{L}");
      add(-Hal::Const::KaonZeroPID(), "K^{0}");
      add(Hal::Const::LambdaPID(), "#Lambda");
      add(-Hal::Const::LambdaPID(), "#bar{#Lambda}");
    }
    int count    = 0;
    Bool_t useim = Hal::Std::FindParam(opt, "im");
    for (auto pair : labels) {
      Hal::TrackPdgCut pid;
      pid.SetMinAndMax(pair.first);
      pid.SetCollectionID(count++);
      if (useim) {
        AddCut(pid, "im");
      } else {
        AddCut(pid);
      }
      fTrackNames.push_back(pair.second);
    }
  }

  Hal::Task::EInitFlag MultiplicityAna::Init() {
    auto status = Hal::TrackAna::Init();
    if (fEventCollectionsNo != fEventNames.size()) {
      Cout::PrintInfo("Incomatible numer of event groups and event labels", EInfo::kWarning);
      fEventNames.clear();
      for (int i = 0; i < fEventCollectionsNo; i++)
        fEventNames.push_back(Form("Event[%i]", i));
    }
    if (fTrackCollectionsNo != fTrackNames.size()) {
      Cout::PrintInfo("Incomatible numer of track groups and track labels", EInfo::kWarning);
      for (int i = 0; i < fTrackCollectionsNo; i++)
        fTrackNames.push_back(Form("Track[%i]", i));
    }
    fCounts.resize(fTrackCollectionsNo);
    fData = new HistogramManager_2_1D<TH1D>();
    std::vector<HistogramAxisConf> vec;
    vec.push_back(HistogramAxisConf("N_{tracks}", fMultiplicityBins, fMultiplicityMin, fMultiplicityMax));
    vec.push_back(HistogramAxisConf("N_{events}", fMultiplicityBins, fMultiplicityMin, fMultiplicityMax));
    fData->Init(fEventCollectionsNo, fTrackCollectionsNo, vec, "", kFALSE);
    for (int i = 0; i < fEventCollectionsNo; i++) {
      for (int j = 0; j < fTrackCollectionsNo; j++) {
        fData->At(i, j)->GetXaxis()->SetTitle(Form("N_{%s}", fTrackNames[j].Data()));
      }
    }
    if (fUseCov) {
      fCovariances.resize(fEventCollectionsNo);
      for (int i = 0; i < fEventCollectionsNo; i++) {
        fCovariances[i] = new CorrelationHisto(fTrackCollectionsNo, 1);
        for (int j = 0; j < fTrackCollectionsNo; j++) {
          fCovariances[i]->ConfigParam(j, fCovBins, fCovMin, fCovMax, Form("N_{%s}", fTrackNames[j].Data()));
        }
      };
    }

    return status;
  }

  void MultiplicityAna::ProcessTrack() { fCounts[fCurrentTrackCollectionID]++; }

  MultiplicityAna::~MultiplicityAna() {}

  Hal::Package* MultiplicityAna::Report() const {
    auto report = Hal::TrackAna::Report();
    for (int i = 0; i < fData->GetSize(); i++) {
      for (int j = 0; j < fData->Get(0)->GetSize(); j++) {
        report->AddObject(fData->At(i, j)->Clone());
      }
    }
    if (fUseCov) {
      // for (int i = 0; i < fEventCollectionsNo; i++) {
      //   for (int j = 0; j < fCovariances[j]->GetEntries(); j++) {
      //     report->AddObject(fCovariances[i]->GetHistograms()[j]);
      //   }
      // }
      for (int i = 0; i < fEventCollectionsNo; i++) {
        report->AddObject(fCovariances[i]->Clone());
      }
    }
    delete fData;
    return report;
  }

  void MultiplicityAna::ProcessEvent() {
    for (auto& i : fCounts) {
      i = 0;
    }
    Hal::TrackAna::ProcessEvent();
    int count = 0;
    for (auto i : fCounts) {
      fData->Fill(fCurrentEventCollectionID, count++, i);
    }
    if (fUseCov) { fCovariances[fCurrentEventCollectionID]->Fill(fCounts); }
  }

  void MultiplicityAna::LinkCollections() {
    Int_t trackCollections = fCutContainer->GetTrackCollectionsNo();
    Int_t eventCollections = fCutContainer->GetEventCollectionsNo();
    for (int i = 0; i < eventCollections; i++) {  // match everything with everything
      for (int j = 0; j < trackCollections; j++) {
        fCutContainer->LinkCollections(ECutUpdate::kEvent, i, ECutUpdate::kTrack, j);
      }
    }
  }

} /* namespace Hal */
