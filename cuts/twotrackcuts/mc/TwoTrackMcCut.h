/*
 * TwoTrackMcCut.h
 *
 *  Created on: 27 mar 2026
 *      Author: daniel
 */

#ifndef HAL_CUTS_TWOTRACKCUTS_MC_TWOTRACKMCCUT_H_
#define HAL_CUTS_TWOTRACKCUTS_MC_TWOTRACKMCCUT_H_

#include <RtypesCore.h>

#include "TwoTrackCut.h"


namespace Hal {

  class TwoTrackMcCut : public TwoTrackCut {
  public:
    TwoTrackMcCut(Int_t size);
    virtual Bool_t Init(Int_t taskid);
    virtual ~TwoTrackMcCut() {};
    ClassDef(TwoTrackMcCut, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_TWOTRACKCUTS_MC_TWOTRACKMCCUT_H_ */
