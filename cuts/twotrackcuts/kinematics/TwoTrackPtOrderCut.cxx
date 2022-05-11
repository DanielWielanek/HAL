/*
 * TwoTrackPtOrderCut.cxx
 *
 *  Created on: 30 sie 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TwoTrackPtOrderCut.h"

#include "Track.h"
#include "TwoTrack.h"
namespace Hal {
  TwoTrackPtOrderCut::TwoTrackPtOrderCut() : TwoTrackCut(1) { SetUnitName("Sloer [bool]"); }

  Bool_t TwoTrackPtOrderCut::Pass(TwoTrack* pair) {
    Double_t px1 = pair->GetTrack1()->GetPx();
    Double_t px2 = pair->GetTrack2()->GetPx();
    Double_t py1 = pair->GetTrack1()->GetPy();
    Double_t py2 = pair->GetTrack2()->GetPy();
    switch (pair->GetPairType()) {
      case TwoTrack::kRotated:
        px2 = -px2;
        py2 = -py2;
        break;
      case TwoTrack::kHemishpere:
        px2 = -px2;
        py2 = -py2;
        break;
      default: break;
    }
    Double_t pt1 = TMath::Sqrt(px1 * px1 + py1 * py1);
    Double_t pt2 = TMath::Sqrt(px2 * px2 + py2 * py2);

    Double_t phi1 = TMath::ATan2(py1, px1);
    Double_t phi2 = TMath::ATan2(py2, px2);
    Double_t dphi = TMath::Abs(phi1 - phi2);
    if (dphi > TMath::Pi()) dphi -= TMath::Pi();
    if (dphi > TMath::Pi() * 0.5) {
      SetValue(-1);
      ForcedUpdate(kFALSE);
    }
    SetValue(TMath::Abs(pt1 - pt2));
    return Validate();
  }

  TwoTrackPtOrderCut::~TwoTrackPtOrderCut() {}
}  // namespace Hal
