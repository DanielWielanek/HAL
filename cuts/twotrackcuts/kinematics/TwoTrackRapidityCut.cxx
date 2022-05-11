/*
 * TwoTrackRapidityCut.cxx
 *
 *  Created on: 30-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackRapidityCut.h"

#include "Track.h"
#include "TwoTrack.h"
namespace Hal {
  TwoTrackRapidityCut::TwoTrackRapidityCut() : TwoTrackCut(1) { SetUnitName("Y_{pair}"); }

  Bool_t TwoTrackRapidityCut::Pass(TwoTrack* pair) {
    Double_t e1  = pair->GetTrack1()->GetE();
    Double_t e2  = pair->GetTrack2()->GetE();
    Double_t pz1 = pair->GetTrack1()->GetPz();
    Double_t pz2 = pair->GetTrack2()->GetPz();
    if (pair->GetPairType() == TwoTrack::kHemishpere) { pz2 = -pz2; }
    Double_t Y = 0.5 * TMath::Log((e1 + e2 + pz1 + pz2) / (e1 + e2 - pz1 - pz2));
    SetValue(Y);
    return Validate();
  }

  TwoTrackRapidityCut::~TwoTrackRapidityCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
