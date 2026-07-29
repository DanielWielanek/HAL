/*
 * CutMonitorComplex.cxx
 *
 *  Created on: 1 sie 2025
 *      Author: daniel
 */

#include "CutMonitorComplex.h"

#include <RtypesCore.h>

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "CutMonitor.h"
#include "DataFormatManager.h"
#include "Package.h"
#include "Parameter.h"
#include "TwoTrack.h"

namespace Hal {

  ComplexMonitor::ComplexMonitor() {}

  ComplexMonitor::ComplexMonitor(const ComplexMonitor& other) : CutMonitor(other) {
    if (other.fMonitor) fMonitor = other.fMonitor->MakeCopy();
  }

  ComplexMonitor::ComplexMonitor(const CutMonitor* other) : CutMonitor(other->GetAxisNo()) {
    fUpdateRatio = other->GetUpdateRatio();
    fMonitor     = other->MakeCopy();
  }

  ComplexMonitor& ComplexMonitor::operator=(const ComplexMonitor& other) {
    if (this != &other) {
      CutMonitor::operator=(other);
      if (fMonitor) delete fMonitor;
      if (other.fMonitor) fMonitor = other.fMonitor->MakeCopy();
    }
    return *this;
  }

  Bool_t ComplexMonitor::ObjMonitor() const { return kTRUE; }

  Hal::Package* ComplexMonitor::Report() const {
    auto report      = new Hal::Package(this);
    auto innerReport = fMonitor->Report();
    innerReport->MoveInnerObjects(report);
    report->AddObject(new Hal::ParameterString("OriginMonitor", fMonitor->ClassName()));
    return report;
  }

  void ComplexMonitor::Update(Bool_t /*passed*/, TObject* /*obj*/) {}

  Bool_t ComplexMonitor::Init(Int_t task_id) {
    auto event = Hal::DataFormatManager::Instance()->GetFormat(task_id);
    if (!dynamic_cast<const Hal::ComplexEvent*>(event)) return kFALSE;
    fMonitor->SetCollectionID(fCollectionID);
    return fMonitor->Init(task_id);
  }

  ComplexMonitor::~ComplexMonitor() {
    if (fMonitor) delete fMonitor;
  }

  //======================================================================

  MonitorComplexPair::MonitorComplexPair() {}

  void MonitorComplexPair::BuildReal(TwoTrack* pair) {
    fOrigin = pair;
    SetPairType(pair->GetPairType());
    AddFirstTrack(((Hal::ComplexTrack*) pair->GetTrack1())->GetRealTrack());
    AddSecondTrack(((Hal::ComplexTrack*) pair->GetTrack2())->GetRealTrack());
  }

  void MonitorComplexPair::BuildImag(TwoTrack* pair) {
    fOrigin = pair;
    SetPairType(pair->GetPairType());
    AddFirstTrack(((Hal::ComplexTrack*) pair->GetTrack1())->GetImgTrack());
    AddSecondTrack(((Hal::ComplexTrack*) pair->GetTrack2())->GetImgTrack());
  }

  MonitorComplexPair::~MonitorComplexPair() {}


  //======================================================

  void EventCutMonitorImaginary::Update(Bool_t passed, TObject* obj) {
    auto event = ((Hal::ComplexEvent*) obj)->GetImgEvent();
    if (!event) return;
    fMonitor->Update(passed, event);
  }

  Hal::CutMonitor* EventCutMonitorImaginary::MakeCopy(const CutOptions& /*opt*/) const {
    return new EventCutMonitorImaginary(*this);
  }

  //======================================================

  void TrackCutMonitorImaginary::Update(Bool_t passed, TObject* obj) {
    auto track = ((Hal::ComplexTrack*) obj)->GetImgTrack();
    if (!track) return;
    fMonitor->Update(passed, track);
  }

  Hal::CutMonitor* TrackCutMonitorImaginary::MakeCopy(const CutOptions& /*opt*/) const {
    return new TrackCutMonitorImaginary(*this);
  }

  //======================================================

  void TwoTrackCutMonitorImaginary::Update(Bool_t passed, TObject* obj) {
    fPair.BuildImag((TwoTrack*) obj);
    fMonitor->Update(passed, &fPair);
  }

  Hal::CutMonitor* TwoTrackCutMonitorImaginary::MakeCopy(const CutOptions& /*opt*/) const {
    return new TwoTrackCutMonitorImaginary(*this);
  }

  //======================================================

  void EventCutMonitorReal::Update(Bool_t passed, TObject* obj) {
    auto event = ((Hal::ComplexEvent*) obj)->GetRealEvent();
    fMonitor->Update(passed, event);
  }

  Hal::CutMonitor* EventCutMonitorReal::MakeCopy(const CutOptions& /*opt*/) const { return new EventCutMonitorReal(*this); }

  //======================================================

  void TrackCutMonitorReal::Update(Bool_t passed, TObject* obj) {
    auto track = ((Hal::ComplexTrack*) obj)->GetRealTrack();
    fMonitor->Update(passed, track);
  }

  Hal::CutMonitor* TrackCutMonitorReal::MakeCopy(const CutOptions& /*opt*/) const { return new TrackCutMonitorReal(*this); }

  //======================================================

  void TwoTrackCutMonitorReal::Update(Bool_t passed, TObject* obj) {
    fPair.BuildReal((TwoTrack*) obj);
    fMonitor->Update(passed, &fPair);
  }

  Hal::CutMonitor* TwoTrackCutMonitorReal::MakeCopy(const CutOptions& /*opt*/) const { return new TwoTrackCutMonitorReal(*this); }

  Bool_t ComplexMonitor::AreSimilar(const Hal::CutMonitor& other) const {
    TString classThis  = ClassName();
    TString otherClass = other.ClassName();
    if (classThis != otherClass) return kFALSE;
    auto otherMon = dynamic_cast<const Hal::ComplexMonitor*>(&other);
    if (otherMon == nullptr) return kFALSE;
    if (fMonitor->AreSimilar(*otherMon->fMonitor)) return kTRUE;
    return kFALSE;
  }

  void ComplexMonitor::MakeComplexAxes(const CutOptions& opt) {
    if (fMonitor) fMonitor->MakeComplexAxes(opt);
  }

} /* namespace Hal */
