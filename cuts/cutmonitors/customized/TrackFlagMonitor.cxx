/*
 * TrackFlagMonitor1D.cxx
 *
 *  Created on: 7 paź 2025
 *      Author: daniel
 */

#include "TrackFlagMonitor.h"

#include <TAxis.h>
#include <TH1.h>

#include "Std.h"
#include "Track.h"

namespace Hal {

  std::vector<Int_t> TrackFlagChecker::GetFlags(Track* track) const {
    std::vector<Int_t> result;
    if (track->IsPrimary()) result.push_back(eFlags::kPrimary);
    if (track->IsGlobal()) result.push_back(eFlags::kGlobal);
    if (track->IsGoodSecondary()) result.push_back(eFlags::kKnownMother);
    if (track->IsV0()) {
      if (track->IsGoodV0()) {
        result.push_back(eFlags::kV0daughtersKnown);
      } else {
        result.push_back(eFlags::kV0daughtersUnknown);
      }
    }
    if (track->IsXi()) {
      if (track->IsGoodXi()) {
        result.push_back(eFlags::kXidaughtersKnown);
      } else {
        result.push_back(eFlags::kXidaughersUnknown);
      }
    }
    if (track->IsBackground()) result.push_back(eFlags::kBackground);
    if (track->IsEmbedded()) result.push_back(eFlags::kEmbeded);
    result.push_back(eFlags::kAny);
    return result;
  }

  void TrackFlagChecker::SetupAxis(TAxis* axis) const {
    axis->SetBinLabel(axis->FindBin((double) eFlags::kPrimary), "Primary");
    axis->SetBinLabel(axis->FindBin((double) eFlags::kGlobal), "Global");
    axis->SetBinLabel(axis->FindBin((double) eFlags::kKnownMother), "Good secondary");
    axis->SetBinLabel(axis->FindBin((double) eFlags::kV0daughtersKnown), "V0-known-daughters");
    axis->SetBinLabel(axis->FindBin((double) eFlags::kV0daughtersUnknown), "V0-unknown-dauthers");

    axis->SetBinLabel(axis->FindBin((double) eFlags::kXidaughtersKnown), "Xi-knonw-daughters");
    axis->SetBinLabel(axis->FindBin((double) eFlags::kXidaughersUnknown), "Xi-uknown-daughters");
    axis->SetBinLabel(axis->FindBin((double) eFlags::kBackground), "Background");
    axis->SetBinLabel(axis->FindBin((double) eFlags::kEmbeded), "Embedded");
    axis->SetBinLabel(axis->FindBin((double) eFlags::kAny), "Any");
  }

  //==================================================================================================

  TrackFlagMonitor1D::TrackFlagMonitor1D() : PropertyMonitorX("Flag", "N", ECutUpdate::kTrack) {
    double size = fChecker.GetFlagNo();
    SetAxis(size, -0.5, size - 0.5, 0);
  }
  void TrackFlagMonitor1D::Update(Bool_t passed, TObject* obj) {
    auto res   = fChecker.GetFlags((Hal::Track*) (obj));
    auto histo = fHistoFailed;
    if (passed) { histo = fHistoPassed; }
    for (auto flag : res) {
      histo->Fill(flag);
    }
  }

  Bool_t TrackFlagMonitor1D::Init(Int_t task_id) {
    auto res = PropertyMonitorX::Init(task_id);
    if (!res) return kFALSE;
    fChecker.SetupAxis(fHistoPassed->GetXaxis());
    fChecker.SetupAxis(fHistoFailed->GetXaxis());
    return kTRUE;
  }

  //==================================================================================================

  TrackFlagMonitor2D::TrackFlagMonitor2D() : PropertyMonitorXY("Flag", "N", ECutUpdate::kTrack) {
    double size = fChecker.GetFlagNo();
    SetAxis(size, -0.5, size - 0.5, 0);
    SetAxis(size, -0.5, size - 0.5, 1);
  }


  void TrackFlagMonitor2D::Update(Bool_t passed, TObject* obj) {
    auto res   = fChecker.GetFlags((Hal::Track*) (obj));
    auto histo = fHistoFailed;
    if (passed) { histo = fHistoPassed; }
    for (auto flag1 : res) {
      for (auto flag2 : res) {
        histo->Fill(flag1, flag2);
      }
    }
  }

  Bool_t TrackFlagMonitor2D::Init(Int_t task_id) {
    auto res = PropertyMonitorXY::Init(task_id);
    if (!res) return kFALSE;
    fChecker.SetupAxis(fHistoPassed->GetXaxis());
    fChecker.SetupAxis(fHistoFailed->GetXaxis());
    fChecker.SetupAxis(fHistoPassed->GetYaxis());
    fChecker.SetupAxis(fHistoFailed->GetYaxis());
    return kTRUE;
  }

}  // namespace Hal
