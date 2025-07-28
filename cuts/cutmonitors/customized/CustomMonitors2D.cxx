/*
 * CutsomMonitors2D.cpp
 *
 *  Created on: 28 lip 2025
 *      Author: daniel
 */

#include "CustomMonitors2D.h"
#include "ComplexTrack.h"
#include "DataFormat.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "ExpTrack.h"

namespace Hal {

  CustomTrackMonitors2D::CustomTrackMonitors2D() : PropertyMonitorXY("", "", ECutUpdate::kTrack) {}

  void CustomTrackMonitors2D::Update(Bool_t passed, TObject* obj) {
    auto track = (Hal::Track*) obj;
    Double_t a = track->GetFieldVal(fFieldId1);
    Double_t b = track->GetFieldVal(fFieldId2);
    ManualFill2D(a, b, passed);
  }

  Bool_t CustomTrackMonitors2D::Init(Int_t task_id) {
    auto res   = PropertyMonitorXY::Init(task_id);
    auto event = Hal::DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    Int_t step = 0;
    if (IsRe()) step = Hal::DataFieldID::ReStep;
    if (IsIm()) step = Hal::DataFieldID::ImStep;
    if (fFormatType == EFormatType::kComplexReco) step = Hal::DataFieldID::ReStep;
    fFieldId1 += step;
    fFieldId2 += step;
    fXaxisName = event->GetFieldName(fFieldId1);
    fYaxisName = event->GetFieldName(fFieldId2);
    if (fXaxisName == "[]" || fYaxisName == "[]") return kFALSE;
    return res;
  }

  CustomEventMonitors2D::CustomEventMonitors2D() : PropertyMonitorXY("", "", ECutUpdate::kEvent) {}

  void CustomEventMonitors2D::Update(Bool_t passed, TObject* obj) {
    auto event = (Hal::Event*) obj;
    Double_t a = event->GetFieldVal(fFieldId1);
    Double_t b = event->GetFieldVal(fFieldId2);
    ManualFill2D(a, b, passed);
  }

  Bool_t CustomEventMonitors2D::Init(Int_t task_id) {
    auto res   = PropertyMonitorXY::Init(task_id);
    auto event = Hal::DataFormatManager::Instance()->GetFormat(task_id, EFormatDepth::kNonBuffered);
    Int_t step = 0;
    if (IsRe()) step = Hal::DataFieldID::ReStep;
    if (IsIm()) step = Hal::DataFieldID::ImStep;
    if (fFormatType == EFormatType::kComplexReco) step = Hal::DataFieldID::ReStep;
    fFieldId1 += step;
    fFieldId2 += step;
    fXaxisName = event->GetFieldName(fFieldId1);
    fYaxisName = event->GetFieldName(fFieldId2);
    if (fXaxisName == "[]" || fYaxisName == "[]") return kFALSE;
    return res;
  }
} /* namespace Hal */
