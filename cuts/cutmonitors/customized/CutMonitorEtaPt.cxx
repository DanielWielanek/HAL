/*
 * CutMonitorEtaPt.cxx
 *
 *  Created on: 15 sty 2025
 *      Author: daniel
 */

#include "CutMonitorEtaPt.h"

#include "Track.h"

#include <TH2D.h>
#include <TLorentzVector.h>

namespace Hal {
  CutMonitorEtaPt::CutMonitorEtaPt() : PropertyMonitorXY("#eta", "p_{T} [GeV/c]", Hal::ECutUpdate::kTrack) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
  }

  void CutMonitorEtaPt::Update(Bool_t passed, TObject* obj) {
    auto track = (Hal::Track*) obj;
    if (passed) {
      ((TH2D*) fHistoPassed)->Fill(track->GetMomentum().Eta(), track->GetMomentum().Pt());
    } else {
      ((TH2D*) fHistoFailed)->Fill(track->GetMomentum().Eta(), track->GetMomentum().Pt());
    }
  }
} /* namespace Hal */
