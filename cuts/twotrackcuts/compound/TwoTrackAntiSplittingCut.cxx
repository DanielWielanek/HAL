/*
 * TwoTrackAntiSplittingCut.cxx
 *
 *  Created on: 28 kwi 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TwoTrackAntiSplittingCut.h"

#include "ComplexTrack.h"
#include "TwoTrack.h"

namespace Hal {
  TwoTrackAntiSplittingCut::TwoTrackAntiSplittingCut() : TwoTrackCut(1) { SetUnitName("AntiSplitting [Stat]"); }

  Bool_t TwoTrackAntiSplittingCut::Pass(TwoTrack* pair) {
    ComplexTrack* tr1 = (ComplexTrack*) pair->GetTrack1();
    ComplexTrack* tr2 = (ComplexTrack*) pair->GetTrack2();
    if (tr1->GetImgTrack() == nullptr) {
      SetValue(0);
      return ForcedUpdate(kTRUE);
    }
    if (tr1->GetImgTrack() == tr2->GetImgTrack()) {
      SetValue(1);
    } else {
      SetValue(0);
    }
    return Validate();
  }

  TwoTrackAntiSplittingCut::~TwoTrackAntiSplittingCut() {}
}  // namespace Hal
