/*
 * TwoTrackPtSumCut.h
 *
 *  Created on: 04-05-2022
 *      Author: wielanek
 */

#ifndef HALTWOTRACKPTSUMCUT_H_
#define HALTWOTRACKPTSUMCUT_H_

#include "TwoTrackCut.h"
/**
 * check half of pair pt sum value |pt1+pt2|/2
 */
namespace Hal {
  class TwoTrackPtSumCut : public TwoTrackCut {
  public:
    TwoTrackPtSumCut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackPtSumCut();
    ClassDef(TwoTrackPtSumCut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKPTSUMCUT_H_ */
