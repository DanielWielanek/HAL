/*
 * QATrackTask.cxx
 *
 *  Created on: 14 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "QATrackTask.h"
#include "QAPlot.h"

#include "Cout.h"
#include "CutCollection.h"
#include "CutContainer.h"
#include "Event.h"
#include "Package.h"
#include "Parameter.h"
#include "Track.h"

#include <iostream>

namespace Hal {
  QATrackTask::QATrackTask() :
    TrackAna(), fEventQA(nullptr), fTempEventPlot(nullptr), fTrackQA(nullptr), fTempTrackPlot(nullptr) {
    AddTags("qa");
  }

  QATrackTask::~QATrackTask() {
    if (fEventQA) delete fEventQA;
    if (fTempEventPlot) delete fTempEventPlot;
    if (fTrackQA) delete fTrackQA;
    if (fTempTrackPlot) delete fTempTrackPlot;
  }

  QATrackTask::QATrackTask(const QATrackTask& other) :
    TrackAna(other), fEventQA(nullptr), fTempEventPlot(nullptr), fTrackQA(nullptr), fTempTrackPlot(nullptr) {
    if (other.fEventQA) {
      fEventQA = new TObjArray();
      for (int i = 0; i < other.fEventQA->GetEntriesFast(); i++) {
        QAPlot* plot = (QAPlot*) other.fEventQA->UncheckedAt(i);
        fEventQA->Add(plot->MakeCopy());
      }
    }
    if (other.fTempEventPlot) { fTempEventPlot = other.fTempEventPlot->MakeCopy(); }

    if (other.fTrackQA) {
      fTrackQA = new TObjArray();
      for (int i = 0; i < other.fTrackQA->GetEntriesFast(); i++) {
        QAPlot* plot = (QAPlot*) other.fTrackQA->UncheckedAt(i);
        fTrackQA->Add(plot->MakeCopy());
      }
    }
    fEventColNames = other.fEventColNames;
    fTrackColNames = other.fTrackColNames;
    if (other.fTempTrackPlot) { fTempTrackPlot = other.fTempTrackPlot->MakeCopy(); }
  }

  void QATrackTask::SetQAPlot(const QAPlot& plot) {
    switch (plot.GetUpdateRatio()) {
      case ECutUpdate::kEvent: {
        fTempEventPlot = plot.MakeCopy();
      } break;
      case ECutUpdate::kTrack: {
        fTempTrackPlot = plot.MakeCopy();
      } break;
      default: {
        Cout::PrintInfo("Unkown QAPlot::GetUpdateRatio()", EInfo::kLowWarning);
      } break;
    }
  }

  void QATrackTask::ProcessEvent() {
    TrackAna::ProcessEvent();
    GetEventQAPlot(fCurrentEventCollectionID)->Fill(fCurrentEvent);
  }

  void QATrackTask::ProcessTrack() {
    Int_t id = fCurrentEventCollectionID * fTrackCollectionsNo + fCurrentTrackCollectionID;
    GetTrackQAPlot(id)->Fill(fCurrentTrack);
  }

  Task::EInitFlag QATrackTask::Init() {
    Task::EInitFlag stat = TrackAna::Init();
    if (stat == Task::EInitFlag::kFATAL) return stat;
    if (fTempEventPlot == nullptr) { fTempEventPlot = new QAPlot("Null", ECutUpdate::kEvent); }
    if (fTempTrackPlot == nullptr) {
      Cout::PrintInfo("Lack of Track QA", EInfo::kError);
      return Task::EInitFlag::kFATAL;
    }
    fEventQA = new TObjArray();
    for (int i = 0; i < fEventCollectionsNo; i++) {
      QAPlot* plot = fTempEventPlot->MakeCopy();
      plot->Init(GetTaskID());
      fEventQA->Add(plot);
    }
    delete fTempEventPlot;
    fTempEventPlot = nullptr;

    fTrackQA = new TObjArray();
    for (int i = 0; i < fTrackCollectionsNo * fEventCollectionsNo; i++) {
      QAPlot* plot = fTempTrackPlot->MakeCopy();
      plot->Init(GetTaskID());
      fTrackQA->Add(plot);
    }
    delete fTempTrackPlot;
    fTempTrackPlot = nullptr;
    return Task::EInitFlag::kSUCCESS;
  }

  QATrackTask& QATrackTask::operator=(const QATrackTask& other) {
    if (this == &other) return *this;
    if (other.fTempEventPlot) {
      if (fTempEventPlot) delete fTempEventPlot;
      fTempEventPlot = other.fTempEventPlot->MakeCopy();
    }
    if (other.fTempTrackPlot) {
      if (fTempTrackPlot) delete fTempTrackPlot;
      fTempTrackPlot = other.fTempTrackPlot->MakeCopy();
    }
    fEventColNames = other.fEventColNames;
    fTrackColNames = other.fTrackColNames;
    return *this;
  }

  void QATrackTask::LinkCollections() {
    fCutContainer->LinkCollections(ECutUpdate::kEvent, ECutUpdate::kTrack, CutContainer::ELinkPolicy::kAnyToAny);
  }

  Package* QATrackTask::Report() const {
    Package* pack        = TrackAna::Report();
    const Int_t eventCol = fEventQA->GetEntries();
    const Int_t trackCol = fTrackQA->GetEntries() / fEventQA->GetEntries();

    for (int iEventCol = 0; iEventCol < eventCol; iEventCol++) {
      QAPlot* qa_event           = (QAPlot*) fEventQA->At(iEventCol);
      QAPlotReport* event_report = qa_event->GetReport();
      event_report->Recalculate();
      TString eventColName = Form("Event coll. %i", iEventCol);
      if ((int) fEventColNames.size() > iEventCol) { eventColName = fEventColNames[iEventCol]; }
      event_report->SetName(eventColName);
      pack->AddObject(event_report);
      for (int jTrackCol = 0; jTrackCol < trackCol; jTrackCol++) {
        QAPlot* qa_track           = (QAPlot*) fTrackQA->At(iEventCol * trackCol + jTrackCol);
        QAPlotReport* track_report = qa_track->GetReport();
        track_report->Recalculate();
        if (jTrackCol < (int) fTrackColNames.size()) {
          TString name = Form("%s %s", eventColName.Data(), fTrackColNames[jTrackCol].Data());
          track_report->SetName(name);
        } else {
          track_report->SetName(Form("Event coll. %i Track coll. %i", iEventCol, jTrackCol));
        }
        pack->AddObject(track_report);
      }
    }
    return pack;
  }

  void QATrackTask::SetEventCollectionNames(const std::initializer_list<TString>& init) {
    fEventColNames.clear();
    for (auto const name : init) {
      fEventColNames.push_back(name);
    }
  }

  void QATrackTask::SetTrackCollectionNames(const std::initializer_list<TString>& init) {
    fTrackColNames.clear();
    for (auto const name : init) {
      fTrackColNames.push_back(name);
    }
  }
}  // namespace Hal
