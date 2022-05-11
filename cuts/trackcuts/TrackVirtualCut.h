/*
 * TrackVirtualCut.h
 *
 *  Created on: 07-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTRACKVIRTUALCUT_H_
#define HALTRACKVIRTUALCUT_H_

#include "TrackCut.h"
/**
 * virtual cut that pass all tracks and doesn't compute any values
 */
namespace Hal {
  class TrackVirtualCut : public TrackCut {
  public:
    /**
     * default constructor
     * @param i - number of monitored parameters
     */
    TrackVirtualCut();
    virtual Bool_t Pass(Track* /*track*/) {
      ForcedUpdate(kTRUE);
      return kTRUE;
    }
    virtual ~TrackVirtualCut();
    ClassDef(TrackVirtualCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKVIRTUALCUT_H_ */
