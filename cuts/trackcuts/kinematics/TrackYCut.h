/*
 * TrackYCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKYCUT_H_
#define HALTRACKYCUT_H_
#include "TrackCut.h"
/**
 * check track rapidity
 */

namespace Hal {

  class TrackYCut : public TrackCut {
  public:
    TrackYCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackYCut();
    ClassDef(TrackYCut, 1)
  };
}  // namespace Hal

#endif /* HALTRACKYCUT_H_ */
