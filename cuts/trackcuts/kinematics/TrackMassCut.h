/*
 * TrackMassCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTRACKMASSCUT_H_
#define HALTRACKMASSCUT_H_

#include "TrackCut.h"
/**
 * check track mass
 */
namespace Hal {
  class TrackMassCut : public TrackCut {
  public:
    TrackMassCut();
    Bool_t Pass(Track* track);
    virtual ~TrackMassCut();
    ClassDef(TrackMassCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKMASSCUT_H_ */
