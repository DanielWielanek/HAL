/*
 * TwoTrackVirtualCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTWOTRACKVIRTUALCUT_H_
#define HALTWOTRACKVIRTUALCUT_H_

#include "TwoTrackCut.h"
/**
 * virtual cut that pass all pairs of tracks and doesn't compute any values
 */
namespace Hal {
  class TwoTrackVirtualCut : public TwoTrackCut {
  protected:
  public:
    TwoTrackVirtualCut();
    virtual Bool_t Pass(TwoTrack* /*pair*/) {
      ForcedUpdate(kTRUE);
      return kTRUE;
    };
    virtual ~TwoTrackVirtualCut();
    ClassDef(TwoTrackVirtualCut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKVIRTUALCUT_H_ */
