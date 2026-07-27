/*
 * TrackFlagMonitor1D.cxx
 *
 *  Created on: 7 paź 2025
 *      Author: daniel
 */

#include "TrackFlagMonitor.h"

#include <Rtypes.h>
#include <TAxis.h>
#include <TString.h>

#include "Cut.h"
#include "CutMonitor.h"
#include "FastHist.h"
#include "StdString.h"
#include "StdTypes.h"
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

  //==================================================================================================

  TrackFlagMonitor1D::TrackFlagMonitor1D() : PropertyMonitorX("Flag", "N", ECutUpdate::kTrack) {
    double size = fChecker.GetFlagNo();
    SetAxis(size, -0.5, size - 0.5, 0);
  }
  void TrackFlagMonitor1D::Update(Bool_t passed, TObject* obj) {
    auto res = fChecker.GetFlags((Hal::Track*) (obj));
    for (auto flag : res) {
      ManualFill1D(flag, passed);
    }
  }

  Bool_t TrackFlagMonitor1D::Init(Int_t task_id) {
    auto res = PropertyMonitorX::Init(task_id);
    if (!res) return kFALSE;
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
    return kTRUE;
  }

  void TrackFlagMonitor2D::CreateHistograms() {
    PropertyMonitorXY::CreateHistograms();
    auto labels = fChecker.GetLabels();

    ((FastHist2D*) fHistoPassed)->LabelizeAxisX(labels);
    ((FastHist2D*) fHistoPassed)->LabelizeAxisY(labels);
    ((FastHist2D*) fHistoFailed)->LabelizeAxisX(labels);
    ((FastHist2D*) fHistoFailed)->LabelizeAxisY(labels);
  }

  void TrackFlagMonitor1D::CreateHistograms() {
    PropertyMonitorX::CreateHistograms();
    auto labels = fChecker.GetLabels();
    ((FastHist2D*) fHistoPassed)->LabelizeAxisX(labels);
    ((FastHist2D*) fHistoFailed)->LabelizeAxisX(labels);
  }

  std::vector<TString> TrackFlagChecker::GetLabels() const {
    std::vector<TString> labels = {"Primary",
                                   "Global",
                                   "Good secondary",
                                   "V0-known-daughters",
                                   "V0-unknown-dauthers",
                                   "Xi-knonw-daughters",
                                   "Xi-uknown-daughters",
                                   "Background",
                                   "Embedded",
                                   "Any"};
    return labels;
  }

}  // namespace Hal
