/*
 * TrackChi2Cut.h
 *
 *  Created on: 13 lis 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKCHI2CUT_H_
#define HALTRACKCHI2CUT_H_
#include "TrackExpCut.h"
namespace Hal {
  class TrackChi2Cut : public TrackExpCut {
  public:
    TrackChi2Cut();
    virtual Bool_t Pass(Track* tr);
    virtual ~TrackChi2Cut();
    ClassDef(TrackChi2Cut, 1)
  };
}  // namespace Hal

#endif /* HALTRACKCHI2CUT_H_ */
