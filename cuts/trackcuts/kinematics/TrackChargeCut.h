/*
 * TrackChargeCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKCHARGECUT_H_
#define HALTRACKCHARGECUT_H_
#include "TrackCut.h"

namespace Hal {
  class TrackChargeCut : public TrackCut {
  public:
    TrackChargeCut();
    Bool_t Pass(Track* tr);
    virtual ~TrackChargeCut();
    ClassDef(TrackChargeCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKCHARGECUT_H_ */
