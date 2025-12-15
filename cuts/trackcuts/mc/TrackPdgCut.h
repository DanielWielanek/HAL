/*
 * TrackPdgCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTRACKPDGCUT_H_
#define HALTRACKPDGCUT_H_

#include "TrackMcCut.h"
/**
 * check track pid
 */
namespace Hal {

  class TrackPdgCut : public TrackMcCut {
  protected:
  public:
    TrackPdgCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackPdgCut();
    ClassDef(TrackPdgCut, 1)
  };

  class TrackPdgAbsCut : public TrackMcCut {
  protected:
  public:
    TrackPdgAbsCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackPdgAbsCut();
    ClassDef(TrackPdgAbsCut, 1)
  };
}  // namespace Hal
#endif /* HALPDGCUT_H_ */
