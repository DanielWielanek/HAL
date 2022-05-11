/*
 * TwoTrackDeltaEtaCut.cxx
 *
 *  Created on: 28 lip 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TwoTrackDeltaEtaCut.h"

#include "Track.h"
#include "TwoTrack.h"
namespace Hal {
  TwoTrackDeltaEtaCut::TwoTrackDeltaEtaCut() : TwoTrackCut(1), fAntiValidate(kTRUE) { SetUnitName("#Delta#eta"); }

  Bool_t TwoTrackDeltaEtaCut::Pass(TwoTrack* pair) {
    Track* tr1    = pair->GetTrack1();
    Track* tr2    = pair->GetTrack2();
    Double_t deta = tr1->GetMomentum().Eta() - tr2->GetMomentum().Eta();
    SetValue(deta);
    if (fAntiValidate) return AntiValidate();
    return Validate();
  }

  TwoTrackDeltaEtaCut::~TwoTrackDeltaEtaCut() {
    // TODO Auto-generated destructor stub
  }

  Package* TwoTrackDeltaEtaCut::Report() const {
    Package* pack = TwoTrackCut::Report();

    return pack;
  }

  TwoTrackDeltaEtaCut::TwoTrackDeltaEtaCut(const TwoTrackDeltaEtaCut& other) : TwoTrackCut(other) {
    fAntiValidate = other.fAntiValidate;
  }
}  // namespace Hal
