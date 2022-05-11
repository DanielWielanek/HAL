/*
 * TrackEtaCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTRACKETACUT_H_
#define HALTRACKETACUT_H_

#include "TrackCut.h"
/**
 * check track pseudorapidity
 */

namespace Hal {
  class TrackEtaCut : public TrackCut {
  protected:
  public:
    TrackEtaCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackEtaCut();
    ClassDef(TrackEtaCut, 1)
  };

  /**
   * check absolute value of track pseudorapidity
   */
  class TrackEtaAbsCut : public TrackCut {
  public:
    TrackEtaAbsCut();
    virtual Bool_t Pass(Track* track);
    virtual Bool_t Init(Int_t task_id);
    virtual ~TrackEtaAbsCut();
    ClassDef(TrackEtaAbsCut, 1)
  };
}  // namespace Hal
#endif /* HALETACUT_H_ */
