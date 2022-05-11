/*
 * TwoTrackPtSumCut.cxx
 *
 *  Created on: 12-06-2014
 *      Author: wielanek
 */

#include "TwoTrackPtSumCut.h"

#include "Track.h"
#include "TwoTrack.h"
namespace Hal {
  TwoTrackPtSumCut::TwoTrackPtSumCut() : TwoTrackCut(1) { SetUnitName("p_{T,sum} [GeV/c]"); }

  Bool_t TwoTrackPtSumCut::Pass(TwoTrack* pair) {
    Double_t px  = pair->GetTrack1()->GetPx();
    Double_t py  = pair->GetTrack1()->GetPy();
    Double_t pt1 = TMath::Sqrt(px * px + py * py);
    px           = pair->GetTrack2()->GetPx();
    py           = pair->GetTrack2()->GetPy();
    Double_t pt2 = TMath::Sqrt(px * py + py * py);
    SetValue((pt1 + pt2) / 2.0);
    return Validate();
  }

  TwoTrackPtSumCut::~TwoTrackPtSumCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
