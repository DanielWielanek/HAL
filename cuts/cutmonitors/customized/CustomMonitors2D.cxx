/*
 * CutsomMonitors2D.cpp
 *
 *  Created on: 28 lip 2025
 *      Author: daniel
 */

#include "CustomMonitors2D.h"
#include "ComplexTrack.h"
#include "Cout.h"
#include "DataFormat.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "ExpTrack.h"

#include <memory>

namespace Hal {

  CustomTrackMonitors2D::CustomTrackMonitors2D() : PropertyMonitorXY("", "", ECutUpdate::kTrack) {}

  void CustomTrackMonitors2D::Update(Bool_t passed, TObject* obj) {
    auto track = (Hal::Track*) obj;
    Double_t a = track->GetFieldVal(fFieldId1);
    Double_t b = track->GetFieldVal(fFieldId2);
    ManualFill2D(a, b, passed);
  }

  Bool_t CustomTrackMonitors2D::Init(Int_t task_id) {
    auto event    = Hal::DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    auto track    = std::unique_ptr<Hal::Track>(event->GetNewTrack());
    auto newEvent = std::unique_ptr<Hal::Event>(event->GetNewEvent());
    track->SetEvent(newEvent.get());
    fFormatType = event->GetFormatType();
    Int_t step  = 0;
    if (IsRe()) step = Hal::DataFieldID::ReStep;
    if (IsIm()) step = Hal::DataFieldID::ImStep;
    if (fFormatType == EFormatType::kComplexReco) {
      auto name1 = track->GetFieldName(fFieldId1);
      auto name2 = track->GetFieldName(fFieldId2);
      if (name1 == "[]" || name2 == "[]") {
        Hal::Cout::PrintInfo(Form("Trying to switch to *real format* in %s", ClassName()), EInfo::kDebugInfo);
        step = Hal::DataFieldID::ReStep;
      }
    }
    fFieldId1 += step;
    fFieldId2 += step;
    fXaxisName = track->GetFieldName(fFieldId1);
    fYaxisName = track->GetFieldName(fFieldId2);
    auto res   = PropertyMonitorXY::Init(task_id);
    if (fXaxisName == "[]" || fYaxisName == "[]") return kFALSE;
    return res;
  }

  Bool_t CustomTrackMonitors2D::AreSimilar(const Hal::CutMonitor& other) const {
    auto conv = dynamic_cast<const CustomTrackMonitors2D*>(&other);
    if (!conv) return kFALSE;
    if (fXaxisName != conv->fXaxisName) return kFALSE;
    if (fYaxisName != conv->fYaxisName) return kFALSE;
    return kTRUE;
  }

  CustomEventMonitors2D::CustomEventMonitors2D() : PropertyMonitorXY("", "", ECutUpdate::kEvent) {}

  void CustomEventMonitors2D::Update(Bool_t passed, TObject* obj) {
    auto event = (Hal::Event*) obj;
    Double_t a = event->GetFieldVal(fFieldId1);
    Double_t b = event->GetFieldVal(fFieldId2);
    ManualFill2D(a, b, passed);
  }

  Bool_t CustomEventMonitors2D::Init(Int_t task_id) {
    auto event  = Hal::DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    fFormatType = event->GetFormatType();
    Int_t step  = 0;
    if (IsRe()) step = Hal::DataFieldID::ReStep;
    if (IsIm()) step = Hal::DataFieldID::ImStep;
    if (fFormatType == EFormatType::kComplexReco) {
      auto name1 = event->GetFieldName(fFieldId1);
      auto name2 = event->GetFieldName(fFieldId2);
      if (name1 == "[]" || name2 == "[]") {
        Hal::Cout::PrintInfo(Form("Trying to switch to *real format* in %s", ClassName()), EInfo::kDebugInfo);
        step = Hal::DataFieldID::ReStep;
      }
    }
    fFieldId1 += step;
    fFieldId2 += step;
    fXaxisName = event->GetFieldName(fFieldId1);
    fYaxisName = event->GetFieldName(fFieldId2);
    if (fXaxisName == "[]" || fYaxisName == "[]") return kFALSE;
    auto res = PropertyMonitorXY::Init(task_id);
    return res;
  }

  Bool_t CustomEventMonitors2D::AreSimilar(const Hal::CutMonitor& other) const {
    auto conv = dynamic_cast<const CustomEventMonitors2D*>(&other);
    if (!conv) return kFALSE;
    if (fXaxisName != conv->fXaxisName) return kFALSE;
    if (fYaxisName != conv->fYaxisName) return kFALSE;
    return kTRUE;
  }

} /* namespace Hal */
