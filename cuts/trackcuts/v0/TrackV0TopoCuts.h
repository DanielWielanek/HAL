/*
 * TrackV0TopoCuts.h
 *
 *  Created on: 10 cze 2024
 *      Author: daniel
 */

#ifndef HAL_CUTS_TRACKCUTS_V0_TRACKV0TOPOCUTS_H_
#define HAL_CUTS_TRACKCUTS_V0_TRACKV0TOPOCUTS_H_

#include "TrackV0Cut.h"

namespace Hal {

  class TrackV0DaughterDistCut : public TrackV0Cut {
  protected:
    virtual Bool_t PassV0(V0Track* tr);

  public:
    TrackV0DaughterDistCut();
    virtual ~TrackV0DaughterDistCut() {};
    ClassDef(TrackV0DaughterDistCut, 1)
  };

  class TrackV0DecLenCut : public TrackV0Cut {
  protected:
    virtual Bool_t PassV0(V0Track* tr);

  public:
    TrackV0DecLenCut();
    virtual ~TrackV0DecLenCut() {};
    ClassDef(TrackV0DecLenCut, 1)
  };

  class TrackV0CosAngleCut : public TrackV0Cut {
  protected:
    virtual Bool_t PassV0(V0Track* tr);

  public:
    TrackV0CosAngleCut();
    virtual ~TrackV0CosAngleCut() {};
    ClassDef(TrackV0CosAngleCut, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_TRACKCUTS_V0_TRACKV0TOPOCUTS_H_ */
