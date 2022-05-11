/*
 * TwoTrackRapidityCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTWOTRACKRAPIDITYCUT_H_
#define HALTWOTRACKRAPIDITYCUT_H_

#include "TwoTrackCut.h"
/**
 * check pair rapidity
 */
namespace Hal {
  class TwoTrackRapidityCut : public TwoTrackCut {
  public:
    TwoTrackRapidityCut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackRapidityCut();
    ClassDef(TwoTrackRapidityCut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKRAPIDITYCUT_H_ */
