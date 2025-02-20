/*
 * MonitorTrackKinem.cxx
 *
 *  Created on: 23 sty 2025
 *      Author: daniel
 */

#include "MonitorKinem.h"

#include <RtypesCore.h>
#include <TH2.h>
#include <TLorentzVector.h>

#include "Std.h"
#include "Track.h"

namespace Hal {

  MonitorEtaPt::MonitorEtaPt() : PropertyMonitorXY("#eta", "p_{T} [GeV/c]", Hal::ECutUpdate::kTrack) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
  }

  void MonitorEtaPt::Update(Bool_t passed, TObject* obj) {
    auto track = (Hal::Track*) obj;
    if (passed) {
      ((TH2D*) fHistoPassed)->Fill(track->GetMomentum().Eta(), track->GetMomentum().Pt());
    } else {
      ((TH2D*) fHistoFailed)->Fill(track->GetMomentum().Eta(), track->GetMomentum().Pt());
    }
  }

  MonitorYPt::MonitorYPt() : PropertyMonitorXY("y", "p_{T} [GeV/c]", Hal::ECutUpdate::kTrack) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
  }

  void MonitorYPt::Update(Bool_t passed, TObject* obj) {
    auto track = (Hal::Track*) obj;
    if (passed) {
      ((TH2D*) fHistoPassed)->Fill(track->GetMomentum().Rapidity(), track->GetMomentum().Pt());
    } else {
      ((TH2D*) fHistoFailed)->Fill(track->GetMomentum().Rapidity(), track->GetMomentum().Pt());
    }
  }

} /* namespace Hal */
