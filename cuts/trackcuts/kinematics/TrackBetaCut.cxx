/*
 * TrackBetaCut.cxx
 *
 *  Created on: 23 lut 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackBetaCut.h"

#include "Track.h"

namespace Hal {
  TrackBetaCut::TrackBetaCut() : TrackCut(1) { SetUnitName("#beta"); }

  Bool_t TrackBetaCut::Pass(Track* track) {
    Double_t px   = track->GetPx();
    Double_t py   = track->GetPy();
    Double_t pz   = track->GetPz();
    Double_t p    = TMath::Abs(px * px + py * py + pz * pz);
    Double_t beta = p / track->GetE();
    SetValue(beta);
    return Validate();
  }

  TrackBetaCut::~TrackBetaCut() {}
}  // namespace Hal
