/*
 * V0MassInvCut.h
 *
 *  Created on: 05-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_CUTS_TRACKCUTS_V0CUTS_HALV0MASSINVCUT_H_
#define HALFEMTO_CUTS_TRACKCUTS_V0CUTS_HALV0MASSINVCUT_H_

#include "TrackCut.h"

namespace Hal {
  class V0MassInvCut : public TrackCut {
  public:
    V0MassInvCut();
    virtual Bool_t Pass(Track* track);
    virtual ~V0MassInvCut();
    ClassDef(V0MassInvCut, 1)
  };
}  // namespace Hal


#endif /* HALFEMTO_CUTS_TRACKCUTS_V0CUTS_HALV0MASSINVCUT_H_ */
