/*
 * MonitorTof.cxx
 *
 *  Created on: 9 lip 2025
 *      Author: daniel
 */

#include "MonitorTof.h"

#include <RtypesCore.h>
#include <TH2.h>

#include "DataFormat.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "ExpTrack.h"
#include "Std.h"

namespace Hal {
  MonitorTofM2::MonitorTofM2() : PropertyMonitorXY("p [GeV/c]", "p_{T} [GeV^{2}/c^{4}]", Hal::ECutUpdate::kTrack) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, -0.5, 2.5);
  }

  Bool_t MonitorTofM2::Init(Int_t task_id) {
    auto dataManager = Hal::DataFormatManager::Instance();
    auto event       = dataManager->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (event->GetFieldVal(Hal::DataFieldID::Track::EExp::kTofM2)) return kTRUE;
    return kFALSE;
  }

  void MonitorTofM2::Update(Bool_t passed, TObject* obj) {
    auto track    = (Hal::ExpTrack*) obj;
    auto tofTrack = (Hal::ToFTrack*) track->GetDetTrack(Hal::DetectorID::kTOF);
    double valueY = tofTrack->GetMass2();
    double valueX = track->GetMomentum().P() * track->GetCharge();
    if (tofTrack->GetFlag() == Hal::DetectorTrack::FlagBad()) { valueY = Hal::Const::DummyVal(); }
    if (passed) {
      ((TH2*) fHistoPassed)->Fill(valueX, valueY);
    } else {
      ((TH2*) fHistoFailed)->Fill(valueX, valueY);
    }
  }

  MonitorTofBeta::MonitorTofBeta() : PropertyMonitorXY("p [GeV/c]", "#beta [AU]", Hal::ECutUpdate::kTrack) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, -0.25, 1.25);
  }

  void MonitorTofBeta::Update(Bool_t passed, TObject* obj) {
    auto track    = (Hal::ExpTrack*) obj;
    auto tofTrack = (Hal::ToFTrack*) track->GetDetTrack(Hal::DetectorID::kTOF);
    double valueY = tofTrack->GetBeta();
    double valueX = track->GetMomentum().P() * track->GetCharge();
    if (tofTrack->GetFlag() == Hal::DetectorTrack::FlagBad()) { valueY = Hal::Const::DummyVal(); }
    if (passed) {
      ((TH2*) fHistoPassed)->Fill(valueX, valueY);
    } else {
      ((TH2*) fHistoFailed)->Fill(valueX, valueY);
    }
  }

  Bool_t MonitorTofBeta::Init(Int_t task_id) {
    auto dataManager = Hal::DataFormatManager::Instance();
    auto event       = dataManager->GetFormat(task_id, EFormatDepth::kNonBuffered);
    if (event->GetFieldVal(Hal::DataFieldID::Track::EExp::kToFBeta)) return kTRUE;
    return kFALSE;
  }
}  // namespace Hal
