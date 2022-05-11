/*
 * TwoTrackDebugCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTWOTRACKDEBUGCUT_H_
#define HALTWOTRACKDEBUGCUT_H_

#include "TwoTrackCut.h"
/**
 * class for debugging two track cuts
 */
namespace Hal {
  class TwoTrackDebugCut : public TwoTrackCut {
  public:
    TwoTrackDebugCut();
    Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackDebugCut();
    ClassDef(TwoTrackDebugCut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKDEBUGCUT_H_ */
