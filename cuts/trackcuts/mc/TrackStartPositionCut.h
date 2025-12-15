/*
 * TrackStartPositionCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKSTARTPOSITIONCUT_H_
#define HALTRACKSTARTPOSITIONCUT_H_

#include "TrackMcCut.h"

namespace Hal {

  class TrackStartPositionCut : public TrackMcCut {
  public:
    TrackStartPositionCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackStartPositionCut();
    ClassDef(TrackStartPositionCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKSTARTPOSITIONCUT_H_ */
