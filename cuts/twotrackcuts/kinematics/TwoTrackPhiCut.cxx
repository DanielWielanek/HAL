/*
 * TwoTrackPhiCut.cxx
 *
 *  Created on: 08-12-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TwoTrackPhiCut.h"

#include "Track.h"
#include "TwoTrack.h"
namespace Hal {
  TwoTrackPhiCut::TwoTrackPhiCut() : TwoTrackCut(1) {}

  Bool_t TwoTrackPhiCut::Pass(TwoTrack* pair) {
    Double_t px, py;
    switch (pair->GetPairType()) {
      case TwoTrack::kRotated:
        px = pair->GetTrack1()->GetPx() - pair->GetTrack2()->GetPx();
        py = pair->GetTrack1()->GetPy() - pair->GetTrack2()->GetPy();
        break;
      case TwoTrack::kHemishpere:
        px = pair->GetTrack1()->GetPx() - pair->GetTrack2()->GetPx();
        py = pair->GetTrack1()->GetPy() - pair->GetTrack2()->GetPy();
        break;
      default:
        px = pair->GetTrack1()->GetPx() + pair->GetTrack2()->GetPx();
        py = pair->GetTrack1()->GetPy() + pair->GetTrack2()->GetPy();
        break;
    }
    SetValue(TMath::ATan2(py, px));
    return Validate();
  }

  TwoTrackPhiCut::~TwoTrackPhiCut() {}
}  // namespace Hal
