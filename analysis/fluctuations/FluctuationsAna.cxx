/*
 * FluctuationsAna.cxx
 *
 *  Created on: 18 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FluctuationsAna.h"

#include <TCollection.h>
#include <TH1.h>
#include <TList.h>
#include <TString.h>

#include "CutCollection.h"
#include "CutContainer.h"
#include "Event.h"
#include "EventVirtualCut.h"
#include "Std.h"
#include "MemoryMapManager.h"
#include "Package.h"

namespace Hal {
  FluctuationsAna::FluctuationsAna() :
    fArray(), fBins(1001), fTackCollectionsPerEvent(0), fMin(-0.5), fMax(100.5), fHistograms(NULL) {}

  void FluctuationsAna::CheckCutContainerCollections() {
    fEventCollectionsNo      = fCutContainer->GetEventCollectionsNo();
    fTrackCollectionsNo      = fCutContainer->GetTrackCollectionsNo();
    fTackCollectionsPerEvent = fTrackCollectionsNo;
    if (fEventCollectionsNo == 0) { AddCut(EventVirtualCut()); }
    for (int i = 1; i < fEventCollectionsNo; i++) {
      for (int j = 0; j < fTackCollectionsPerEvent; j++) {
        fCutContainer->ReplicateCollection(ECutUpdate::kTrack, j, fTackCollectionsPerEvent * i + j);
      }
    }
    fTrackCollectionsNo = fCutContainer->GetTrackCollectionsNo();
  }

  void FluctuationsAna::ProcessEvent() {
    for (int i = 0; i < fTackCollectionsPerEvent; i++)
      fArray[i] = 0;
    CutCollection* cont = fCutContainer->GetEventCollection(fCurrentEventCollectionID);
    for (int i = 0; i < fMemoryMap->GetTemporaryTotalTracksNo(); i++) {
      fCurrentTrack = fCurrentEvent->GetTrack(i);
      for (int j = 0; j < cont->GetNextNo(); j++) {
        fCurrentTrackCollectionID = cont->GetNextAddr(j);
        if (fCutContainer->PassTrack(fCurrentTrack, fCurrentTrackCollectionID)) { fArray[j]++; }
      }
    }
    for (int i = 0; i < fTackCollectionsPerEvent; i++) {
      fHistograms->Fill(fCurrentEventCollectionID, i, fArray[i]);
    }
  }

  Task::EInitFlag FluctuationsAna::Init() {
    if (TrackAna::Init() == Task::EInitFlag::kFATAL) return Task::EInitFlag::kFATAL;
    fHistograms = new HistogramManager_2_1D<TH1D>();
    fHistograms->Init(fEventCollectionsNo, fTackCollectionsPerEvent, fBins, fMin, fMax, kFALSE);
    fArray.MakeBigger(fEventCollectionsNo);
    return Task::EInitFlag::kSUCCESS;
  }

  void FluctuationsAna::SetHistogramAxis(Int_t bins, Double_t min, Double_t max) {
    fBins = bins;
    fMin  = min;
    fMax  = max;
  }

  void FluctuationsAna::SetHistogramAxis(Int_t bins) {
    Double_t min = -0.5;
    Double_t max = ((Double_t) bins) + 0.5;
    SetHistogramAxis(bins + 1, min, max);
  }

  Package* FluctuationsAna::Report() const {
    Package* report = TrackAna::Report();
    for (int i = 0; i < fEventCollectionsNo; i++) {
      TList* list = new TList();
      list->SetName(Form("Event collection [%i]", i));
      for (int j = 0; j < fTackCollectionsPerEvent; j++) {
        TH1* h = (TH1*) fHistograms->At(i, j)->Clone(Form("HIST_%i", j));
        h->SetName(Form("[%i][%i]", i, j));
        list->AddLast(h);
      }
      report->AddObject(list);
    }
    return report;
  }

  FluctuationsAna::~FluctuationsAna() {
    if (fHistograms) delete fHistograms;
  }
}  // namespace Hal
