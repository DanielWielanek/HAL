/*
 * CutMonitorObjectXY.cxx
 *
 *  Created on: 21 sie 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "PropertyMonitorXY.h"

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "Cout.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"
#include "StdString.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TH1.h>
#include <TH2.h>
#include <TString.h>


namespace Hal {

  void PropertyMonitorXY::CreateHistograms() {
    TH1::AddDirectory(kFALSE);
    TString title = Form("%s vs %s", Hal::Std::RemoveUnits(fYaxisName).Data(), Hal::Std::RemoveUnits(fXaxisName).Data());
    TString name;  // = Form("%s_vs_%s",
                   // fCut[0]->GetUnit(fOptionAxis[0]).Data(),fCut[1]->GetUnit(fOptionAxis[1]).Data());
    name         = "Passed";
    fHistoPassed = new TH2D(name, title, fAxisBins[0], fAxisMin[0], fAxisMax[0], fAxisBins[1], fAxisMin[1], fAxisMax[1]);
    fHistoPassed->GetXaxis()->SetTitle(fXaxisName);
    fHistoPassed->GetYaxis()->SetTitle(fYaxisName);
    name         = name + "_F";
    name         = "Failed";
    fHistoFailed = new TH2D(name, title, fAxisBins[0], fAxisMin[0], fAxisMax[0], fAxisBins[1], fAxisMin[1], fAxisMax[1]);
    fHistoFailed->GetXaxis()->SetTitle(fXaxisName);
    fHistoFailed->GetYaxis()->SetTitle(fYaxisName);
    TH1::AddDirectory(kTRUE);
  }

  PropertyMonitorXY::PropertyMonitorXY(TString xLabel, TString yLabel, ECutUpdate update) :
    CutMonitorXY(), fXaxisName(xLabel), fYaxisName(yLabel), fFormatType(EFormatType::kReco) {
    fUpdateRatio = update;
  }

  PropertyMonitorXY::PropertyMonitorXY(const PropertyMonitorXY& other) : CutMonitorXY(other) {
    fXaxisName  = other.fXaxisName;
    fYaxisName  = other.fYaxisName;
    fFormatType = other.fFormatType;
  }

  Bool_t PropertyMonitorXY::Init(Int_t task_id) {
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

  Package* PropertyMonitorXY::Report() const {
    Package* report = CutMonitorXY::Report();
    report->AddObject(new ParameterString("AxisX", fXaxisName));
    report->AddObject(new ParameterString("CutXName", "-"));
    report->AddObject(new ParameterDouble("CutXMin", 0));
    report->AddObject(new ParameterDouble("CutXMax", 0));
    report->AddObject(new ParameterInt("CutXAxis", 0));
    report->AddObject(new ParameterInt("CutXCollection", fCollectionID));
    report->AddObject(new ParameterString("AxisY", fYaxisName));
    report->AddObject(new ParameterString("CutYName", "-"));
    report->AddObject(new ParameterDouble("CutYMin", 0));
    report->AddObject(new ParameterDouble("CutYMax", 0));
    report->AddObject(new ParameterInt("CutYAxis", 0));
    report->AddObject(new ParameterInt("CutYCollection", fCollectionID));
    return report;
  }

  PropertyMonitorXY& PropertyMonitorXY::operator=(const PropertyMonitorXY& other) {
    if (this != &other) {
      CutMonitorXY::operator=(other);
      fXaxisName            = other.fXaxisName;
      fYaxisName            = other.fYaxisName;
      fFormatType           = other.fFormatType;
    }
    return *this;
  }

  PropertyMonitorXY::~PropertyMonitorXY() {
    // TODO Auto-generated destructor stub
  }
  //========================================================================
  EventFieldMonitorXY::EventFieldMonitorXY(Int_t fieldIDX, Int_t fiedIDY) :
    PropertyMonitorXY("", "", ECutUpdate::kEvent), fFieldIDX(fieldIDX), fFieldIDY(fiedIDY) {}

  EventFieldMonitorXY::EventFieldMonitorXY(Int_t fieldIDX,
                                           std::initializer_list<Double_t> xAxis,
                                           Int_t fiedIDY,
                                           std::initializer_list<Double_t> yAxis) :
    PropertyMonitorXY("", "", ECutUpdate::kEvent), fFieldIDX(fieldIDX), fFieldIDY(fiedIDY) {
    SetAxisList(xAxis, 'x');
    SetAxisList(yAxis, 'y');
  }

  void EventFieldMonitorXY::Update(Bool_t passed, TObject* obj) {
    Event* ev = (Event*) obj;
    if (passed) {
      fHistoPassed->Fill(ev->GetFieldVal(fFieldIDX), ev->GetFieldVal(fFieldIDY));
    } else {
      fHistoFailed->Fill(ev->GetFieldVal(fFieldIDX), ev->GetFieldVal(fFieldIDY));
    }
  }

  Bool_t EventFieldMonitorXY::Init(Int_t task_id) {
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    fXaxisName      = ev->GetFieldName(fFieldIDX);
    fYaxisName      = ev->GetFieldName(fFieldIDY);
    return PropertyMonitorXY::Init(task_id);
  }
  //========================================================================
  TrackFieldMonitorXY::TrackFieldMonitorXY(Int_t fieldIDX, Int_t fiedIDY) :
    PropertyMonitorXY(",", "", ECutUpdate::kTrack), fFieldIDX(fieldIDX), fFieldIDY(fiedIDY) {}

  TrackFieldMonitorXY::TrackFieldMonitorXY(Int_t fieldIDX,
                                           std::initializer_list<Double_t> xAxis,
                                           Int_t fiedIDY,
                                           std::initializer_list<Double_t> yAxis) :
    PropertyMonitorXY("", "", ECutUpdate::kTrack), fFieldIDX(fieldIDX), fFieldIDY(fiedIDY) {
    SetAxisList(xAxis, 'x');
    SetAxisList(yAxis, 'y');
  }

  void TrackFieldMonitorXY::Update(Bool_t passed, TObject* obj) {
    Track* tr = (Track*) obj;
    if (passed) {
      fHistoPassed->Fill(tr->GetFieldVal(fFieldIDX), tr->GetFieldVal(fFieldIDY));
    } else {
      fHistoFailed->Fill(tr->GetFieldVal(fFieldIDX), tr->GetFieldVal(fFieldIDY));
    }
  }

  Bool_t TrackFieldMonitorXY::Init(Int_t task_id) {
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (ev->InheritsFrom("Hal::ComplexEvent")) {
      ComplexTrack* tr  = (ComplexTrack*) ev->GetNewTrack();
      ComplexEvent* tev = (ComplexEvent*) ev->GetNewEvent();
      tr->SetEvent(tev);
      fXaxisName = tr->GetFieldName(fFieldIDX);
      fYaxisName = tr->GetFieldName(fFieldIDY);
      delete tr;
      delete tev;
    } else {
      Track* tr  = ev->GetNewTrack();
      fXaxisName = tr->GetFieldName(fFieldIDX);
      fYaxisName = tr->GetFieldName(fFieldIDY);
      delete tr;
    }
    return PropertyMonitorXY::Init(task_id);
  }
}  // namespace Hal
