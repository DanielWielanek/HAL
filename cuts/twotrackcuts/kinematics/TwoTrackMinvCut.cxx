/*
 * TwoTrackMinvCut.cpp
 *
 *  Created on: 25-03-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackMinvCut.h"

#include "Track.h"
#include "TwoTrack.h"

namespace Hal {
  TwoTrackMinvCut::TwoTrackMinvCut() : TwoTrackCut(1) { SetUnitName("M_{inv} [GeV/c]"); }

  Bool_t TwoTrackMinvCut::Pass(TwoTrack* pair) {
    Double_t px1, py1, pz1, e1;
    Double_t px2, py2, pz2, e2;
    px1 = pair->GetTrack1()->GetPx();
    py1 = pair->GetTrack1()->GetPy();
    pz1 = pair->GetTrack1()->GetPz();
    e1  = pair->GetTrack1()->GetE();
    switch (pair->GetPairType()) {
      case TwoTrack::kHemishpere: {
        px2 = -pair->GetTrack2()->GetPx();
        py2 = -pair->GetTrack2()->GetPy();
        pz2 = -pair->GetTrack2()->GetPz();
        e2  = pair->GetTrack2()->GetE();
      } break;
      case TwoTrack::kRotated: {
        px2 = -pair->GetTrack2()->GetPx();
        py2 = -pair->GetTrack2()->GetPy();
        pz2 = pair->GetTrack2()->GetPz();
        e2  = pair->GetTrack2()->GetE();
      } break;
      default: {
        px2 = pair->GetTrack2()->GetPx();
        py2 = pair->GetTrack2()->GetPy();
        pz2 = pair->GetTrack2()->GetPz();
        e2  = pair->GetTrack2()->GetE();
      } break;
    }

    Double_t m_inv =
      TMath::Sqrt((e1 + e2) * (e1 + e2) - (px1 + px2) * (px1 + px2) - (py1 + py2) * (py1 + py2) - (pz1 + pz2) * (pz1 + pz2));
    SetValue(m_inv);
    return Validate();
  }

  TwoTrackMinvCut::~TwoTrackMinvCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
