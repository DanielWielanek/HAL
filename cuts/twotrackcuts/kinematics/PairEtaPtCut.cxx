/*
 * PairEtaPtCut.cxx
 *
 *  Created on: 2 sie 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "PairEtaPtCut.h"

#include "Track.h"
#include "TwoTrack.h"
#include <TMath.h>

namespace Hal {
  PairEtaPtCut::PairEtaPtCut() : TwoTrackCut(2) {
    SetUnitName("#eta_{pair} []", 0);
    SetUnitName("p_{T pair} [GeV/c]", 1);
  }

  Bool_t PairEtaPtCut::Pass(TwoTrack* pair) {
    Double_t px = pair->GetTrack1()->GetMomentum().Px() + pair->GetTrack2()->GetMomentum().Px();
    Double_t py = pair->GetTrack1()->GetMomentum().Py() + pair->GetTrack2()->GetMomentum().Py();
    Double_t pz = pair->GetTrack1()->GetMomentum().Pz() + pair->GetTrack2()->GetMomentum().Pz();
    Double_t pt = TMath::Sqrt(px * px + py * py);
    Double_t p  = TMath::Sqrt(pt * pt + pz * pz);
    SetValue(0.5 * TMath::Log((p + pz) / (p - pz)), 0);
    SetValue(pt, 1);
    return Validate();
  }

  PairEtaPtCut::~PairEtaPtCut() {}
}  // namespace Hal
