/*
 * TrackThetaCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_CUTS_TRACKCUTS_KINEMATICS_HALTRACKTHETACUT_H_
#define HALFEMTO_CUTS_TRACKCUTS_KINEMATICS_HALTRACKTHETACUT_H_

#include "TrackCut.h"

namespace Hal {

  class TrackThetaCut : public TrackCut {
  public:
    TrackThetaCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackThetaCut();
    ClassDef(TrackThetaCut, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO_CUTS_TRACKCUTS_KINEMATICS_HALTRACKTHETACUT_H_ */
