/*
 * PropertyMonitorXYZ.cxx
 *
 *  Created on: 21 sie 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "PropertyMonitorXYZ.h"

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

#include <TAxis.h>
#include <TString.h>


namespace Hal {
  void PropertyMonitorXYZ::CreateHistograms() {
    TString title = Form("%s vs %s vs %s",
                         Hal::Std::RemoveUnits(fZaxisName).Data(),
                         Hal::Std::RemoveUnits(fYaxisName).Data(),
                         Hal::Std::RemoveUnits(fXaxisName).Data());
    TString
      name;  // = Form("%s_vs_%s_vs_%s",
             // fCut[0]->GetUnit(fOptionAxis[0]).Data(),fCut[1]->GetUnit(fOptionAxis[1]).Data(),fCut[2]->GetUnit(fOptionAxis[2]).Data());
    name = "Passed";
    // title = title + Form(" ** %s %s %s
    // ",fCut[0]->ClassName(),fCut[1]->ClassName(),fCut[2]->ClassName());
    fHistoPassed = new FastHist3D(name,
                                  title,
                                  fAxisBins[0],
                                  fAxisMin[0],
                                  fAxisMax[0],
                                  fAxisBins[1],
                                  fAxisMin[1],
                                  fAxisMax[1],
                                  fAxisBins[2],
                                  fAxisMin[2],
                                  fAxisMax[2]);
    fHistoPassed->SetXaxisName(fXaxisName);
    fHistoPassed->SetYaxisName(fYaxisName);
    fHistoPassed->SetZaxisName(fZaxisName);
    name         = "Failed";
    fHistoFailed = (FastHist3D*) fHistoPassed->Clone(name);
    MarkAsInitialized();
  }

  PropertyMonitorXYZ::PropertyMonitorXYZ(TString xLabel, TString yLabel, TString zLabel, ECutUpdate update) :
    fXaxisName(xLabel), fYaxisName(yLabel), fZaxisName(zLabel), fFormatType(EFormatType::kReco) {
    fUpdateRatio = update;
  }

  PropertyMonitorXYZ::PropertyMonitorXYZ(const PropertyMonitorXYZ& other) : CutMonitorXYZ(other) {
    fXaxisName  = other.fXaxisName;
    fYaxisName  = other.fYaxisName;
    fZaxisName  = other.fZaxisName;
    fFormatType = other.fFormatType;
  }

  Bool_t PropertyMonitorXYZ::Init(Int_t task_id) {
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

  Package* PropertyMonitorXYZ::Report() const {
    Package* report = CutMonitorXYZ::Report();
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
    report->AddObject(new ParameterString("AxisZ", fZaxisName));
    report->AddObject(new ParameterString("CutZName", "-"));
    report->AddObject(new ParameterDouble("CutZMin", 0));
    report->AddObject(new ParameterDouble("CutZMax", 0));
    report->AddObject(new ParameterInt("CutZAxis", 0));
    report->AddObject(new ParameterInt("CutZCollection", fCollectionID));
    return report;
  }

  PropertyMonitorXYZ& PropertyMonitorXYZ::operator=(const PropertyMonitorXYZ& other) {
    if (this != &other) {
      CutMonitorXYZ::operator=(other);
      fXaxisName  = other.fXaxisName;
      fYaxisName  = other.fYaxisName;
      fZaxisName  = other.fZaxisName;
      fFormatType = other.fFormatType;
    }
    return *this;
  }

  //========================================================================
  EventFieldMonitorXYZ::EventFieldMonitorXYZ(Int_t fieldIDX, Int_t fiedIDY, Int_t fiedIDZ) :
    PropertyMonitorXYZ("", "", "", ECutUpdate::kEvent), fFieldIDX(fieldIDX), fFieldIDY(fiedIDY), fFieldIDZ(fiedIDZ) {
    fTranslator.SetFieldId(fFieldIDX, 0);
    fTranslator.SetFieldId(fFieldIDY, 1);
    fTranslator.SetFieldId(fFieldIDZ, 2);
  }

  EventFieldMonitorXYZ::EventFieldMonitorXYZ(Int_t fieldIDX,
                                             std::initializer_list<Double_t> xAxis,
                                             Int_t fieldDY,
                                             std::initializer_list<Double_t> yAxis,
                                             Int_t fieldDZ,
                                             std::initializer_list<Double_t> zAxis) :
    EventFieldMonitorXYZ(fieldIDX, fieldDY, fieldDZ) {
    SetAxisList(xAxis, 'x');
    SetAxisList(yAxis, 'y');
    SetAxisList(zAxis, 'z');
  }

  void EventFieldMonitorXYZ::Update(Bool_t passed, TObject* obj) {
    Event* ev = (Event*) obj;
    ManualFill3D(ev->GetFieldVal(fFieldIDX), ev->GetFieldVal(fFieldIDY), ev->GetFieldVal(fFieldIDZ), passed);
  }

  Bool_t EventFieldMonitorXYZ::Init(Int_t task_id) {
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    fXaxisName      = ev->GetFieldName(fTranslator.GetFieldFull(0));
    fYaxisName      = ev->GetFieldName(fTranslator.GetFieldFull(1));
    fZaxisName      = ev->GetFieldName(fTranslator.GetFieldFull(2));
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
    if (fZaxisName == "[]") {
      Hal::Cout::PrintInfo(Form("Cannot find event field for field Z:ID = %i [%i]", fFieldIDZ, fTranslator.GetFieldFull(2)),
                           EInfo::kError);
      return kFALSE;
    }
    return PropertyMonitorXYZ::Init(task_id);
  }

  void EventFieldMonitorXYZ::MakeComplexAxes(TString opt) {
    if (Hal::Std::FindParam(opt, "re")) fTranslator.SwitchToRe();
    if (Hal::Std::FindParam(opt, "im")) fTranslator.SwitchToIm();
  }

  //========================================================================

  TrackFieldMonitorXYZ::TrackFieldMonitorXYZ(Int_t fieldIDX, Int_t fiedIDY, Int_t fiedIDZ) :
    PropertyMonitorXYZ("", "", "", ECutUpdate::kTrack), fFieldIDX(fieldIDX), fFieldIDY(fiedIDY), fFieldIDZ(fiedIDZ) {
    fTranslator.SetFieldId(fFieldIDX, 0);
    fTranslator.SetFieldId(fFieldIDY, 1);
    fTranslator.SetFieldId(fFieldIDZ, 2);
  }

  TrackFieldMonitorXYZ::TrackFieldMonitorXYZ(Int_t fieldIDX,
                                             std::initializer_list<Double_t> xAxis,
                                             Int_t fieldDY,
                                             std::initializer_list<Double_t> yAxis,
                                             Int_t fieldDZ,
                                             std::initializer_list<Double_t> zAxis) :
    TrackFieldMonitorXYZ(fieldIDX, fieldDY, fieldDZ) {
    SetAxisList(xAxis, 'x');
    SetAxisList(yAxis, 'y');
    SetAxisList(zAxis, 'z');
  }

  void TrackFieldMonitorXYZ::Update(Bool_t passed, TObject* obj) {
    Track* tr = (Track*) obj;
    ManualFill3D(tr->GetFieldVal(fFieldIDX), tr->GetFieldVal(fFieldIDY), tr->GetFieldVal(fFieldIDZ), passed);
  }

  Bool_t TrackFieldMonitorXYZ::Init(Int_t task_id) {

    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    auto tr         = std::unique_ptr<Track>(ev->GetNewTrack());
    auto event      = std::unique_ptr<Event>(ev->GetNewEvent());
    tr->SetEvent(event.get());
    fXaxisName = tr->GetFieldName(fTranslator.GetFieldFull(0));
    fYaxisName = tr->GetFieldName(fTranslator.GetFieldFull(1));
    fZaxisName = tr->GetFieldName(fTranslator.GetFieldFull(2));
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
    if (fZaxisName == "[]") {
      Hal::Cout::PrintInfo(Form("Cannot find track field for field Z:ID = %i [%i]", fFieldIDZ, fTranslator.GetFieldFull(2)),
                           EInfo::kError);
      return kFALSE;
    }
    return PropertyMonitorXYZ::Init(task_id);
  }

  Bool_t EventFieldMonitorXYZ::AreSimilar(const CutMonitor& other) const {
    auto monitor = dynamic_cast<const EventFieldMonitorXYZ*>(&other);
    if (!monitor) return kFALSE;
    if (fTranslator.GetFieldFull(0) != monitor->fTranslator.GetFieldFull(0)) return kFALSE;
    if (fTranslator.GetFieldFull(1) != monitor->fTranslator.GetFieldFull(1)) return kFALSE;
    if (fTranslator.GetFieldFull(2) != monitor->fTranslator.GetFieldFull(2)) return kFALSE;
    return CutMonitorXYZ::AreSimilar(other);
  }

  Bool_t TrackFieldMonitorXYZ::AreSimilar(const CutMonitor& other) const {
    auto monitor = dynamic_cast<const TrackFieldMonitorXYZ*>(&other);
    if (!monitor) return kFALSE;
    if (fTranslator.GetFieldFull(0) != monitor->fTranslator.GetFieldFull(0)) return kFALSE;
    if (fTranslator.GetFieldFull(1) != monitor->fTranslator.GetFieldFull(1)) return kFALSE;
    if (fTranslator.GetFieldFull(2) != monitor->fTranslator.GetFieldFull(2)) return kFALSE;
    return CutMonitorXYZ::AreSimilar(other);
  }

  Bool_t PropertyMonitorXYZ::AreSimilar(const CutMonitor& other) const {
    auto conv = dynamic_cast<const PropertyMonitorXYZ*>(&other);
    if (!conv) return kFALSE;
    if (fXaxisName != conv->fXaxisName) return kFALSE;
    if (fYaxisName != conv->fYaxisName) return kFALSE;
    if (fZaxisName != conv->fZaxisName) return kFALSE;
    return CutMonitorXYZ::AreSimilar(other);
  }

  void TrackFieldMonitorXYZ::MakeComplexAxes(TString opt) {
    if (Hal::Std::FindParam(opt, "re")) fTranslator.SwitchToRe();
    if (Hal::Std::FindParam(opt, "im")) fTranslator.SwitchToIm();
  }

}  // namespace Hal
