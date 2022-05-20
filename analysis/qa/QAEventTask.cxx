/*
 * QAEventTask.cxx
 *
 *  Created on: 14 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "QAEventTask.h"


#include "Cout.h"
#include "Event.h"
#include "Std.h"
#include "Package.h"
#include "QAPlotReport.h"
#include "Track.h"

namespace Hal {
  QAEventTask::QAEventTask() : fEventQA(nullptr), fTempEventPlot(nullptr) { AddTags("qa"); }

  QAEventTask::~QAEventTask() {
    if (fEventQA) delete fEventQA;
    if (fTempEventPlot) delete fTempEventPlot;
  }

  QAEventTask::QAEventTask(const QAEventTask& other) : EventAna(other), fEventQA(nullptr), fTempEventPlot(nullptr) {
    if (other.fEventQA) {
      fEventQA = new TObjArray();
      for (int i = 0; i < other.fEventQA->GetEntriesFast(); i++) {
        QAPlot* plot = (QAPlot*) other.fEventQA->UncheckedAt(i);
        fEventQA->Add(plot->MakeCopy());
      }
    }
    if (other.fTempEventPlot) { fTempEventPlot = other.fTempEventPlot->MakeCopy(); }
    fEventColNames = other.fEventColNames;
  }

  Task::EInitFlag QAEventTask::Init() {
    Task::EInitFlag stat = EventAna::Init();
    if (stat == Task::EInitFlag::kFATAL) return stat;
    if (fTempEventPlot == nullptr) {
      Cout::PrintInfo("Lack of Event QA", EInfo::kLessError);
      return Task::EInitFlag::kFATAL;
    } else {
      fEventQA = new TObjArray();
      for (int i = 0; i < fEventCollectionsNo; i++) {
        QAPlot* plot = fTempEventPlot->MakeCopy();
        plot->Init(GetTaskID());
        fEventQA->Add(plot);
      }
      delete fTempEventPlot;
      fTempEventPlot = nullptr;
    }
    return Task::EInitFlag::kSUCCESS;
  }

  Package* QAEventTask::Report() const {
    Package* report = EventAna::Report();
    for (int i = 0; i < fEventCollectionsNo; i++) {
      QAPlot* plot               = (QAPlot*) fEventQA->UncheckedAt(i);
      QAPlotReport* event_report = plot->GetReport();
      if (i < fEventColNames.size()) {
        event_report->SetName(fEventColNames[i]);
      } else {
        event_report->SetName(Form("Ev %i", i));
      }
      event_report->Recalculate();
      report->AddObject(event_report);
    }
    return report;
  }

  void QAEventTask::SetQAPlot(const QAPlot& plot) {
    if (plot.GetUpdateRatio() == ECutUpdate::kEvent) fTempEventPlot = plot.MakeCopy();
  }

  void QAEventTask::ProcessEvent() { GetQAPlot(fCurrentEventCollectionID)->Fill(fCurrentEvent); }

  QAEventTask& QAEventTask::operator=(const QAEventTask& other) {
    if (this == &other) return *this;
    if (other.fTempEventPlot) {
      if (fTempEventPlot) delete fTempEventPlot;
      fTempEventPlot = other.fTempEventPlot->MakeCopy();
    }
    fEventColNames = other.fEventColNames;
    return *this;
  }
}  // namespace Hal
