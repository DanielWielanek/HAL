/*
 * TrackPzCut.h
 *
 *  Created on: 5 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKPZCUT_H_
#define HALTRACKPZCUT_H_
#include "TrackCut.h"

namespace Hal {

  class TrackPzCut : public TrackCut {
  public:
    TrackPzCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackPzCut();
    ClassDef(TrackPzCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKPZCUT_H_ */
