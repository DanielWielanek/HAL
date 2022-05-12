/*
 * TwoTrackComplexCut.cxx
 *
 *  Created on: 24 maj 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackComplexCut.h"

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "Cut.h"
#include "DataFormatManager.h"
#include "HalStd.h"
#include "Package.h"
#include "Parameter.h"
#include "TwoTrack.h"

#include <TString.h>


namespace Hal {
  TwoTrackComplexCut::TwoTrackComplexCut() :
    TwoTrackCut(1), fRealCut(nullptr), fImgCut(nullptr), fNullObjects(0), fAcceptNulls(kTRUE) {
    fStep     = 0;
    fRealPair = new TwoTrack();
    fImgPair  = new TwoTrack();
  }

  TwoTrackComplexCut::TwoTrackComplexCut(const TwoTrackCut* real, const TwoTrackCut* img) :
    TwoTrackCut(real->GetCutSize() + img->GetCutSize()),
    fRealCut((TwoTrackCut*) real->MakeCopy()),
    fImgCut((TwoTrackCut*) img->MakeCopy()),
    fStep(0),
    fNullObjects(0),
    fAcceptNulls(kTRUE) {
    fRealPair = new TwoTrack();
    fImgPair  = new TwoTrack();
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

  TwoTrackComplexCut::TwoTrackComplexCut(const TwoTrackComplexCut& other) : TwoTrackComplexCut(other.fRealCut, other.fImgCut) {
    fNullObjects = other.fNullObjects;
    fAcceptNulls = other.fAcceptNulls;
  }

  Bool_t TwoTrackComplexCut::Pass(TwoTrack* pair) {
    ComplexTrack* track1 = (ComplexTrack*) pair->GetTrack1();
    ComplexTrack* track2 = (ComplexTrack*) pair->GetTrack2();
    fRealPair->FastBuild(track1->GetRealTrack(), track2->GetRealTrack());
    fImgPair->FastBuild(track1->GetImgTrack(), track2->GetImgTrack());
    Bool_t stat2 = kTRUE;
    if (fImgPair->GetTrack1() == nullptr || fImgPair->GetTrack2() == nullptr) {
      ++fNullObjects;
      for (int i = 0; i < fStep; i++)
        fImgCut->SetValue(-DBL_MAX, i);
      stat2 = fImgCut->ForcedUpdate(fAcceptNulls);
    } else {
      stat2 = fImgCut->Pass(fImgPair);
    }
    Bool_t stat1 = fRealCut->Pass(fRealPair);
    for (int i = 0; i < fStep; i++) {
      SetValue(fRealCut->GetValue(i), i);
    }
    for (int i = 0; i < fImgCut->GetCutSize(); i++) {
      SetValue(fImgCut->GetValue(i), fStep + i);
    }
    if (stat1 && stat2) return ForcedUpdate(kTRUE);
    return ForcedUpdate(kFALSE);
  }

  Bool_t TwoTrackComplexCut::Init(Int_t task_id) {
    if (!TwoTrackCut::Init(task_id)) return kFALSE;
    DataFormatManager* manager = DataFormatManager::Instance();
    const Event* event         = manager->GetFormat(task_id, EFormatDepth::kBuffered);
    if (event->InheritsFrom("ComplexEvent")) {
      ComplexEvent* z = (ComplexEvent*) event;
      Int_t ok        = 0;
      manager->SetFormat(z->GetRealEvent(), task_id, EFormatDepth::kBuffered, kTRUE);
      ok += fRealCut->Init(task_id);
      manager->SetFormat(z->GetImgEvent(), task_id, EFormatDepth::kBuffered, kTRUE);
      ok += fImgCut->Init(task_id);
      manager->SetFormat(z, task_id, EFormatDepth::kBuffered, kTRUE);
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

  Package* TwoTrackComplexCut::Report() const {
    Package* pack = TwoTrackCut::Report();
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

  TString TwoTrackComplexCut::CutName(Option_t* /*opt*/) const {
    return Form("TwoTrackComplexCut(%s,%s)", fRealCut->CutName().Data(), fImgCut->CutName().Data());
  }

  TwoTrackComplexCut::~TwoTrackComplexCut() {
    if (fImgCut) delete fImgCut;
    if (fRealCut) delete fRealCut;
    delete fImgPair;
    delete fRealPair;
  }

  TwoTrackComplexCut::TwoTrackComplexCut(const TwoTrackCut& real, const TwoTrackCut& img) :
    TwoTrackCut(real.GetCutSize() + img.GetCutSize()),
    fRealCut((TwoTrackCut*) real.MakeCopy()),
    fImgCut((TwoTrackCut*) img.MakeCopy()),
    fStep(0),
    fNullObjects(0),
    fAcceptNulls(kFALSE) {
    fRealPair = new TwoTrack();
    fImgPair  = new TwoTrack();
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

  TwoTrackRealCut::TwoTrackRealCut() : TwoTrackCut(1), fRealCut(nullptr), fPair(new TwoTrack()) {}

  TwoTrackRealCut::TwoTrackRealCut(const TwoTrackCut* real) :
    TwoTrackCut(real->GetCutSize()), fRealCut((TwoTrackCut*) real->MakeCopy()), fPair(new TwoTrack()) {
    for (int i = 0; i < real->GetCutSize(); i++) {
      SetUnitName(real->GetUnit(i) + "_{re}", i);
      SetMinMax(real->GetMin(i), real->GetMax(i), i);
    }
  }

  TwoTrackRealCut::TwoTrackRealCut(const TwoTrackCut& real) :
    TwoTrackCut(real.GetCutSize()), fRealCut((TwoTrackCut*) real.MakeCopy()), fPair(new TwoTrack()) {
    for (int i = 0; i < real.GetCutSize(); i++) {
      SetUnitName(real.GetUnit(i) + "_{re}", i);
      SetMinMax(real.GetMin(i), real.GetMax(i), i);
    }
  }

  TwoTrackRealCut::TwoTrackRealCut(const TwoTrackRealCut& other) : TwoTrackRealCut(other.GetRealCut()) {}

  Bool_t TwoTrackRealCut::Pass(TwoTrack* pair) {
    ComplexTrack* tr1 = (ComplexTrack*) pair->GetTrack1();
    ComplexTrack* tr2 = (ComplexTrack*) pair->GetTrack2();
    fPair->FastBuild(tr1->GetRealTrack(), tr2->GetRealTrack());
    Bool_t passed = fRealCut->Pass(fPair);
    for (int i = 0; i < GetCutSize(); i++) {
      SetValue(fRealCut->GetValue(i), i);
    }
    return ForcedUpdate(passed);
  }

  Bool_t TwoTrackRealCut::Init(Int_t task_id) {
    if (!TwoTrackCut::Init(task_id)) return kFALSE;
    DataFormatManager* manager = DataFormatManager::Instance();
    const Event* event         = manager->GetFormat(task_id, EFormatDepth::kBuffered);
    if (event->InheritsFrom("ComplexEvent")) {
      ComplexEvent* z = (ComplexEvent*) event;
      manager->SetFormat(z->GetRealEvent(), task_id, EFormatDepth::kBuffered, kTRUE);
      Bool_t ok = fRealCut->Init(task_id);
      manager->SetFormat(z, task_id, EFormatDepth::kBuffered, kTRUE);
      for (int i = 0; i < fRealCut->GetCutSize(); i++) {
        SetMinMax(fRealCut->GetMin(i), fRealCut->GetMax(i), i);
      }
      if (ok) return kTRUE;
    }
    return kFALSE;
  }

  Package* TwoTrackRealCut::Report() const {
    Package* pack = TwoTrackCut::Report();
    pack->AddObject(new ParameterString("CutName_{re}", fRealCut->CutName()));
    pack->AddObject(new ParameterInt("CutSize_{re}", fRealCut->GetCutSize()));
    pack->AddObject(new ParameterULong64("Passed_{re}", fRealCut->GetPassed(), '+'));
    pack->AddObject(new ParameterULong64("Failed_{re}", fRealCut->GetFailed(), '+'));
    pack->AddObject(fRealCut->Report());
    return pack;
  }

  TString TwoTrackRealCut::CutName(Option_t* /*opt*/) const { return Form("TwoTrackRealCut(%s)", fRealCut->CutName().Data()); }

  TwoTrackRealCut::~TwoTrackRealCut() {
    if (fRealCut) delete fRealCut;
    if (fPair) delete fPair;
  }

  //============================================
  TwoTrackImaginaryCut::TwoTrackImaginaryCut() :
    TwoTrackCut(1), fImgCut(nullptr), fPair(new TwoTrack()), fNullObjects(0), fAcceptNulls(kFALSE) {}

  TwoTrackImaginaryCut::TwoTrackImaginaryCut(const TwoTrackCut* img) :
    TwoTrackCut(img->GetCutSize()),
    fImgCut((TwoTrackCut*) img->MakeCopy()),
    fPair(new TwoTrack()),
    fNullObjects(0),
    fAcceptNulls(kFALSE) {
    for (int i = 0; i < img->GetCutSize(); i++) {
      SetUnitName(img->GetUnit(i) + "_{im}", i);
      SetMinMax(img->GetMin(i), img->GetMax(i), i);
    }
  }

  TwoTrackImaginaryCut::TwoTrackImaginaryCut(const TwoTrackCut& img) :
    TwoTrackCut(img.GetCutSize()),
    fImgCut((TwoTrackCut*) img.MakeCopy()),
    fPair(new TwoTrack()),
    fNullObjects(0),
    fAcceptNulls(kFALSE) {
    for (int i = 0; i < img.GetCutSize(); i++) {
      SetUnitName(img.GetUnit(i) + "_{im}", i);
      SetMinMax(img.GetMin(i), img.GetMax(i), i);
    }
  }

  TwoTrackImaginaryCut::TwoTrackImaginaryCut(const TwoTrackImaginaryCut& other) : TwoTrackImaginaryCut(other.GetImgCut()) {
    fNullObjects = other.fNullObjects;
    fAcceptNulls = other.fAcceptNulls;
  }

  Bool_t TwoTrackImaginaryCut::Pass(TwoTrack* pair) {
    ComplexTrack* tr1 = (ComplexTrack*) pair->GetTrack1();
    ComplexTrack* tr2 = (ComplexTrack*) pair->GetTrack2();

    if (tr1->GetImgTrack() == nullptr || tr2->GetImgTrack() == nullptr) {
      ++fNullObjects;
      for (int i = 0; i < GetCutSize(); i++) {
        SetValue(-DBL_MAX, i);
        fImgCut->SetValue(-DBL_MAX, i);
      }
      fImgCut->ForcedUpdate(fAcceptNulls);
      return ForcedUpdate(fAcceptNulls);
    }

    fPair->FastBuild(tr1->GetImgTrack(), tr2->GetImgTrack());
    Bool_t passed = fImgCut->Pass(fPair);
    for (int i = 0; i < GetCutSize(); i++) {
      SetValue(fImgCut->GetValue(i), i);
    }
    return ForcedUpdate(passed);
  }

  Bool_t TwoTrackImaginaryCut::Init(Int_t task_id) {
    if (!TwoTrackCut::Init(task_id)) return kFALSE;
    DataFormatManager* manager = DataFormatManager::Instance();
    const Event* event         = manager->GetFormat(task_id, EFormatDepth::kBuffered);
    if (event->InheritsFrom("ComplexEvent")) {
      ComplexEvent* z = (ComplexEvent*) event;
      manager->SetFormat(z->GetImgEvent(), task_id, EFormatDepth::kBuffered, kTRUE);
      Bool_t ok = fImgCut->Init(task_id);
      manager->SetFormat(z, task_id, EFormatDepth::kBuffered, kTRUE);
      for (int i = 0; i < fImgCut->GetCutSize(); i++) {
        SetMinMax(fImgCut->GetMin(i), fImgCut->GetMax(i), i);
      }
      if (ok) return kTRUE;
    }
    return kFALSE;
  }

  Package* TwoTrackImaginaryCut::Report() const {
    Package* pack = TwoTrackCut::Report();
    pack->AddObject(new ParameterString("CutName_{im}", fImgCut->CutName()));
    pack->AddObject(new ParameterInt("CutSize_{im}", fImgCut->GetCutSize()));
    pack->AddObject(new ParameterULong64("Passed_{im}", fImgCut->GetPassed(), '+'));
    pack->AddObject(new ParameterULong64("Failed_{im}", fImgCut->GetFailed(), '+'));
    pack->AddObject(new ParameterULong64("Nulls", fNullObjects, '+'));
    pack->AddObject(new ParameterBool("Accept Nulls", fAcceptNulls));
    pack->AddObject(fImgCut->Report());
    return pack;
  }

  TString TwoTrackImaginaryCut::CutName(Option_t* /*opt*/) const {
    return Form("TwoTrackImaginaryCut(%s)", fImgCut->CutName().Data());
  }

  TwoTrackImaginaryCut::~TwoTrackImaginaryCut() {
    if (fImgCut) delete fImgCut;
    if (fPair) delete fPair;
  }
}  // namespace Hal
