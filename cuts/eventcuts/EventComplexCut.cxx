/*
 * EventComplexCut.cxx
 *
 *  Created on: 24 maj 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventComplexCut.h"

#include "ComplexEvent.h"
#include "Cout.h"
#include "DataFormatManager.h"
#include "Package.h"
#include "Parameter.h"


namespace Hal {
  EventComplexCut::EventComplexCut() :
    EventCut(1), fRealCut(NULL), fImgCut(NULL), fStep(0), fNullObjects(0), fAcceptNulls(kFALSE) {}

  EventComplexCut::EventComplexCut(const EventCut* real, const EventCut* img) :
    EventCut(real->GetCutSize() + img->GetCutSize()),
    fRealCut((EventCut*) real->MakeCopy()),
    fImgCut((EventCut*) img->MakeCopy()),
    fNullObjects(0),
    fAcceptNulls(kFALSE) {
    for (int i = 0; i < real->GetCutSize(); i++) {
      SetUnitName(real->GetUnit(i) + "_{re}", i);
      SetMinMax(real->GetMin(i), real->GetMax(i), i);
    }
    fStep = real->GetCutSize();
    int j = 0;
    for (int i = fStep; i < GetCutSize(); i++) {
      SetUnitName(img->GetUnit(j) + "_{img}", i);
      SetMinMax(img->GetMin(j), img->GetMax(j), i);
      j++;
    }
  }

  EventComplexCut::EventComplexCut(const EventComplexCut& other) : EventComplexCut(other.fRealCut, other.fImgCut) {
    fNullObjects = other.fNullObjects;
    fAcceptNulls = other.fAcceptNulls;
  }

  Bool_t EventComplexCut::Pass(Event* event) {
    ComplexEvent* z_event = (ComplexEvent*) event;
    Bool_t stat2          = kTRUE;
    if (z_event->GetImgEvent() == nullptr) {
      ++fNullObjects;
      for (int i = 0; i < fImgCut->GetCutSize(); i++)
        fImgCut->SetValue(-DBL_MAX, i);
      stat2 = fImgCut->ForcedUpdate(fAcceptNulls);
    } else {
      stat2 = fImgCut->Pass(z_event->GetImgEvent());
    }
    Bool_t stat1 = fRealCut->Pass(z_event->GetRealEvent());
    for (int i = 0; i < fStep; i++) {
      SetValue(fRealCut->GetValue(i), i);
    }
    for (int i = 0; i < fImgCut->GetCutSize(); i++) {
      SetValue(fImgCut->GetValue(i), fStep + i);
    }
    if (stat1 && stat2) return ForcedUpdate(kTRUE);
    return ForcedUpdate(kFALSE);
  }

  Bool_t EventComplexCut::Init(Int_t task_id) {
    if (!EventCut::Init(task_id)) return kFALSE;
    DataFormatManager* manager = DataFormatManager::Instance();
    const Event* event         = manager->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (event->InheritsFrom("Hal::ComplexEvent")) {
      ComplexEvent* z = (ComplexEvent*) event;
      Int_t ok        = 0;
      manager->SetFormat(z->GetRealEvent(), task_id, EFormatDepth::kNonBuffered, kTRUE);
      ok += fRealCut->Init(task_id);
      manager->SetFormat(z->GetImgEvent(), task_id, EFormatDepth::kNonBuffered, kTRUE);
      ok += fImgCut->Init(task_id);
      manager->SetFormat(z, task_id, EFormatDepth::kNonBuffered, kTRUE);
      for (int i = 0; i < fRealCut->GetCutSize(); i++) {
        SetMinMax(fRealCut->GetMin(i), fRealCut->GetMax(i), i);
      }
      for (int i = 0; i < fImgCut->GetCutSize(); i++) {
        SetMinMax(fImgCut->GetMin(i), fImgCut->GetMax(i), i + fStep);
      }
      if (ok == 2) return kTRUE;
    }
    return kFALSE;
  }

  Package* EventComplexCut::Report() const {
    Package* pack = EventCut::Report();
    pack->AddObject(new ParameterString("CutName_{re}", fRealCut->CutName()));
    pack->AddObject(new ParameterString("CutName_{im}", fImgCut->CutName()));
    pack->AddObject(new ParameterInt("CutSize_{re}", fRealCut->GetCutSize()));
    pack->AddObject(new ParameterInt("CutSize_{im}", fImgCut->GetCutSize()));
    pack->AddObject(new ParameterULong64("Passed_{re}", fRealCut->GetPassed(), '+'));
    pack->AddObject(new ParameterULong64("Failed_{re}", fRealCut->GetFailed(), '+'));
    pack->AddObject(new ParameterULong64("Passed_{im}", fImgCut->GetPassed(), '+'));
    pack->AddObject(new ParameterULong64("Failed_{im}", fImgCut->GetFailed(), '+'));
    pack->AddObject(new ParameterULong64("Nulls", fNullObjects, '+'));
    pack->AddObject(new ParameterBool("Accept Nulls", fAcceptNulls));
    pack->AddObject(fRealCut->Report());
    pack->AddObject(fImgCut->Report());
    return pack;
  }

  TString EventComplexCut::CutName(Option_t* /*opt*/) const {
    return Form("EventComplexCut(%s,%s)", fRealCut->CutName().Data(), fImgCut->CutName().Data());
  }

  EventComplexCut::~EventComplexCut() {
    if (fRealCut) delete fRealCut;
    if (fImgCut) delete fImgCut;
  }

  EventComplexCut::EventComplexCut(const EventCut& real, const EventCut& img) :
    EventCut(real.GetCutSize() + img.GetCutSize()),
    fRealCut((EventCut*) real.MakeCopy()),
    fImgCut((EventCut*) img.MakeCopy()),
    fNullObjects(0),
    fAcceptNulls(kFALSE) {
    for (int i = 0; i < real.GetCutSize(); i++) {
      SetUnitName(real.GetUnit(i) + "_{re}", i);
      SetMinMax(real.GetMin(i), real.GetMax(i), i);
    }
    fStep = real.GetCutSize();
    int j = 0;
    for (int i = fStep; i < GetCutSize(); i++) {
      SetUnitName(img.GetUnit(j) + "_{img}", i);
      SetMinMax(img.GetMin(j), img.GetMax(j), i);
      j++;
    }
  }

  //============================================

  EventRealCut::EventRealCut() : EventCut(1), fRealCut(nullptr) {}

  EventRealCut::EventRealCut(const EventCut* real) : EventCut(real->GetCutSize()), fRealCut((EventCut*) real->MakeCopy()) {
    for (int i = 0; i < real->GetCutSize(); i++) {
      SetUnitName(real->GetUnit(i) + "_{re}", i);
      SetMinMax(real->GetMin(i), real->GetMax(i), i);
    }
  }

  EventRealCut::EventRealCut(const EventCut& real) : EventCut(real.GetCutSize()), fRealCut((EventCut*) real.MakeCopy()) {
    for (int i = 0; i < real.GetCutSize(); i++) {
      SetUnitName(real.GetUnit(i) + "_{re}", i);
      SetMinMax(real.GetMin(i), real.GetMax(i), i);
    }
  }

  EventRealCut::EventRealCut(const EventRealCut& other) : EventRealCut(other.GetRealCut()) {}

  Bool_t EventRealCut::Pass(Event* event) {
    ComplexEvent* z_event = (ComplexEvent*) event;
    Bool_t passed         = fRealCut->Pass(z_event->GetRealEvent());
    for (int i = 0; i < GetCutSize(); i++) {
      SetValue(fRealCut->GetValue(i), i);
    }
    return ForcedUpdate(passed);
  }

  Bool_t EventRealCut::Init(Int_t task_id) {
    if (!EventCut::Init(task_id)) return kFALSE;
    DataFormatManager* manager = DataFormatManager::Instance();
    const Event* event         = manager->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (event->InheritsFrom("Hal::ComplexEvent")) {
      ComplexEvent* z = (ComplexEvent*) event;
      manager->SetFormat(z->GetRealEvent(), task_id, EFormatDepth::kNonBuffered, kTRUE);
      Bool_t ok = fRealCut->Init(task_id);
      manager->SetFormat(z, task_id, EFormatDepth::kNonBuffered, kTRUE);
      for (int i = 0; i < fRealCut->GetCutSize(); i++) {
        SetMinMax(fRealCut->GetMin(i), fRealCut->GetMax(i), i);
      }
      if (ok) return kTRUE;
    }

    return kFALSE;
  }

  Package* EventRealCut::Report() const {
    Package* pack = EventCut::Report();
    pack->AddObject(new ParameterString("CutName_{re}", fRealCut->CutName()));
    pack->AddObject(new ParameterInt("CutSize_{re}", fRealCut->GetCutSize()));
    pack->AddObject(new ParameterULong64("Passed_{re}", fRealCut->GetPassed(), '+'));
    pack->AddObject(new ParameterULong64("Failed_{re}", fRealCut->GetFailed(), '+'));
    pack->AddObject(fRealCut->Report());
    return pack;
  }

  TString EventRealCut::CutName(Option_t* /*opt*/) const { return Form("EventRealCut(%s)", fRealCut->CutName().Data()); }

  EventRealCut::~EventRealCut() {
    if (fRealCut) delete fRealCut;
  }

  //============================================

  EventImaginaryCut::EventImaginaryCut() : EventCut(1), fImgCut(nullptr), fNullObjects(0), fAcceptNulls(kFALSE) {}

  EventImaginaryCut::EventImaginaryCut(const EventCut* img) :
    EventCut(img->GetCutSize()), fImgCut((EventCut*) img->MakeCopy()), fNullObjects(0), fAcceptNulls(kFALSE) {
    for (int i = 0; i < img->GetCutSize(); i++) {
      SetUnitName(img->GetUnit(i) + "_{im}", i);
      SetMinMax(img->GetMin(i), img->GetMax(i), i);
    }
  }

  EventImaginaryCut::EventImaginaryCut(const EventCut& img) :
    EventCut(img.GetCutSize()), fImgCut((EventCut*) img.MakeCopy()), fNullObjects(0), fAcceptNulls(kFALSE) {
    for (int i = 0; i < img.GetCutSize(); i++) {
      SetUnitName(img.GetUnit(i) + "_{im}", i);
      SetMinMax(img.GetMin(i), img.GetMax(i), i);
    }
  }

  EventImaginaryCut::EventImaginaryCut(const EventImaginaryCut& other) : EventImaginaryCut(other.GetImgCut()) {
    fNullObjects = other.fNullObjects;
    fAcceptNulls = other.fAcceptNulls;
  }

  Bool_t EventImaginaryCut::Pass(Event* event) {
    ComplexEvent* z_event = (ComplexEvent*) event;
    if (z_event->GetImgEvent() == nullptr) {
      for (int i = 0; i < GetCutSize(); i++) {
        fImgCut->SetValue(-DBL_MAX, i);
        SetValue(-DBL_MAX, i);
      }
      ++fNullObjects;
      fImgCut->ForcedUpdate(fAcceptNulls);
      return ForcedUpdate(fAcceptNulls);
    }

    Bool_t passed = fImgCut->Pass(z_event->GetImgEvent());
    for (int i = 0; i < GetCutSize(); i++) {
      SetValue(fImgCut->GetValue(i), i);
    }
    return ForcedUpdate(passed);
  }

  Bool_t EventImaginaryCut::Init(Int_t task_id) {
    if (!EventCut::Init(task_id)) return kFALSE;
    DataFormatManager* manager = DataFormatManager::Instance();
    const Event* event         = manager->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (event->InheritsFrom("Hal::ComplexEvent")) {
      ComplexEvent* z = (ComplexEvent*) event;
      manager->SetFormat(z->GetImgEvent(), task_id, EFormatDepth::kNonBuffered, kTRUE);
      Bool_t ok = fImgCut->Init(task_id);
      manager->SetFormat(z, task_id, EFormatDepth::kNonBuffered, kTRUE);
      for (int i = 0; i < fImgCut->GetCutSize(); i++) {
        SetMinMax(fImgCut->GetMin(i), fImgCut->GetMax(i), i);
      }
      if (ok) return kTRUE;
    }
    return kFALSE;
  }

  Package* EventImaginaryCut::Report() const {
    Package* pack = EventCut::Report();
    pack->AddObject(new ParameterString("CutName_{im}", fImgCut->CutName()));
    pack->AddObject(new ParameterInt("CutSize_{im}", fImgCut->GetCutSize()));
    pack->AddObject(new ParameterULong64("Passed_{im}", fImgCut->GetPassed(), '+'));
    pack->AddObject(new ParameterULong64("Failed_{im}", fImgCut->GetFailed(), '+'));
    pack->AddObject(new ParameterULong64("Nulls", fNullObjects, '+'));
    pack->AddObject(new ParameterBool("Accept Nulls", fAcceptNulls));
    pack->AddObject(fImgCut->Report());
    return pack;
  }

  TString EventImaginaryCut::CutName(Option_t* /*opt*/) const { return Form("EventImaginaryCut(%s)", fImgCut->CutName().Data()); }

  EventImaginaryCut::~EventImaginaryCut() {
    if (fImgCut) delete fImgCut;
  }
}  // namespace Hal
