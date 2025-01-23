/*
 * CutMonitorResoSph.cxx
 *
 *  Created on: 23 sty 2025
 *      Author: daniel
 */

#include "MonitorReso.h"

#include <RtypesCore.h>
#include <TH2.h>

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "Cut.h"
#include "DataFormatManager.h"
#include "McEvent.h"
#include "Std.h"
#include "TrackCut.h"

namespace Hal {

  MonitorResoP::MonitorResoP() : PropertyMonitorXY("p_{sim} [GeV/c]", "#Delta p [GeV/c]", Hal::ECutUpdate::kTrack) {
    SetAxis(100, 0, 2, 0);
    SetAxis(100, -0.1, 0.1, 1);
  }

  Bool_t MonitorResoP::Init(Int_t task_id) {
    if (PropertyMonitorXY::Init(task_id) == kFALSE) return kFALSE;
    if (fFormatType != EFormatType::kComplexReco) return kFALSE;
    return kTRUE;
  }

  void MonitorResoP::Update(Bool_t passed, TObject* obj) {
    auto reco_track = ((Hal::ComplexTrack*) obj)->GetRealTrack();
    auto sim_track  = ((Hal::ComplexTrack*) obj)->GetImgTrack();
    if (!sim_track) return;
    Double_t x  = sim_track->GetMomentum().P();
    Double_t dx = x - reco_track->GetMomentum().P();
    ManualFill2D(x, dx, passed);
  }

  MonitorResoTheta::MonitorResoTheta() : PropertyMonitorXY("p_{sim} [GeV/c]", "#Delta #theta [rad]", Hal::ECutUpdate::kTrack) {
    SetAxis(100, 0, 2, 0);
    SetAxis(100, -TMath::Pi() * 0.25, TMath::Pi() * 0.25, 1);
  }

  Bool_t MonitorResoTheta::Init(Int_t task_id) {
    if (PropertyMonitorXY::Init(task_id) == kFALSE) return kFALSE;
    if (fFormatType != EFormatType::kComplexReco) return kFALSE;
    return kTRUE;
  }

  void MonitorResoTheta::Update(Bool_t passed, TObject* obj) {
    auto reco_track = ((Hal::ComplexTrack*) obj)->GetRealTrack();
    auto sim_track  = ((Hal::ComplexTrack*) obj)->GetImgTrack();
    if (!sim_track) return;
    Double_t x  = sim_track->GetMomentum().P();
    Double_t dx = sim_track->GetMomentum().Theta() - reco_track->GetMomentum().Theta();
    dx          = TVector2::Phi_mpi_pi(dx);
    ManualFill2D(x, dx, passed);
  }

  MonitorResoPhi::MonitorResoPhi() : PropertyMonitorXY("p_{sim} [GeV/c]", "#Delta #phi [rad]", Hal::ECutUpdate::kTrack) {
    SetAxis(100, 0, 2, 0);
    SetAxis(100, -TMath::Pi() * 0.25, TMath::Pi() * 0.25, 1);
  }

  Bool_t MonitorResoPhi::Init(Int_t task_id) {
    if (PropertyMonitorXY::Init(task_id) == kFALSE) return kFALSE;
    if (fFormatType != EFormatType::kComplexReco) return kFALSE;
    return kTRUE;
  }

  void MonitorResoPhi::Update(Bool_t passed, TObject* obj) {
    auto reco_track = ((Hal::ComplexTrack*) obj)->GetRealTrack();
    auto sim_track  = ((Hal::ComplexTrack*) obj)->GetImgTrack();
    if (!sim_track) return;
    Double_t x  = sim_track->GetMomentum().P();
    Double_t dx = sim_track->GetMomentum().Phi() - reco_track->GetMomentum().Phi();
    dx          = TVector2::Phi_mpi_pi(dx);
    ManualFill2D(x, dx, passed);
  }

} /* namespace Hal */
