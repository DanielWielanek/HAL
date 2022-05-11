/*
 * TwoTrackAntiSplittingCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_CUTS_TWOTRACKCUTS_COMPOUND_HALTWOTRACKANTISPLITTINGCUT_H_
#define HALFEMTO_CUTS_TWOTRACKCUTS_COMPOUND_HALTWOTRACKANTISPLITTINGCUT_H_

#include "TwoTrackCut.h"

namespace Hal {
  class TwoTrackAntiSplittingCut : public TwoTrackCut {
  public:
    TwoTrackAntiSplittingCut();
    void AcceptOnlySplitted() { SetMinAndMax(1); }
    void AcceptOnlyNonSplitted() { SetMinAndMax(0); };
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackAntiSplittingCut();
    ClassDef(TwoTrackAntiSplittingCut, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO_CUTS_TWOTRACKCUTS_COMPOUND_HALTWOTRACKANTISPLITTINGCUT_H_ */
