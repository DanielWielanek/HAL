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
#include "CutMonitorComplex.h"
#include "DataFormat.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"
#include "StdString.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TString.h>

#include <memory>


namespace Hal {

  void PropertyMonitorXY::CreateHistograms() {
    TString title = Form("%s vs %s", Hal::Std::RemoveUnits(fYaxisName).Data(), Hal::Std::RemoveUnits(fXaxisName).Data());
    TString name;  // = Form("%s_vs_%s",
                   // fCut[0]->GetUnit(fOptionAxis[0]).Data(),fCut[1]->GetUnit(fOptionAxis[1]).Data());
    name         = "Passed";
    fHistoPassed = new FastHist2D(name, title, fAxisBins[0], fAxisMin[0], fAxisMax[0], fAxisBins[1], fAxisMin[1], fAxisMax[1]);
    fHistoPassed->SetXaxisName(fXaxisName);
    fHistoPassed->SetYaxisName(fYaxisName);
    name         = name + "_F";
    name         = "Failed";
    fHistoFailed = new FastHist2D(name, title, fAxisBins[0], fAxisMin[0], fAxisMax[0], fAxisBins[1], fAxisMin[1], fAxisMax[1]);
    fHistoFailed->SetXaxisName(fXaxisName);
    fHistoFailed->SetYaxisName(fYaxisName);
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
    if (IsInitialized()) {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("%s is initialized ", this->ClassName()), EInfo::kDebugInfo);
#endif
      return kFALSE;
    }
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    fFormatType     = ev->GetFormatType();
    CreateHistograms();
    MarkAsInitialized();
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
      fXaxisName  = other.fXaxisName;
      fYaxisName  = other.fYaxisName;
      fFormatType = other.fFormatType;
    }
    return *this;
  }

  //========================================================================

  EventFieldMonitorXY::EventFieldMonitorXY(Int_t fieldIDX, Int_t fiedIDY) :
    PropertyMonitorXY("", "", ECutUpdate::kEvent), fFieldIDX(fieldIDX), fFieldIDY(fiedIDY) {
    fTranslator.SetFieldId(fFieldIDX, 0);
    fTranslator.SetFieldId(fFieldIDY, 1);
  }

  EventFieldMonitorXY::EventFieldMonitorXY(Int_t fieldIDX,
                                           std::initializer_list<Double_t> xAxis,
                                           Int_t fiedIDY,
                                           std::initializer_list<Double_t> yAxis) :
    EventFieldMonitorXY(fieldIDX, fiedIDY) {
    SetAxisList(xAxis, 'x');
    SetAxisList(yAxis, 'y');
  }

  void EventFieldMonitorXY::Update(Bool_t passed, TObject* obj) {
    Event* ev = (Event*) obj;
    ManualFill2D(ev->GetFieldVal(fFieldIDX), ev->GetFieldVal(fFieldIDY), passed);
  }

  Bool_t EventFieldMonitorXY::Init(Int_t task_id) {
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    fXaxisName      = ev->GetFieldName(fTranslator.GetFieldFull(0));
    fYaxisName      = ev->GetFieldName(fTranslator.GetFieldFull(1));
    if (fXaxisName == "[]") {
      Hal::Cout::PrintInfo(Form("Cannot find event field for field X:ID = %i [%i]", fFieldIDX, fTranslator.GetFieldFull(0)),
                           EInfo::kError);
      return kFALSE;
    }
    if (fYaxisName == "[]") {
      Hal::Cout::PrintInfo(Form("Cannot find event field for field Y:ID = %i [%i]", fFieldIDY, fTranslator.GetFieldFull(1)),
                           EInfo::kError);
      return kFALSE;
    }
    if (fXaxisName == "[]" || fYaxisName == "[]") return kFALSE;
    return PropertyMonitorXY::Init(task_id);
  }
  //========================================================================
  TrackFieldMonitorXY::TrackFieldMonitorXY(Int_t fieldIDX, Int_t fiedIDY) :
    PropertyMonitorXY(",", "", ECutUpdate::kTrack), fFieldIDX(fieldIDX), fFieldIDY(fiedIDY) {
    fTranslator.SetFieldId(fFieldIDX, 0);
    fTranslator.SetFieldId(fFieldIDY, 1);
  }

  TrackFieldMonitorXY::TrackFieldMonitorXY(Int_t fieldIDX,
                                           std::initializer_list<Double_t> xAxis,
                                           Int_t fieldIDY,
                                           std::initializer_list<Double_t> yAxis) :
    TrackFieldMonitorXY(fieldIDX, fieldIDY) {
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
    auto tr         = std::unique_ptr<Track>(ev->GetNewTrack());
    auto event      = std::unique_ptr<Event>(ev->GetNewEvent());
    tr->SetEvent(event.get());
    fXaxisName = tr->GetFieldName(fTranslator.GetFieldFull(0));
    fYaxisName = tr->GetFieldName(fTranslator.GetFieldFull(1));
    if (fXaxisName == "[]") {
      Hal::Cout::PrintInfo(Form("Cannot find track field for field X:ID = %i [%i]", fFieldIDX, fTranslator.GetFieldFull(0)),
                           EInfo::kError);
      return kFALSE;
    }
    if (fYaxisName == "[]") {
      Hal::Cout::PrintInfo(Form("Cannot find track field for field Y:ID = %i [%i]", fFieldIDY, fTranslator.GetFieldFull(1)),
                           EInfo::kError);
      return kFALSE;
    }
    return PropertyMonitorXY::Init(task_id);
  }

  Bool_t EventFieldMonitorXY::AreSimilar(const CutMonitor& other) const {
    auto monitor = dynamic_cast<const EventFieldMonitorXY*>(&other);
    if (!monitor) return kFALSE;
    if (fTranslator.GetFieldFull(0) != monitor->fTranslator.GetFieldFull(0)) return kFALSE;
    if (fTranslator.GetFieldFull(1) != monitor->fTranslator.GetFieldFull(1)) return kFALSE;
    return CutMonitorXY::AreSimilar(other);
  }

  Bool_t TrackFieldMonitorXY::AreSimilar(const CutMonitor& other) const {
    auto monitor = dynamic_cast<const TrackFieldMonitorXY*>(&other);
    if (!monitor) return kFALSE;
    if (fTranslator.GetFieldFull(0) != monitor->fTranslator.GetFieldFull(0)) return kFALSE;
    if (fTranslator.GetFieldFull(1) != monitor->fTranslator.GetFieldFull(1)) return kFALSE;
    return CutMonitorXY::AreSimilar(other);
  }

  Bool_t PropertyMonitorXY::AreSimilar(const CutMonitor& other) const {
    auto conv = dynamic_cast<const PropertyMonitorXY*>(&other);
    if (!conv) return kFALSE;
    if (fXaxisName != conv->fXaxisName) return kFALSE;
    if (fYaxisName != conv->fYaxisName) return kFALSE;
    return CutMonitorXY::AreSimilar(other);
  }

  void EventFieldMonitorXY::MakeComplexAxes(TString opt) {
    if (Hal::Std::FindParam(opt, "re")) fTranslator.SwitchToRe();
    if (Hal::Std::FindParam(opt, "im")) fTranslator.SwitchToIm();
  }

  void TrackFieldMonitorXY::MakeComplexAxes(TString opt) {
    if (Hal::Std::FindParam(opt, "re")) fTranslator.SwitchToRe();
    if (Hal::Std::FindParam(opt, "im")) fTranslator.SwitchToIm();
  }

}  // namespace Hal
