/*
 * TrackCutComplex.cxx
 *
 *  Created on: 9 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackComplexCut.h"

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "Cout.h"
#include "Cut.h"
#include "DataFormatManager.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"

#include <TString.h>


namespace Hal {
  Bool_t TrackComplexCut::Pass(Track* track) {
    ComplexTrack* z_track = (ComplexTrack*) track;
    Bool_t stat2          = kTRUE;
    if (z_track->GetImgTrack() == nullptr) {
      ++fNullObjects;
      for (int i = 0; i < fImgCut->GetCutSize(); i++)
        fImgCut->SetValue(-DBL_MAX, i);
      stat2 = fImgCut->ForcedUpdate(fAcceptNulls);
    } else {
      stat2 = fImgCut->Pass(z_track->GetImgTrack());
    }
    Bool_t stat1 = fRealCut->Pass(z_track->GetRealTrack());
    for (int i = 0; i < fStep; i++) {
      SetValue(fRealCut->GetValue(i), i);
    }
    for (int i = 0; i < fImgCut->GetCutSize(); i++) {
      SetValue(fImgCut->GetValue(i), fStep + i);
    }
    if (stat1 && stat2) return ForcedUpdate(kTRUE);
    return ForcedUpdate(kFALSE);
  }

  Package* TrackComplexCut::Report() const {
    Package* pack = TrackCut::Report();
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

  TrackComplexCut::TrackComplexCut() :
    TrackCut(1), fRealCut(NULL), fImgCut(NULL), fStep(0), fNullObjects(0), fAcceptNulls(kFALSE) {}

  TrackComplexCut::TrackComplexCut(const TrackCut* real, const TrackCut* img) :
    TrackCut(real->GetCutSize() + img->GetCutSize()),
    fRealCut((TrackCut*) real->MakeCopy()),
    fImgCut((TrackCut*) img->MakeCopy()),
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

  Bool_t TrackComplexCut::Init(Int_t task_id) {
    if (!TrackCut::Init(task_id)) {
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("Failed to init cut %s", ClassName()), EInfo::kLessInfo);
#endif
      return kFALSE;
    }
    DataFormatManager* manager = DataFormatManager::Instance();
    const Event* event         = manager->GetFormat(task_id);
    if (dynamic_cast<const ComplexEvent*>(event)) {
      ComplexEvent* z = (ComplexEvent*) event;
      Int_t ok        = 0;
      manager->SetFormat(z->GetRealEvent(), task_id, EFormatDepth::kNonBuffered, kTRUE);
      ok += fRealCut->Init(task_id);
      if (ok == 0) {
#ifdef HAL_DEBUG
        Cout::PrintInfo(Form("Failed to init %s due to init %s", ClassName(), fRealCut->ClassName()), EInfo::kLessInfo);
#endif
      }
      manager->SetFormat(z->GetImgEvent(), task_id, EFormatDepth::kNonBuffered, kTRUE);
      Int_t dx = fImgCut->Init(task_id);
      if (dx == 0) {
#ifdef HAL_DEBUG
        Cout::PrintInfo(Form("Failed to init %s due to init %s", ClassName(), fImgCut->ClassName()), EInfo::kLessInfo);
#endif
      }
      ok += dx;
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

  TrackComplexCut::TrackComplexCut(const TrackComplexCut& other) : TrackComplexCut(other.fRealCut, other.fImgCut) {
    fNullObjects = other.fNullObjects;
    fAcceptNulls = other.fAcceptNulls;
  }

  TrackComplexCut::TrackComplexCut(TrackCut* cut) : TrackComplexCut(cut, cut) {}

  TString TrackComplexCut::CutName(Option_t* /*opt*/) const {
    return Form("TrackComplexCut(%s,%s)", fRealCut->CutName().Data(), fImgCut->CutName().Data());
  }

  TrackComplexCut::~TrackComplexCut() {
    if (fRealCut) delete fRealCut;
    if (fImgCut) delete fImgCut;
  }

  TrackComplexCut::TrackComplexCut(const TrackCut& real, const TrackCut& img) :
    TrackCut(real.GetCutSize() + img.GetCutSize()),
    fRealCut((TrackCut*) real.MakeCopy()),
    fImgCut((TrackCut*) img.MakeCopy()),
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

  TrackRealCut::TrackRealCut() : TrackCut(1), fRealCut(nullptr) {}

  TrackRealCut::TrackRealCut(const TrackCut* real) : TrackCut(real->GetCutSize()), fRealCut((TrackCut*) real->MakeCopy()) {
    for (int i = 0; i < real->GetCutSize(); i++) {
      SetUnitName(real->GetUnit(i) + "_{re}", i);
      SetMinMax(real->GetMin(i), real->GetMax(i), i);
    }
  }

  TrackRealCut::TrackRealCut(const TrackCut& real) : TrackCut(real.GetCutSize()), fRealCut((TrackCut*) real.MakeCopy()) {
    for (int i = 0; i < real.GetCutSize(); i++) {
      SetUnitName(real.GetUnit(i) + "_{re}", i);
      SetMinMax(real.GetMin(i), real.GetMax(i), i);
    }
  }

  TrackRealCut::TrackRealCut(const TrackRealCut& other) : TrackRealCut(other.GetRealCut()) {}

  Bool_t TrackRealCut::Pass(Track* track) {
    ComplexTrack* z_track = (ComplexTrack*) track;
    Bool_t passed         = fRealCut->Pass(z_track->GetRealTrack());
    for (int i = 0; i < GetCutSize(); i++) {
      SetValue(fRealCut->GetValue(i), i);
    }
    return ForcedUpdate(passed);
  }

  Bool_t TrackRealCut::Init(Int_t task_id) {
    if (!TrackCut::Init(task_id)) return kFALSE;
    DataFormatManager* manager = DataFormatManager::Instance();
    const Event* event         = manager->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (dynamic_cast<const ComplexEvent*>(event)) {
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

  Package* TrackRealCut::Report() const {
    Package* pack = TrackCut::Report();
    pack->AddObject(new ParameterString("CutName_{re}", fRealCut->CutName()));
    pack->AddObject(new ParameterInt("CutSize_{re}", fRealCut->GetCutSize()));
    pack->AddObject(new ParameterULong64("Passed_{re}", fRealCut->GetPassed(), '+'));
    pack->AddObject(new ParameterULong64("Failed_{re}", fRealCut->GetFailed(), '+'));
    pack->AddObject(fRealCut->Report());
    return pack;
  }

  TString TrackRealCut::CutName(Option_t* /*opt*/) const { return Form("TrackRealCut(%s)", fRealCut->CutName().Data()); }

  TrackRealCut::~TrackRealCut() {
    if (fRealCut) delete fRealCut;
  }

  //============================================

  TrackImaginaryCut::TrackImaginaryCut() : TrackCut(1), fImgCut(nullptr), fNullObjects(0), fAcceptNulls(kFALSE) {}

  TrackImaginaryCut::TrackImaginaryCut(const TrackCut* img) :
    TrackCut(img->GetCutSize()), fImgCut((TrackCut*) img->MakeCopy()), fNullObjects(0), fAcceptNulls(kFALSE) {
    for (int i = 0; i < img->GetCutSize(); i++) {
      SetUnitName(img->GetUnit(i) + "_{im}", i);
      SetMinMax(img->GetMin(i), img->GetMax(i), i);
    }
  }

  TrackImaginaryCut::TrackImaginaryCut(const TrackCut& img) :
    TrackCut(img.GetCutSize()), fImgCut((TrackCut*) img.MakeCopy()), fNullObjects(0), fAcceptNulls(kFALSE) {
    for (int i = 0; i < img.GetCutSize(); i++) {
      SetUnitName(img.GetUnit(i) + "_{im}", i);
      SetMinMax(img.GetMin(i), img.GetMax(i), i);
    }
  }

  TrackImaginaryCut::TrackImaginaryCut(const TrackImaginaryCut& other) : TrackImaginaryCut(other.GetImgCut()) {
    fNullObjects = other.fNullObjects;
    fAcceptNulls = other.fAcceptNulls;
  }

  Bool_t TrackImaginaryCut::Pass(Track* track) {
    ComplexTrack* z_track = (ComplexTrack*) track;
    if (z_track->GetImgTrack() == nullptr) {
      ++fNullObjects;
      for (int i = 0; i < GetCutSize(); i++) {
        fImgCut->SetValue(-DBL_MAX, i);
        SetValue(-DBL_MAX, i);
      }
      fImgCut->ForcedUpdate(fAcceptNulls);
      return ForcedUpdate(fAcceptNulls);
    }
    Bool_t passed = fImgCut->Pass(z_track->GetImgTrack());
    for (int i = 0; i < GetCutSize(); i++) {
      SetValue(fImgCut->GetValue(i), i);
    }
    return ForcedUpdate(passed);
  }

  Bool_t TrackImaginaryCut::Init(Int_t task_id) {
    if (!TrackCut::Init(task_id)) return kFALSE;
    DataFormatManager* manager = DataFormatManager::Instance();
    const Event* event         = manager->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (dynamic_cast<const ComplexEvent*>(event)) {
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

  Package* TrackImaginaryCut::Report() const {
    Package* pack = TrackCut::Report();
    pack->AddObject(new ParameterString("CutName_{im}", fImgCut->CutName()));
    pack->AddObject(new ParameterInt("CutSize_{im}", fImgCut->GetCutSize()));
    pack->AddObject(new ParameterULong64("Passed_{im}", fImgCut->GetPassed(), '+'));
    pack->AddObject(new ParameterULong64("Failed_{im}", fImgCut->GetFailed(), '+'));
    pack->AddObject(new ParameterULong64("Nulls", fNullObjects, '+'));
    pack->AddObject(new ParameterBool("Accept Nulls", fAcceptNulls));
    pack->AddObject(fImgCut->Report());
    return pack;
  }

  TString TrackImaginaryCut::CutName(Option_t* /*opt*/) const { return Form("TrackImaginaryCut(%s)", fImgCut->CutName().Data()); }

  TrackImaginaryCut::~TrackImaginaryCut() {
    if (fImgCut) delete fImgCut;
  }
}  // namespace Hal
