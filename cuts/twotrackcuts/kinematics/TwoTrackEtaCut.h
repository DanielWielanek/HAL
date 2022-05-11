/*
 * TwoTrackEtaCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTWOTRACKETACUT_H_
#define HALTWOTRACKETACUT_H_

#include "TwoTrackCut.h"
/**
 * check pair eta value
 */
namespace Hal {
  class TwoTrackEtaCut : public TwoTrackCut {
  public:
    TwoTrackEtaCut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackEtaCut();
    ClassDef(TwoTrackEtaCut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKETACUT_H_ */
