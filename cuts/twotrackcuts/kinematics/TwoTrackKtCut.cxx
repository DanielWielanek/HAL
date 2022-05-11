/*
 * TwoTrackKtCut.cxx
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackKtCut.h"

#include "Track.h"
#include "TwoTrack.h"

namespace Hal {
  TwoTrackKtCut::TwoTrackKtCut() : TwoTrackCut(1) { SetUnitName("k_{T} [GeV/c]"); }

  TwoTrackKtCut::~TwoTrackKtCut() {}

  Bool_t TwoTrackKtCut::Pass(TwoTrack* pair) {
    Double_t px = pair->GetTrack1()->GetPx();
    Double_t py = pair->GetTrack1()->GetPy();
    switch (pair->GetPairType()) {
      case TwoTrack::kRotated:
        px -= pair->GetTrack2()->GetPx();
        py -= pair->GetTrack2()->GetPy();
        break;
      case TwoTrack::kHemishpere:
        px -= pair->GetTrack2()->GetPx();
        py -= pair->GetTrack2()->GetPy();
        break;
      default:
        px += pair->GetTrack2()->GetPx();
        py += pair->GetTrack2()->GetPy();
        break;
    }
    SetValue((TMath::Sqrt(px * px + py * py)) / 2.0);
    return Validate();
  }
}  // namespace Hal
