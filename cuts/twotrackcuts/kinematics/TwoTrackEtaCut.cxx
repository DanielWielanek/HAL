/*
 * TwoTrackEtaCut.cxx
 *
 *  Created on: 24-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackEtaCut.h"

#include "Track.h"
#include "TwoTrack.h"

#include <TLorentzVector.h>

namespace Hal {
  TwoTrackEtaCut::TwoTrackEtaCut() : TwoTrackCut(1) { SetUnitName("#eta_{pair}"); }

  Bool_t TwoTrackEtaCut::Pass(TwoTrack* pair) {
    TLorentzVector p = pair->GetTrack1()->GetMomentum();
    p += pair->GetTrack2()->GetMomentum();
    switch (pair->GetPairType()) {
      case TwoTrack::kRotated: {
        TLorentzVector p2 = pair->GetTrack2()->GetMomentum();
        p2.SetXYZT(-p2.X(), -p2.Y(), p2.Z(), p2.T());
        p += p2;
      } break;
      case TwoTrack::kHemishpere: {
        TLorentzVector p2 = pair->GetTrack2()->GetMomentum();
        p2.SetXYZT(p2.X(), -p2.Y(), -p2.Z(), p2.T());
        p += p2;
      } break;
      default: {
        TLorentzVector p2 = pair->GetTrack2()->GetMomentum();
        p += p2;
        break;
      }
    }
    SetValue(p.Eta());
    return Validate();
  }

  TwoTrackEtaCut::~TwoTrackEtaCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
