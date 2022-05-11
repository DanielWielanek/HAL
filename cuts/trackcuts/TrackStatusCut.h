/*
 * TrackStatusCut.h
 *
 *  Created on: 15-07-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKSTATUSCUT_H_
#define HALTRACKSTATUSCUT_H_
#include "TrackCut.h"

/**
 * check track status
 */
namespace Hal {
  class TrackStatusCut : public TrackCut {
  public:
    TrackStatusCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackStatusCut();
    ClassDef(TrackStatusCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKSTATUSCUT_H_ */
