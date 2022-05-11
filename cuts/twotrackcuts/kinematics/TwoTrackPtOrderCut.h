/*
 * TwoTrackPtOrderCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTWOTRACKPTORDERCUT_H_
#define HALTWOTRACKPTORDERCUT_H_

#include "TwoTrackCut.h"

/**
 * pass pair if Pt of first particle is larger than Pt of second particle
 */
namespace Hal {
  class TwoTrackPtOrderCut : public TwoTrackCut {
  public:
    TwoTrackPtOrderCut();
    virtual Bool_t Pass(TwoTrack* pair);
    ClassDef(TwoTrackPtOrderCut, 1) virtual ~TwoTrackPtOrderCut();
  };
}  // namespace Hal
#endif /* HALTWOTRACKPTORDERCUT_H_ */
