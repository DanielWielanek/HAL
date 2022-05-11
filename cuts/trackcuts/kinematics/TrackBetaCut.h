/*
 * TrackBetaCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKBETACUT_H_
#define HALTRACKBETACUT_H_

#include "TrackCut.h"
/**
 * class for making beta cut
 */

namespace Hal {
  class TrackBetaCut : public TrackCut {
  public:
    TrackBetaCut();
    Bool_t Pass(Track* track);
    virtual ~TrackBetaCut();
    ClassDef(TrackBetaCut, 1);
  };
}  // namespace Hal
#endif /* HALTRACKBETACUT_H_ */
