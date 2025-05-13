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
#include "DataFormatManager.h"
#include "Event.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"
#include "StdString.h"

#include <TAxis.h>
#include <TH1.h>
#include <TH3.h>
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
    fHistoPassed = new TH3D(name,
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
    fHistoPassed->GetXaxis()->SetTitle(fXaxisName);
    fHistoPassed->GetYaxis()->SetTitle(fYaxisName);
    fHistoPassed->GetZaxis()->SetTitle(fZaxisName);
    name         = "Failed";
    fHistoFailed = (TH3D*) fHistoPassed->Clone(name);
    fInit        = kTRUE;
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

  PropertyMonitorXYZ::~PropertyMonitorXYZ() {
    // TODO Auto-generated destructor stub
  }
  //========================================================================
  EventFieldMonitorXYZ::EventFieldMonitorXYZ(Int_t fieldIDX, Int_t fiedIDY, Int_t fiedIDZ) :
    PropertyMonitorXYZ("", "", "", ECutUpdate::kEvent), fFieldIDX(fieldIDX), fFieldIDY(fiedIDY), fFieldIDZ(fiedIDZ) {}

  EventFieldMonitorXYZ::EventFieldMonitorXYZ(Int_t fieldIDX,
                                             std::initializer_list<Double_t> xAxis,
                                             Int_t fieldDY,
                                             std::initializer_list<Double_t> yAxis,
                                             Int_t fieldDZ,
                                             std::initializer_list<Double_t> zAxis) :
    PropertyMonitorXYZ("", "", "", ECutUpdate::kEvent), fFieldIDX(fieldIDX), fFieldIDY(fieldDY), fFieldIDZ(fieldDZ) {
    SetAxisList(xAxis, 'x');
    SetAxisList(yAxis, 'y');
    SetAxisList(zAxis, 'z');
  }

  void EventFieldMonitorXYZ::Update(Bool_t passed, TObject* obj) {

    Event* ev = (Event*) obj;
    if (passed) {
      ((TH3*) fHistoPassed)->Fill(ev->GetFieldVal(fFieldIDX), ev->GetFieldVal(fFieldIDY), ev->GetFieldVal(fFieldIDZ));
    } else {
      ((TH3*) fHistoFailed)->Fill(ev->GetFieldVal(fFieldIDX), ev->GetFieldVal(fFieldIDY), ev->GetFieldVal(fFieldIDZ));
    }
  }

  Bool_t EventFieldMonitorXYZ::Init(Int_t task_id) {
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);

    fXaxisName = ev->GetFieldName(fFieldIDX);
    fYaxisName = ev->GetFieldName(fFieldIDY);
    fZaxisName = ev->GetFieldName(fFieldIDZ);
    return PropertyMonitorXYZ::Init(task_id);
  }
  //========================================================================
  TrackFieldMonitorXYZ::TrackFieldMonitorXYZ(Int_t fieldIDX, Int_t fiedIDY, Int_t fiedIDZ) :
    PropertyMonitorXYZ("", "", "", ECutUpdate::kTrack), fFieldIDX(fieldIDX), fFieldIDY(fiedIDY), fFieldIDZ(fiedIDZ) {}

  TrackFieldMonitorXYZ::TrackFieldMonitorXYZ(Int_t fieldIDX,
                                             std::initializer_list<Double_t> xAxis,
                                             Int_t fieldDY,
                                             std::initializer_list<Double_t> yAxis,
                                             Int_t fieldDZ,
                                             std::initializer_list<Double_t> zAxis) :
    PropertyMonitorXYZ("", "", "", ECutUpdate::kTrack), fFieldIDX(fieldIDX), fFieldIDY(fieldDY), fFieldIDZ(fieldDZ) {
    SetAxisList(xAxis, 'x');
    SetAxisList(yAxis, 'y');
    SetAxisList(zAxis, 'z');
  }

  void TrackFieldMonitorXYZ::Update(Bool_t passed, TObject* obj) {

    Track* tr = (Track*) obj;
    if (passed) {
      ((TH3*) fHistoPassed)->Fill(tr->GetFieldVal(fFieldIDX), tr->GetFieldVal(fFieldIDY), tr->GetFieldVal(fFieldIDZ));
    } else {
      ((TH3*) fHistoFailed)->Fill(tr->GetFieldVal(fFieldIDX), tr->GetFieldVal(fFieldIDY), tr->GetFieldVal(fFieldIDZ));
    }
  }

  Bool_t TrackFieldMonitorXYZ::Init(Int_t task_id) {
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (ev->InheritsFrom("Hal::ComplexEvent")) {
      ComplexTrack* tr  = (ComplexTrack*) ev->GetNewTrack();
      ComplexEvent* tev = (ComplexEvent*) ev->GetNewEvent();
      tr->SetEvent(tev);
      fXaxisName = tr->GetFieldName(fFieldIDX);
      fYaxisName = tr->GetFieldName(fFieldIDY);
      fZaxisName = tr->GetFieldName(fFieldIDZ);
      delete tr;
      delete tev;
    } else {
      Track* tr  = ev->GetNewTrack();
      fXaxisName = tr->GetFieldName(fFieldIDX);
      fYaxisName = tr->GetFieldName(fFieldIDY);
      fZaxisName = tr->GetFieldName(fFieldIDZ);
      delete tr;
    }
    return PropertyMonitorXYZ::Init(task_id);
  }

  Bool_t EventFieldMonitorXYZ::AreSimilar(CutMonitor* other) const {
    auto monitor = dynamic_cast<EventFieldMonitorXYZ*>(other);
    if (!monitor) return kFALSE;
    if (fFieldIDX != monitor->fFieldIDX) return kFALSE;
    if (fFieldIDY != monitor->fFieldIDY) return kFALSE;
    if (fFieldIDZ != monitor->fFieldIDZ) return kFALSE;
    return CutMonitorXYZ::AreSimilar(other);
  }

  Bool_t TrackFieldMonitorXYZ::AreSimilar(CutMonitor* other) const {
    auto monitor = dynamic_cast<TrackFieldMonitorXYZ*>(other);
    if (!monitor) return kFALSE;
    if (fFieldIDX != monitor->fFieldIDX) return kFALSE;
    if (fFieldIDY != monitor->fFieldIDY) return kFALSE;
    if (fFieldIDZ != monitor->fFieldIDZ) return kFALSE;
    return CutMonitorXYZ::AreSimilar(other);
  }

}  // namespace Hal
