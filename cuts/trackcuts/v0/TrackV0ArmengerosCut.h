/*
 * TrackV0ArmengerosCut.h
 *
 *  Created on: 10 cze 2024
 *      Author: daniel
 */

#ifndef HAL_CUTS_TRACKCUTS_V0_TRACKV0ARMENGEROSCUT_H_
#define HAL_CUTS_TRACKCUTS_V0_TRACKV0ARMENGEROSCUT_H_

#include "TrackV0Cut.h"

namespace Hal {
  class V0Track;


  class TrackV0ArmengerosCut : public TrackV0Cut {
  protected:
    virtual Bool_t PassV0(V0Track* tr);

  public:
    TrackV0ArmengerosCut();
    virtual ~TrackV0ArmengerosCut() {};
    ClassDef(TrackV0ArmengerosCut, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_TRACKCUTS_V0_TRACKV0ARMENGEROSCUT_H_ */
