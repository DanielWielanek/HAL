/*
 * TrackPtCut.cxx
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackPtCut.h"

#include "ComplexTrack.h"
#include "Track.h"

namespace Hal {

  TrackPtCut::TrackPtCut() : TrackCut(1) {
    SetUnitName("p_{T} [GeV/c]");
    SetMinMax(0, 1E+6);
  }

  TrackPtCut::~TrackPtCut() {}

  Bool_t TrackPtCut::Pass(Track* track) {
    Double_t px = (track->GetPx());
    Double_t py = (track->GetPy());
    Track* tr1  = track;
    tr1->SetBit(1);
    SetValue(TMath::Sqrt(px * px + py * py));
    return Validate();
  }
}  // namespace Hal
