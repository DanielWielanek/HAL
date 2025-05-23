/*
 * CutMonitorObjectX.cxx
 *
 *  Created on: 21 sie 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "PropertyMonitorX.h"

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "Cout.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"
#include "StdString.h"

#include <TAttFill.h>
#include <TAxis.h>
#include <TH1.h>
#include <TString.h>
#include <iostream>


namespace Hal {
  void PropertyMonitorX::CreateHistograms() {
    TH1::AddDirectory(kFALSE);
    TString title = Form("%s", Hal::Std::RemoveUnits(fXaxisName).Data());
    TString name  = "Passed";
    fHistoPassed  = new TH1D(name, title, fAxisBins[0], fAxisMin[0], fAxisMax[0]);
    fHistoPassed->GetXaxis()->SetTitle(fXaxisName);
    fHistoPassed->GetYaxis()->SetTitle(fYaxisName);
    fHistoPassed->SetFillColor(kGreen);
    name         = "Failed";
    fHistoFailed = (TH1D*) fHistoPassed->Clone(name);
    fHistoFailed->SetFillColor(kRed);
    TH1::AddDirectory(kTRUE);
  }

  PropertyMonitorX::PropertyMonitorX(TString xLabel, TString yLabel, ECutUpdate update) :
    CutMonitorX(), fXaxisName(xLabel), fYaxisName(yLabel), fFormatType(EFormatType::kReco) {
    fUpdateRatio = update;
  }

  PropertyMonitorX::PropertyMonitorX(const PropertyMonitorX& other) : CutMonitorX(other) {
    fXaxisName  = other.fXaxisName;
    fYaxisName  = other.fYaxisName;
    fFormatType = other.fFormatType;
  }

  Bool_t PropertyMonitorX::Init(Int_t task_id) {
    if (fInit) {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("%s is initialized ", this->ClassName()), EInfo::kDebugInfo);
#endif
      return kFALSE;
    }
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    fFormatType     = ev->GetFormatType();
    TH1::AddDirectory(kFALSE);
    CreateHistograms();
    TH1::AddDirectory(kTRUE);
    fInit = kTRUE;
    return kTRUE;
  }

  Package* PropertyMonitorX::Report() const {
    Package* report = CutMonitorX::Report();
    report->AddObject(new ParameterString("AxisX", fXaxisName));
    report->AddObject(new ParameterString("CutXName", "-"));
    report->AddObject(new ParameterDouble("CutXMin", 0));
    report->AddObject(new ParameterDouble("CutXMax", 0));
    report->AddObject(new ParameterInt("CutXAxis", 0));
    report->AddObject(new ParameterInt("CutXCollection", fCollectionID));
    return report;
  }

  PropertyMonitorX& PropertyMonitorX::operator=(const PropertyMonitorX& other) {
    if (this != &other) {
      CutMonitorX::operator=(other);
      fXaxisName  = other.fXaxisName;
      fYaxisName  = other.fYaxisName;
      fFormatType = other.fFormatType;
    }
    return *this;
  }

  PropertyMonitorX::~PropertyMonitorX() {}

  //========================================================================
  EventFieldMonitorX::EventFieldMonitorX(Int_t fieldID) :
    PropertyMonitorX("", "N_{events}", ECutUpdate::kEvent), fFieldID(fieldID) {}

  EventFieldMonitorX::EventFieldMonitorX(Int_t fieldID, std::initializer_list<Double_t> xAxis) : EventFieldMonitorX(fieldID) {
    SetAxisList(xAxis, 'x');
  }

  void EventFieldMonitorX::Update(Bool_t passed, TObject* obj) {
    Event* ev = (Event*) obj;
    if (passed) {
      fHistoPassed->Fill(ev->GetFieldVal(fFieldID));
    } else {
      fHistoFailed->Fill(ev->GetFieldVal(fFieldID));
    }
  }

  Bool_t EventFieldMonitorX::Init(Int_t task_id) {
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    fXaxisName      = ev->GetFieldName(fFieldID);
    return PropertyMonitorX::Init(task_id);
  }
  //========================================================================
  TrackFieldMonitorX::TrackFieldMonitorX(Int_t fieldID) :
    PropertyMonitorX("", "N_{tracks}", ECutUpdate::kTrack), fFieldID(fieldID) {}

  TrackFieldMonitorX::TrackFieldMonitorX(Int_t fieldID, std::initializer_list<Double_t> xAxis) : TrackFieldMonitorX(fieldID) {
    SetAxisList(xAxis, 'x');
  }

  void TrackFieldMonitorX::Update(Bool_t passed, TObject* obj) {
    Track* tr = (Track*) obj;
    if (passed) {
      fHistoPassed->Fill(tr->GetFieldVal(fFieldID));
    } else {
      fHistoFailed->Fill(tr->GetFieldVal(fFieldID));
    }
  }

  Bool_t TrackFieldMonitorX::Init(Int_t task_id) {
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (ev->InheritsFrom("Hal::ComplexEvent")) {
      ComplexTrack* tr  = (ComplexTrack*) ev->GetNewTrack();
      ComplexEvent* tev = (ComplexEvent*) ev->GetNewEvent();
      tr->SetEvent(tev);
      fXaxisName = tr->GetFieldName(fFieldID);
      delete tr;
      delete tev;
    } else {
      Track* tr  = ev->GetNewTrack();
      fXaxisName = tr->GetFieldName(fFieldID);
      delete tr;
    }
    return PropertyMonitorX::Init(task_id);
  }

  Bool_t EventFieldMonitorX::AreSimilar(const CutMonitor& other) const {
    auto monitor = dynamic_cast<const EventFieldMonitorX*>(&other);
    if (!monitor) return kFALSE;
    if (fFieldID != monitor->fFieldID) return kFALSE;
    return CutMonitorX::AreSimilar(other);
  }

  Bool_t TrackFieldMonitorX::AreSimilar(const CutMonitor& other) const {
    auto monitor = dynamic_cast<const TrackFieldMonitorX*>(&other);
    if (!monitor) return kFALSE;
    if (fFieldID != monitor->fFieldID) return kFALSE;
    return CutMonitorX::AreSimilar(other);
  }

  Bool_t PropertyMonitorX::AreSimilar(const CutMonitor& other) const {
    auto conv = dynamic_cast<const PropertyMonitorX*>(&other);
    if (!conv) return kFALSE;
    if (fXaxisName != conv->fXaxisName) return kFALSE;
    if (fYaxisName != conv->fYaxisName) return kFALSE;
    return CutMonitorX::AreSimilar(other);
  }

}  // namespace Hal
