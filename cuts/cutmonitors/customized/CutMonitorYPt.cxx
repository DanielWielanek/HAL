/*
 * CutMonitorKinematics.cxx
 *
 *  Created on: 10 sty 2025
 *      Author: daniel
 */

#include "CutMonitorYPt.h"

#include <RtypesCore.h>
#include <TH2D.h>
#include <TLorentzVector.h>

#include "Track.h"

namespace Hal {

  CutMonitorYPt::CutMonitorYPt() : PropertyMonitorXY("y", "p_{T} [GeV/c]", Hal::ECutUpdate::kTrack) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
  }

  void CutMonitorYPt::Update(Bool_t passed, TObject* obj) {
    auto track = (Hal::Track*) obj;
    if (passed) {
      ((TH2D*) fHistoPassed)->Fill(track->GetMomentum().Rapidity(), track->GetMomentum().Pt());
    } else {
      ((TH2D*) fHistoFailed)->Fill(track->GetMomentum().Rapidity(), track->GetMomentum().Pt());
    }
  }

} /* namespace Hal */
