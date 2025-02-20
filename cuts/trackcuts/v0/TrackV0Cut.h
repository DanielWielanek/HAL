/*
 * TrackV0Cut.h
 *
 *  Created on: 10 cze 2024
 *      Author: daniel
 */

#ifndef HAL_CUTS_TRACKCUTS_V0_TRACKV0CUT_H_
#define HAL_CUTS_TRACKCUTS_V0_TRACKV0CUT_H_

#include "TrackCut.h"

namespace Hal {
  class V0Track;

  class TrackV0Cut : public TrackCut {
  protected:
    virtual Bool_t PassV0(V0Track* tr) = 0;

  public:
    TrackV0Cut(Int_t params = 1) : TrackCut(params) {}
    virtual Bool_t Pass(Hal::Track* track);
    virtual ~TrackV0Cut() {};
    ClassDef(TrackV0Cut, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_TRACKCUTS_V0_TRACKV0CUT_H_ */
