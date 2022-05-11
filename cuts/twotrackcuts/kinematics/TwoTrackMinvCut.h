/*
 * TwoTrackMinvCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTWOTRACKMINVCUT_H_
#define HALTWOTRACKMINVCUT_H_

#include "TwoTrackCut.h"
/**
 * check pair mass invariant value
 */
namespace Hal {
  class TwoTrackMinvCut : public TwoTrackCut {
  public:
    TwoTrackMinvCut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackMinvCut();
    ClassDef(TwoTrackMinvCut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKMINVCUT_H_ */
