/*
 * TrackRandomCut.h
 *
 *  Created on: 26 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKRANDOMCUT_H_
#define HALTRACKRANDOMCUT_H_
#include "TrackCut.h"

namespace Hal {
  class TrackRandomCut : public TrackCut {
  public:
    TrackRandomCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackRandomCut();
    ClassDef(TrackRandomCut, 1)
  };
}  // namespace Hal

#endif /* HALTRACKRANDOMCUT_H_ */
